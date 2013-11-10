#include "testApp.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

Poco::Timestamp::TimeDiff hdelay = 300000; // delay between 2 events, in microseconds
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
            
            if(rayi < 20 && twenty[rayi] > 0)
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
	//img.loadImage("test.jpg");
	finder.setup("parojosG.xml");    //parojos
	vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(rayx,rayy);
    colorImg.allocate(rayx,rayy);
	grayImage.allocate(rayx,rayy);
    finderblobssize = 0;
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
    colorImg.setFromPixels(vidGrabber.getPixels(), rayx,rayy);
    grayImage = colorImg;
    finder.findHaarObjects(grayImage);
    
    ofSleepMillis(raysleep);
}

//--------------------------------------------------------------
void testApp::draw(){
#ifdef RAYDRAW
	grayImage.draw(0, 0);
	ofNoFill();
#endif
    finderblobssize = min(finder.blobs.size(),MAX_N_PTS) ;
    for(int i = 0; i < finderblobssize; i++) 
    {
        ofRectangle cur = finder.blobs[i].boundingRect;
		pts[i].set(cur.x + cur.width/2, cur.y + cur.height/2);
        ofRect(cur.x, cur.y, cur.width, cur.height);
	}
    qsort(pts, finderblobssize, sizeof(ofVec2f), compare);
    
    int tempptsy = 56;
    int tempptsx = 0;
    
    for(int i = 0; i < finderblobssize; i++) 
    {
        tempptsy = 58 - ( ( (int)pts[i].y * 58 ) / rayy );
        tempptsx = 1 + ( (int)pts[i].x * 20 ) / rayx;
        mm.setAddress("/D57");
        //mm.setAddress("/D" + ofToString(tempptsy) );
        mm.addIntArg(127);
        mm.addIntArg( tempptsx ); // must be <= new iPad's NUM_MSG_STRINGS
        sTimer.sixtyfour[tempptsy-1] = 2;
        sTimer.haveyou[tempptsy-1] = false;
        sender.sendMessage(mm);
        mm.clear();
        
        mm.setAddress("/2/multifader/" + ofToString(tempptsx) );
        mm.addIntArg(tempptsy);
        sTimer.twenty[tempptsx-1] = 4;
        sTimer.already[tempptsx-1] = false;
        sender.sendMessage(mm);
        mm.clear();
        
        mm.setAddress("/2/label" + ofToString(20 + tempptsx) );
        switch(tempptsy%12)
        {
            case 0:
                mm.addStringArg("Ti");
                break;
            case 1:
                mm.addStringArg("Do");
                break;
            case 2:
                mm.addStringArg("Do+");
                break;
            case 3:
                mm.addStringArg("Re");
                break;
            case 4:
                mm.addStringArg("Re+");
                break;
            case 5:
                mm.addStringArg("Mi");
                break;
            case 6:
                mm.addStringArg("Fa");
                break;
            case 7:
                mm.addStringArg("Fa+");
                break;
            case 8:
                mm.addStringArg("So");
                break;
            case 9:
                mm.addStringArg("So+");
                break;
            case 10:
                mm.addStringArg("La");
                break;
            case 11:
                mm.addStringArg("La+");
                break;
        }
        sender.sendMessage(mm);
        mm.clear();
        
        //ofRect(pts[i].x, pts[i].y, 20, 20);
    }

    // check for waiting messages
	while(receiver.hasWaitingMessages())
    {
		// get the next message
		receiver.getNextMessage(&mmmm);
        
		// check for mouse moved message
		if(mmmm.getAddress() == "/3/toggle1")
        {
			// both the arguments are int32's
			mm.setAddress("/MIDI");
            mm.addFloatArg(mmmm.getArgAsFloat(0));
            sender.sendMessage(mm);
            mm.clear();
        }
		// check for mouse button message
		else if(mmmm.getAddress() == "/3/toggle2")
        {
			// both the arguments are int32's
			mm.setAddress("/OSC");
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
            mm.setAddress("/D57");
            //mm.setAddress("/D" + ofToString(rayi+1) );
            mm.addIntArg(0);
            sender.sendMessage(mm);
            mm.clear();
            sTimer.haveyou[rayi] = true;
        }
        
        if(rayi < 20 && 0 == sTimer.twenty[rayi] && false == sTimer.already[rayi] )
        {
            mm.setAddress("/2/multifader/" + ofToString(rayi+1) );
            mm.addIntArg(0);
            sender.sendMessage(mm);
            mm.clear();
            
            mm.setAddress("/2/label" + ofToString(rayi+21) );
            mm.addStringArg("");
            sender.sendMessage(mm);
            mm.clear();
            
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