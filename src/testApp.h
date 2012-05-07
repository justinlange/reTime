#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "MeshMaker.h"
#include "ofxOsc.h"

//OSC
#define PORT 12345
#define PORT2 9000
#define NUM_MSG_STRINGS 20


//#include "ofxGrabCam.h"


class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void drawReport();
		//-----------------------
        void updateOsc();
        void updateOsc2();
		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		//-----------------------
		ofxKinect kinect;
		ofEasyCam easyCam;
        MeshMaker* myMesh;
    
        //-------OSC stuff----------------

        ofxOscReceiver	receiver;
        ofxOscReceiver	receiver2;

        int				current_msg_string;
        string          msg_strings[NUM_MSG_STRINGS];
        float			timers[NUM_MSG_STRINGS];
        int             razorYaw, razorPitch, razorRoll;
        float           fWiiX, fWiiY;
        int             wiiX, wiiY;

};
