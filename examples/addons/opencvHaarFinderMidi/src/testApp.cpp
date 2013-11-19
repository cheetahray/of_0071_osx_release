#include "testApp.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

Poco::Timestamp::TimeDiff hdelay = 1000000; // delay between 2 events, in microseconds
Poco::Timestamp::TimeDiff nextTime = 0;    // time when next event occurs (calculated in seqTimerFunc)

void seqTimer::seqTimerFunc(Poco::Timestamp::TimeDiff curTime)
// Events that are regularly sent are processed here
{
    if(curTime >= nextTime)
    {
        nextTime += hdelay;
        for(int rayi = 0; rayi < 58; rayi++)
        {
            if(sixtyfour[rayi] > 0)
            {
                sixtyfour[rayi]--;
            }
            
            if(rayi < NUM_MSG_STRINGS && twenty[rayi] > 0)
            {
                twenty[rayi]--;
            }
        }
        //cout <<  (long)(curTime / 1000) <<  " .. " << (long)(nextTime / 1000) << endl;
        // Execute real messages (midi etc) here ...
    }
}

//--------------------------------------------------------------
void testApp::setup(){
    ofHideCursor();
	//img.loadImage("test.jpg");
	finder.setup("parojosG.xml");    //parojos
	vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(rayx,rayy);
    colorImg.allocate(rayx,rayy);
	grayImage.allocate(rayx,rayy);
    totalPixels = rayx*rayy;
    videoInverted 	= new unsigned char[totalPixels*3];
    finderblobssize = tempptsx = 0;
    iPadFace = 1;
    // open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    receiver.setup(PORTII);
    // and in the method void testApp::setup()
    timer = new Timer(0, 10); // parameters : immediate and delay of 10 milliseconds (fast enough i think)
    timer->start(TimerCallback<seqTimer>(sTimer, &seqTimer::onTimer), Thread::PRIO_HIGHEST);
}

int compare (const void * a, const void * b)
{
    return ( ((ofVec2f*)a)->x - ((ofVec2f*)b)->x );
}

//--------------------------------------------------------------
void testApp::update(){
    vidGrabber.grabFrame();
    if(vidGrabber.isFrameNew())
    {
        unsigned char * pixels = vidGrabber.getPixels();
        for (int i = 0; i < totalPixels; i++)
        {
            int raymiddle = i%rayx;
            int rayii = i*3;
            if(raymiddle >= 364 && raymiddle <= 596)
            {
                memcpy(videoInverted + rayii, pixels + rayii, 3);
            }
            else
            {
                videoInverted[rayii] = videoInverted[rayii+1] = videoInverted[rayii+2] = 0;
            }
        }
        colorImg.setFromPixels(videoInverted, rayx,rayy);
        grayImage = colorImg;
        if(iPadFace > 0)
            finder.findHaarObjects(grayImage);
    
        //ofSleepMillis(raysleep);
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofTranslate(-192, 0);
#ifdef RAYDRAW
	grayImage.draw(0, 0);
#endif
    finderblobssize = iPadFace>0?min(finder.blobs.size(),MAX_N_PTS):0;
    /*
    for(int i = 0; i < finderblobssize; i++) 
    {
        ofRectangle cur = finder.blobs[i].boundingRect;
		pts[i].set(cur.x + cur.width/2, cur.y + cur.height/2);
	}
    qsort(pts, finderblobssize, sizeof(ofVec2f), compare);
    */
    int tempptsy = 56;
    float rayline = 0;
    string rayptsy;
    
    if(tempptsx > NUM_MSG_STRINGS)
        tempptsx = 1;
    
    for(int i = 0; i < finderblobssize; i++) 
    {
        ofRectangle cur = finder.blobs[i].boundingRect;
        
        tempptsy = 58 - ( ( (int)cur.y * 58 ) / rayy );
        //tempptsx = 1 + ( (int)pts[i].x * NUM_MSG_STRINGS ) / rayx;
        //mm.setAddress("/D57");
        mm.setAddress("/D" + ofToString(tempptsy) );
        mm.addFloatArg(1);
        mm.addIntArg( tempptsx ); // must be <= new iPad's NUM_MSG_STRINGS
        sTimer.sixtyfour[tempptsy-1] = 1;
        sTimer.haveyou[tempptsy-1] = false;
        sender.sendMessage(mm);
        mm.clear();
        
        
        mm.setAddress("/2/multifader/" + ofToString(tempptsx) );
        mm.addIntArg(tempptsy);
        if(true == sTimer.already[tempptsx-1])
        {
            sTimer.twenty[tempptsx-1] = 5;
            sTimer.already[tempptsx-1] = false;
        }
        sender.sendMessage(mm);
        mm.clear();
        
        mm.setAddress("/2/label" + ofToString(20 + tempptsx) );
        switch(tempptsy%12)
        {
            case 0:
                rayptsy = "Ti";
                break;
            case 1:
                rayptsy = "Do";
                break;
            case 2:
                rayptsy = "Do+";
                break;
            case 3:
                rayptsy = "Re";
                break;
            case 4:
                rayptsy = "Re+";
                break;
            case 5:
                rayptsy = "Mi";
                break;
            case 6:
                rayptsy = "Fa";
                break;
            case 7:
                rayptsy = "Fa+";
                break;
            case 8:
                rayptsy = "So";
                break;
            case 9:
                rayptsy = "So+";
                break;
            case 10:
                rayptsy = "La";
                break;
            case 11:
                rayptsy = "La+";
                break;
        }
        mm.addStringArg(rayptsy);
        sender.sendMessage(mm);
        mm.clear();
        
        ofNoFill();
		ofRect(cur.x, cur.y, cur.width, cur.height);
        rayline = cur.y+cur.height/2;
        ofFill();
        if(i%2 == 0)
        {
            ofLine(300, rayline, cur.x, rayline);
            ofCircle(300, rayline, 3);
            ofDrawBitmapString(rayptsy.insert(0," ").insert(0,ofToString(rayline)), 210, rayline);
        }
        else
        {
            ofLine(cur.x+cur.width, rayline, 660, rayline);
            ofCircle(660, rayline, 3);
            ofDrawBitmapString(rayptsy.insert(0," ").insert(0,ofToString(rayline)), 670, rayline);
        }
        
    }

    // check for waiting messages
	while(receiver.hasWaitingMessages())
    {
		// get the next message
		receiver.getNextMessage(&mmmm);
        
        if(mmmm.getAddress() == "/FACE")
        {
            iPadFace = mmmm.getArgAsFloat(0);
        }
        else 
        {
            mm.setAddress(mmmm.getAddress());
            mm.addFloatArg(mmmm.getArgAsFloat(0));
            sender.sendMessage(mm);
            mm.clear();
        }
		
        mmmm.clear();
        
    }

    for(int rayi = 0; rayi < 58; rayi++)
    {
        if( 0 == sTimer.sixtyfour[rayi] && false == sTimer.haveyou[rayi] )
        {
            //mm.setAddress("/D57");
            mm.setAddress("/D" + ofToString(rayi+1) );
            mm.addFloatArg(0);
            sender.sendMessage(mm);
            mm.clear();
            sTimer.haveyou[rayi] = true;
        }
        
        if(rayi < NUM_MSG_STRINGS && 0 == sTimer.twenty[rayi] && false == sTimer.already[rayi] )
        {
            mm.setAddress("/2/multifader/" + ofToString(rayi+1) );
            mm.addIntArg(0);
            sender.sendMessage(mm);
            mm.clear();
            
            mm.setAddress("/2/label" + ofToString(rayi+21) );
            mm.addStringArg("");
            sender.sendMessage(mm);
            mm.clear();
            
            tempptsx++;
            sTimer.already[rayi] = true;
        }
    }

    if(nextTime > 2000000000)
    {
        nextTime = 0;
        timer->restart();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}