#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxOsc.h"

#include "Poco/Stopwatch.h"
#include "Poco/Thread.h"
#include "Poco/Timestamp.h"
#include "Poco/Timer.h"

using Poco::Stopwatch;
using Poco::Thread;
using Poco::Timer;
using Poco::TimerCallback;

#define rayx    960
#define rayy    720
#define MAX_N_PTS         15
#define NUM_MSG_STRINGS   20
#define RAYDRAW
#define HOST "192.168.11.255"
#define PORT 12345
//#define raysleep 150

#define PORTII 54321

class seqTimer
{
    public:
        void seqTimerFunc(Poco::Timestamp::TimeDiff curTime); // prototype of my function

        seqTimer()
        {
            stopwatch.start();
        }
    
        void onTimer(Timer& timer)
        {
            seqTimerFunc(stopwatch.elapsed()); // function call
        }
    
        int sixtyfour[64], twenty[NUM_MSG_STRINGS];
        bool haveyou[64], already[NUM_MSG_STRINGS];
    
    private:
        Stopwatch stopwatch;
};

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
        //ofVec2f	pts[MAX_N_PTS];
        ofxOscSender sender;
        ofxOscMessage mm,mmmm;
        ofxOscReceiver receiver;
        
        int finderblobssize, tempptsx, totalPixels;
        
        // and I declare in class testApp : public ofBaseApp
        seqTimer sTimer;
        Timer * timer;
    
        unsigned char * 	videoInverted;
        float iPadFace;
};
