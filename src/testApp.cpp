#include "testApp.h"
#include "MeshMaker.h"

using namespace std;

// kinect frame width
#define KW 640
// kinect frame height
#define KH 480
// for the average fps calculation
#define FPS_MEAN 30

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
int eyeDistance = 10; //change later to 7.5
int transZ;


ostringstream controlString;     
string controlStringToShow;

float smoothPZ = 460;

float smoothDiffX;
float smoothDiffY;
float smoothTransZ;

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
    ofSetVerticalSync(true);
	glEnable(GL_DEPTH_TEST);
    // enable depth->rgb image calibration
	kinect.setRegistration(true);
    kinect.init();
    kinect.open();
    
    kinect.setDepthClipping(50, 8000);


    
    myMesh = new MeshMaker;
    
    fullWindowWidth = ofGetWindowWidth();
    fullWindowHeight = ofGetWindowHeight();

    
    //above constructor contains default arguments, which do not have to be stated if the default is acceptable 
    
}

//--------------------------------------------------------------
void testApp::update(){
  

    kinect.update();
    
    
    if (kinect.isFrameNew()) {

        myMesh->recordTime( &kinect ) ;   //& gets the pointer for the object           
        }
    
    
}

//--------------------------------------------------------------
void testApp::drawReport() {
    ofPushMatrix();
    ostringstream controlString;     
    controlString << "eye Distance (r & f) " << eyeDistance;    
    controlStringToShow = controlString.str(); 
    ofDrawBitmapString(ofToString(controlStringToShow), 50,50);
    ofPopMatrix();
}
//--------------------------------------------------------------
void testApp::draw(){
    
     
ofTranslate(fullWindowWidth/2,fullWindowHeight/2); 
//    ofPushMatrix();    
    
//    printf("eye distance (r & f) %d\n", eyeDistance);
    
//    drawReport();
    
    
//    ofViewport(0, 0, ofGetWindowWidth(), ofGetWindowHeight()/2); 
//    ofPushMatrix();    
//    ofTranslate(fullWindowWidth/2,fullWindowHeight/2, -PZ); //centers everything
    
    //we basidrawcally need a container for the translation -- translate to the head origin point and then inside, perform the perspective translations 
    
//    int translateZ = mouseX;
//    
//    ofTranslate(0,0,smoothPZ);
//    ofRotateY(smoothDiffX/3*PI);
//    ofRotateX(smoothDiffY/3*PI);
//    ofTranslate(0,0,-smoothPZ); //translates back to the default
    
//        if (kinect.isFrameNew()) {}
    
    
    
//    easyCam.begin();
    
    
ofViewport(0, ofGetWindowHeight()/2, ofGetWindowWidth(), ofGetWindowHeight()/2);  
    myMesh->updateMesh(smoothPZ, smoothDiffX, smoothDiffY, transZ);

ofViewport(0, 0, ofGetWindowWidth(), ofGetWindowHeight()/2);  
    myMesh->updateMesh(smoothPZ, smoothDiffX+eyeDistance, smoothDiffY, transZ);
    
    
//    ofPopMatrix();
//    easyCam.end();
       
    
//    ofPopMatrix();


//    
////    ofScale(1,1,sqrt(float(ofGetWindowWidth()/2)/mouseX));
//    


    
//    if (bDrawCloud) {
////        drawPointCloud();
//    }



    
 /*
  
  
    ofPushMatrix();
    ofTranslate(fullWindowWidth/2,fullWindowHeight/2, -PZ); //centers everything
    ofTranslate(0,0,smoothPZ);
    ofRotateY(smoothDiffX-eyeDistance/3*PI);
    ofRotateX(smoothDiffY/3*PI);
    
    ofTranslate(0,0,-smoothPZ); //translates back to the default
*/    
//    if (bDrawCloud) {
//        drawPointCloud();
//        drawPoses();
//    }

//    easyCam.end();
    
//    ofPopMatrix();


    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    switch (key) {
		case OF_KEY_UP: kTilt += 1; if (kTilt > 30) kTilt = 30; kinect.setCameraTiltAngle(kTilt); break;
        case OF_KEY_DOWN: kTilt -= 1; if (kTilt < -30) kTilt = -30; kinect.setCameraTiltAngle(kTilt); break;

                        
//        case 'x': rotateXvalue+=10; break;
            
//        case 'y': rotateYvalue+=10; break;
            
//        case 'z': rotateZvalue+=10; break;
            
        case 't': smoothPZ+=10; break;
           
        case 'g': smoothPZ-=10; break;
            
        case 'r': smoothDiffX+=10; break;
       
        case 'f': smoothDiffX-=10; break;
            
        case 'y': smoothDiffY+=10; break;
        
        case 'h': smoothDiffY-=10; break;
            
        case 'u': transZ+=10; break;
            
        case 'j': transZ-=10; break;



    }

    myMesh->timeControl();


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
