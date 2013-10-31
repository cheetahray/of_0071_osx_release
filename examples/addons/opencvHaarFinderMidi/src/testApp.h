#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxOsc.h"

#define rayx    960
#define rayy    720
#define MAX_N_PTS         15
#define RAYDRAW
#define HOST "192.168.11.137"
#define PORT 12345
#define raysleep 300

class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
        void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		
    
		//ofImage img;
		ofxCvHaarFinder finder;
        ofxCvColorImage			colorImg;
        ofxCvGrayscaleImage 	grayImage;
        ofVideoGrabber 		vidGrabber;    
        ofVec2f	pts[MAX_N_PTS];
        ofxOscSender sender;
        ofxOscMessage mm;
    
        int finderblobssize;
};
