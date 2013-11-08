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
        for(int rayi = 0; rayi < 64; rayi++)
        {
            if(sixtyfour[rayi] > 0)
            {
                sixtyfour[rayi]--;
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
    for(int i = 0; i < finderblobssize; i++) {
        ofRectangle cur = finder.blobs[i].boundingRect;
		pts[i].set(cur.x + cur.width/2, cur.y + cur.height/2);
        ofRect(cur.x, cur.y, cur.width, cur.height);
	}
    qsort(pts, finderblobssize, sizeof(ofVec2f), compare);
    int tempptsy = 56;
    for(int i = 0; i < finderblobssize; i++) 
    {
        tempptsy = ( (int)pts[i].y << 6 );
        mm.setAddress("/D57");
        //mm.setAddress("/D" + ofToString( tempptsy / rayy) );
        mm.addIntArg(100);
        sTimer.sixtyfour[tempptsy] = 2;
        sender.sendMessage(mm);
        mm.clear();
        //ofRect(pts[i].x, pts[i].y, 20, 20);
    }

    for(int rayi = 0; rayi < 64; rayi++)
    {
        if(sTimer.sixtyfour[rayi] == 1)
        {
            mm.setAddress("/D57");
            //mm.setAddress("/D" + ofToString(rayi+1) );
            mm.addIntArg(0);
            sender.sendMessage(mm);
            mm.clear();
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