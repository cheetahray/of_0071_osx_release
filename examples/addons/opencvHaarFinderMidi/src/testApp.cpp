#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//img.loadImage("test.jpg");
	finder.setup("parojosG.xml");    //parojos
	vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(rayx,rayy);
    colorImg.allocate(rayx,rayy);
	grayImage.allocate(rayx,rayy);

}

//--------------------------------------------------------------
void testApp::update(){
    vidGrabber.grabFrame();
    colorImg.setFromPixels(vidGrabber.getPixels(), rayx,rayy);
    grayImage = colorImg;
    finder.findHaarObjects(grayImage);
    ofSleepMillis(2000);
}

//--------------------------------------------------------------
void testApp::draw(){
	grayImage.draw(0, 0);
	ofNoFill();
	for(int i = 0; i < finder.blobs.size(); i++) {
		ofRectangle cur = finder.blobs[i].boundingRect;
		ofRect(cur.x, cur.y, cur.width, cur.height);
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