#include "testApp.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

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
    int mmsleep = raysleep / (finderblobssize+1) ; 
	for(int i = 0; i < finderblobssize; i++) 
    {
        //mm.setAddress("/D57");
        mm.setAddress("/D" + ofToString( ( (int)pts[i].y << 6 ) / rayy) );
        mm.addIntArg(100);
        sender.sendMessage(mm);
        mm.clear();
        //ofRect(pts[i].x, pts[i].y, 20, 20);
        ofSleepMillis(mmsleep);
        //mm.setAddress("/D57");
        mm.setAddress("/D" + ofToString( ( (int)pts[i].y << 6 ) / rayy) );
        mm.addIntArg(0);
        sender.sendMessage(mm);
        mm.clear();
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