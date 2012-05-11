#include "testApp.h"
#include "MeshMaker.h"

using namespace std;

// kinect frame width
#define KW 640
// kinect frame height
#define KH 480
// for the average fps calculation
#define FPS_MEAN 30


bool mode3D = true;

int kTilt = 0;



int frameCount = 0;
int lastMillis = 0;

bool bPastMesh = false;

int rotateXvalue = 0;
int rotateYvalue = 0;
int rotateZvalue = 180;

int PX;
int PY;
int PZ;
int DX;
int DY;
int DZ;
int diffX;
int diffY;
int eyeDistance = 7; //change later to 7.5
int transZ;
int wiiMap = 600;


ostringstream controlString;     
string controlStringToShow;

float smoothPZ = 650;

float smoothValue = 0.05;
float smoothVal = 0.05;

int fullWindowWidth;
int fullWindowHeight;



//float posX = 0;
//float posY = 0;
//float posZ = 0;
//float dirX = 0;
//float dirY = 0;
//float dirZ = 0;

float eyePoint = 500;
int defaultVar = 0;



//--------------------------------------------------------------
void testApp::setup(){
    
    
    
    //set up OSC
    
    receiver.setup( PORT );
    receiver2.setup( PORT2 );
    
    razorPitch = 0;
    razorYaw = 0;
    razorRoll = 0;
    wiiX = 0;
    wiiY = 0;
    wiiZ = 0;


    
    
    ofSetVerticalSync(true);
	glEnable(GL_DEPTH_TEST);
    // enable depth->rgb image calibration
	kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    kinect.setDepthClipping(800, 4000);


    
    myMesh = new MeshMaker;
    
    fullWindowWidth = ofGetWindowWidth();
    fullWindowHeight = ofGetWindowHeight();

    
    //above constructor contains default arguments, which do not have to be stated if the default is acceptable 
    
}

//--------------------------------------------------------------
void testApp::update(){
  
    updateOsc();
//    updateOsc2();
    
//        kinect.update();
//        if (kinect.isFrameNew()) {
//            myMesh->recordTime( &kinect ) ;   //& gets the pointer for the object           
//            }
//        
    
}

//--------------------------------------------------------------
void testApp::drawReport() {
    ostringstream controlString;
    controlString << "razorPitch(q-a) " << razorPitch << "razorYaw (w-s) " << razorYaw << "razorRoll(e-d) "<< razorRoll << "\n" << "wiiX(r-f) " << wiiX << "wiiY(t-g)" << wiiY << "\n" << "wiiMap(7-8): " << wiiMap;    
    controlStringToShow = controlString.str(); 
    ofDrawBitmapString(ofToString(controlStringToShow), 10,10);
}
//--------------------------------------------------------------
void testApp::draw(){
    
    ofBackground(0);
    
    if (mode3D == false) {
        drawReport(); 
    }

    if (mode3D == false) {
        ofTranslate(fullWindowWidth/2,fullWindowHeight/2); 
        cam.setVariables(wiiX, wiiY, wiiZ, razorYaw, razorPitch, -razorRoll );
        cam.begin();
        ofTranslate(0,0,-1000);
        ofRotateX(-90);
        myMesh->updateMesh(mode3D, frameCounter, smoothPZ, razorYaw, razorPitch, razorRoll, wiiX, wiiY, wiiZ, fWiiX, fWiiY, fWiiZ);  
        cam.end(); 
        
    } else {
        ofTranslate(fullWindowWidth/2,fullWindowHeight/2); 
            myMesh->updateMesh(mode3D, frameCounter, smoothPZ, razorYaw, razorPitch, razorRoll, wiiX, wiiY, wiiZ, fWiiX, fWiiY, fWiiZ);  
    }

   
////    ofScale(1,1,sqrt(float(ofGetWindowWidth()/2)/mouseX));
    


    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    myMesh->timeControl();

    
    switch (key) {
		case OF_KEY_UP: kTilt += 1; if (kTilt > 30) kTilt = 30; kinect.setCameraTiltAngle(kTilt); break;
        case OF_KEY_DOWN: kTilt -= 1; if (kTilt < -30) kTilt = -30; kinect.setCameraTiltAngle(kTilt); break;
        
            
        case 'q': razorYaw+=5; break;
        case 'a': razorYaw-=5; break;
        case 'w': razorPitch+=5; break;
        case 's': razorPitch-=5; break;
        case 'e': razorRoll+=5; break;
        case 'd': razorRoll-=5; break;
        case 'r': wiiX+=10; break;
        case 'f': wiiX-=10; break;
        case 't': wiiY+=10; break;
        case 'g': wiiY-=10; break;
        case 'y': wiiZ+=10; break;
        case 'h': wiiZ-=10; break;    
        case '1': if(fWiiX<1) fWiiX+=.01; break;
        case '2': if(fWiiX>0) fWiiX-=.01; break;
        case '3': if(fWiiY<1) fWiiY+=.01; break;
        case '4': if(fWiiY>0) fWiiY-=.01; break;
        case '5': if(fWiiZ<1) fWiiZ+=.01; break;
        case '6': if(fWiiZ>0) fWiiZ-=.01; break;
        case '7': wiiMap+=50; break;
        case '8': wiiMap-=50; break;
        case 'u': smoothPZ+=50; break;
        case 'j': smoothPZ-=50; break;          
        case 'o': frameCounter++; break;
        case 'l': frameCounter = 0; break;
        case '0': razorYaw=0;razorPitch=0;razorRoll=0;wiiX=0;wiiY=0;wiiZ=0; break;




    }
  
}

//--------------------------------------------------------------
void testApp::updateOsc(){
    
    // hide old messages
	for ( int i=0; i<NUM_MSG_STRINGS; i++ )
	{
		if ( timers[i] < ofGetElapsedTimef() )
			msg_strings[i] = "";
	}
    
	// check for waiting messages
	while( receiver.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage( &m );
        razorYaw = m.getArgAsInt32( 0 );
        razorPitch = m.getArgAsInt32( 1 );
        razorRoll = m.getArgAsInt32( 2 );
	}
    printf("razorYaw: %d razorPitch: %d razorRoll: %d wiiX: %d wiiY: %d wiiZ: %d\n", razorYaw, razorPitch, razorRoll, wiiX, wiiY, wiiZ);

    
}

//--------------------------------------------------------------
void testApp::updateOsc2(){
    
    
    
	// check for waiting messages
	while( receiver2.hasWaitingMessages() )
	{
		// get the next message
		ofxOscMessage b;
		receiver2.getNextMessage( &b );
        
		// check for mouse moved message
		if ( b.getAddress() == "/wii/1/ir/0" )
		{
			// both the arguments are int32's
			fWiiX = b.getArgAsFloat( 0 );
//            printf("the address is good for 0\n");
		}
		// check for mouse button message
        else if ( b.getAddress() == "/wii/1/ir/1" )
		{
			// the single argument is a string
			fWiiY = b.getArgAsFloat( 0 ) ;
//            printf("the address is good for 1\n");

		}
        else if (b.getAddress() == "/wii/1/ir/xys/1/2" ){
            fWiiZ = b.getArgAsFloat( 0 ) ;

        }
	}
    
    wiiX = ofMap(fWiiX, 0, 1, -wiiMap, wiiMap);
    wiiY = ofMap(fWiiY, 0, 1, wiiMap, -wiiMap);
    wiiZ = ofMap(fWiiZ, 0, 1, -wiiMap/2, wiiMap/2);

    
    
    

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
