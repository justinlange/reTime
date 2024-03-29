//
//  MeshMaker.cpp
//

#include <iostream>
#include "MeshMaker.h"
#include "math.h"
#include <string>
#include <sstream>
#include <iostream> 
 

using namespace std; 

//---------creating a mesh out of the present or past-----------

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	mesh.addVertex(a);
	mesh.addVertex(b);
	mesh.addVertex(c);
}

void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c) {
	mesh.addTexCoord(a);
	mesh.addTexCoord(b);
	mesh.addTexCoord(c);
}

void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d){
	addTexCoords(mesh, a, b, c);
	addTexCoords(mesh, a, c, d);
}

ofVec3f getVertexFromImg(ofImage& pastImg, int x, int y) {   
	ofColor color = pastImg.getColor(x, y);
	if(color.a > 0) {
		float z = ofMap(color.a, 0, 255, -480, 480);
        // this maps this to -480, 480
		return ofVec3f(x - pastImg.getWidth() / 2, y - pastImg.getHeight() / 2, z);
	} else {
		return ofVec3f(0, 0, 0);
	}
}



MeshMaker::MeshMaker() {
    
    //---------recording the present-----------
        
    numberOfFramesToRecord = 1000;
    recordingOn = false;
    kinectDisplayEnabled = false;
    mostRecentFrame = 2;  //will make the first frame recorded "2"
    string path = "/volumes/Untitled/"; 
    lastTime = 0;
    recordInterval = 300;  //sets the time between recording and displaying frames, mostly limited by speed
    currentTime = 0; 
    picX=0; //short for pixelIndexCounterX
    picY=0; //short for pixelIndexCounterY
    numberOfFramesRecorded = 0;
    recordReady = false;
    
    //---------showing the present or past-----------
    
    currentTime = ofGetElapsedTimeMillis();
    timeOffsetFrames = 0;
    frameToShow = 2;
    previousFrame = 0;
    startY = 0;
    startX = 0;
    endBufferY = 0;
    endBufferX = 0;
    initialZtranslation = -500;
    showGrid = true;
    recordIntervalChange = 50;
    
    presentImg.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    pastImg.allocate(640, 480, OF_IMAGE_COLOR_ALPHA);
    ofSetVerticalSync(true);
    pastImg.loadImage("1.png"); //this means we'll always have to have one image to start!
    mesh.setMode(OF_PRIMITIVE_TRIANGLES); //rather than points
    skip = 2;	
	width = pastImg.getWidth();
	height = pastImg.getHeight();
	ofVec3f zero(0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    
    ofxGameCamera cam;
    
    
    //-------only ONE of these modes should be set to true!----//
    
    bool playbackMode = true;
    bool liveRecordMode = false;
    bool stepThroughMode = false;


    
}



void MeshMaker::updateMesh(bool _mode3D, int _frameCounter, int _smoothPZ, int _razorYaw, int _razorPitch, int _razorRoll, int _wiiX, int _wiiY, int _wiiZ, float _fWiiX, float _fWiiY, float _fWiiZ) {
    
    currentTime = ofGetElapsedTimeMillis();

    mode3D = _mode3D;
    frameCounter = _frameCounter;
    razorYaw = _razorYaw;
    razorPitch = _razorPitch;
    razorRoll = _razorRoll;
        
    if (stepThroughMode == true) {
        frameToShow = frameCounter;
    }
    
    if (liveRecordMode == true){
        frameToShow = numberOfFramesRecorded - timeOffsetFrames;
    }

    if (playbackMode == true){
        if(currentTime > (lastTime + recordInterval)) {
            printf("we are increasing frame to show");
            frameToShow=frameToShow+1;
            lastTime = currentTime;
        }
    }


    
    
    mesh.clear();
    ostringstream fileNameToLoad;     
    fileNameToLoad << "/Volumes/Untitled/" << frameToShow << ".png";     
    frameResult = fileNameToLoad.str(); 
    pastImg.loadImage(ofToString(frameResult));
    
    
    for(int y = startY; y < height - endBufferY - skip; y += skip) {        
        for(int x = startX; x < width - endBufferX - skip; x += skip) {
            
            /* 
             this is kind of like quadrants
             ofVec3f short for oF vector w/ 3 floats
             vector in c++ could be (1) coming from stl or (2)
             math -- a direction -- in either 2D or 3D space...
             */
            
            ofVec2f nwi (x,y);
            ofVec2f nei (x+skip, y);
            ofVec2f sei (x+skip, y+skip);
            ofVec2f swi (x, y+skip);
            
            ofVec3f nw = getVertexFromImg(pastImg, x, y);
            ofVec3f ne = getVertexFromImg(pastImg, x + skip, y);
            ofVec3f sw = getVertexFromImg(pastImg, x, y + skip);
            ofVec3f se = getVertexFromImg(pastImg, x + skip, y + skip);
            
            /*
             check for bad data i.e. making sure that nothing 
             is zero, otherwise vertices point to front of screen
             */
            
            if(nw != 0 && ne != 0 && sw != 0 && se != 0) {                 
                addTexCoords(mesh, nwi, nei, sei, swi);
                addFace(mesh, nw, ne, se, sw);                  
            }
        }
    }
    
    if (mode3D == true){
        ofTranslate(ofGetWindowWidth()/2,ofGetWindowHeight()/2); 
        cam.setVariables(wiiX, wiiY, wiiZ, razorYaw, razorPitch, -razorRoll );
        cam.begin();
        ofViewport(0, 0, ofGetWindowWidth(), ofGetWindowHeight()/2); 
        if (showGrid == true) gridMaker();
        ofTranslate(0,0,-1000);
        ofRotateX(-90);
        pastImg.bind();
        mesh.draw();  //
        pastImg.unbind();
        cam.end();
        cam.setVariables(wiiX, wiiY, wiiZ, razorYaw-9, razorPitch, -razorRoll );
        cam.begin();
        ofViewport(0, ofGetWindowHeight()/2, ofGetWindowWidth(), ofGetWindowHeight()/2); 
        if (showGrid == true) gridMaker();
        ofTranslate(0,0,-1000);
        ofRotateX(-90);
        pastImg.bind();
        mesh.draw();  //
        pastImg.unbind();
        cam.end();


    }

    
    if (mode3D == false){
       
        if (showGrid == true) { gridMaker(); }

        pastImg.bind();
        mesh.draw();  //
        pastImg.unbind();
    
    }
}  
        

void MeshMaker::timeControl(){
    
        if(ofGetKeyPressed('v')){
            showGrid = false;
        }
    
        if(ofGetKeyPressed('n')){
            recordInterval+=recordIntervalChange;
        }
        
        if(ofGetKeyPressed('m')){
            if(recordInterval>recordIntervalChange){
                recordInterval-=recordIntervalChange;
            }
        }

    
        if(ofGetKeyPressed('x')){
            printf("frameoffset %d\n", timeOffsetFrames);
            if(timeOffsetFrames < numberOfFramesRecorded){
                timeOffsetFrames = numberOfFramesRecorded;
            }else{
                timeOffsetFrames++;
            }
        }
        if(ofGetKeyPressed('c')){
            printf("frameoffset %d\n", timeOffsetFrames);

            if (timeOffsetFrames > 1){
                timeOffsetFrames--;
            }else{
                timeOffsetFrames = 1;
            }
        }
    
    if(ofGetKeyPressed('b')){
        printf("mostRecentFrame: %d numberOfFramesToRecord: %d frameToShow: %d numberOfFramesRecorded:  %d currentTime: %d lastTime: %d recordInterval %d  recordIntervalChange %d \n",mostRecentFrame,numberOfFramesToRecord, frameToShow, numberOfFramesRecorded, currentTime, lastTime, recordInterval, recordIntervalChange);
    }
    
    if(ofGetKeyPressed('z')) {       //reset everything

        
        printf("we are resetting the variables");
        string path = "/volumes/Untitled/"; 
        mostRecentFrame = 2;  //will make the first frame recorded "2"
        numberOfFramesToRecord = 1000;
        recordingOn = false;
        lastTime = 0;
        recordInterval = 300;
        currentTime = 0; 
        picX=0; //short for pixelIndexCounterX
        picY=0; //short for pixelIndexCounterY
        numberOfFramesRecorded = 0;
        recordReady = false;
        timeOffsetFrames = 0;
        frameToShow = 1;
        previousFrame = 0;
        frameCounter = 0;
        showGrid = true;
        currentTime = ofGetElapsedTimeMillis();
        playbackMode = true;
        
    
  } 
}

void MeshMaker::gridMaker() {
    ofSetColor(255,0,0);
    ofLine(0, 0, 0, 1000, 0, 0);
    ofSetColor(0,255,0);
    ofLine(0,0,0,0,1000,0);
    ofSetColor(0,0,255);
    ofLine(0,0,0,0,0,1000);
    ofSetColor(255,255,255);

}
    
    




void MeshMaker::recordTime ( ofxKinect* kinect ) {


    
    currentTime = ofGetElapsedTimeMillis();
        
    if(ofGetKeyPressed(' ')) {
       recordingOn = true;
        printf("recordingOn has been set to true");
    }
    
        
    if (recordingOn == true){
        if(currentTime > (lastTime + recordInterval)) {
            lastTime = currentTime;

            if (mostRecentFrame < numberOfFramesToRecord){
                mostRecentFrame = mostRecentFrame + 1;

                ofPixels& depthPixels = kinect->getDepthPixelsRef();
                ofPixels& colorPixels = kinect->getPixelsRef();

                    
for(int y = 0; y < 480; y++) {
    for(int x = 0; x < 640; x++) {
        ofColor color = colorPixels.getColor(x, y);
        ofColor depth = depthPixels.getColor(x, y);
        presentImg.setColor(x, y, ofColor(color, depth.getBrightness()));
                }
            }
                    ostringstream fileNameToSave;
                    fileNameToSave << "/Volumes/Untitled/" << mostRecentFrame << ".png";
                    string result = fileNameToSave.str();
                    presentImg.saveImage(result);
                    numberOfFramesRecorded++;
                }
            }
        }         
    }    


        //    ofPushMatrix();
        //    ofScale(1, 1, 1); // "make y point down" I still don't understand what this means
        //    ofTranslate(0,0, initialZtranslation); //centers everything
        //    ofTranslate(wiiX,0,0);
        //    ofRotateX(razorPitch);
        //    ofTranslate(-wiiX,0,0);
        //    ofTranslate(0,wiiY,0);
        //    ofRotateY(razorYaw);
        //    ofTranslate(0,-wiiY,0);
        //    ofTranslate(0,0,wiiZ);
        //    ofRotateZ(razorRoll);
        //    ofTranslate(0,0,-wiiZ);
        
        
        
        //create a vector from the Translates
        //positionVector = ofVec3f(wiiX,wiiY,wiiZ);
        //normalize this vector positionVector = positionVector.normalize;
        
        
        
        /*
         create a vector from wiiX, wiiY, wiiZ (my position)
         
         */
        
        //    ofVec3f pVec = ofVec3f(fWiiX,fWiiY,fWiiZ);
        //    pVec.normalize();
        //    ofVec3f vVec = ofVec3f(razorYaw, razorPitch, razorRoll);
        //    ofPushMatrix();
        
        //    ofTranslate(wiiX,wiiY,wiiZ);
        //    ofRotate(razorYaw, wiiX,wiiY,wiiZ );
        //    ofRotate(razorPitch, wiiX,wiiY,wiiZ );
        //    ofRotate(razorRoll, pVec.z, pVec.x, pVec.y );
        
        
        
        //    ofRotateY(smoothDiffX/3*PI);
        //    ofRotateX(smoothDiffY/3*PI);   
        //    ofTranslate(0,0,smoothPZ); //translates back to the default
        
        
        //    ofRectangle viewport = ofRectangle(0, 0, 1024,768);
        
        //who, what, at what position   who/direction/vector
        
        
        //second vector needs to be normalized roll
        
        //lookAtX = wiiX 
        //lookAtY
        
        //    float lookAtZ = wiiZ+(defaultZoom*(sin((razorPitch*PI)/180)));
        
        //    float lookAtX = 0+(defaultZoom*(sin(razorYaw)));
        //    float lookAtY = 0+(defaultZoom*(sin(razorPitch)));
        //    float lookAtZ = 0-(defaultZoom*(1));
        
        /* float lookAtX = 0+(defaultZoom*(sin((razorYaw*PI)/180)));
         float lookAtY = 0+(defaultZoom*(cos((razorYaw*PI)/180)));
         float lookAtZ = 0+(defaultZoom*(tan((razorPitch*PI)/180)));*/
        
        //    printf("wiiX%d  wiiX%d  wiiZ%d  razorYaw%d  razorPitch
        
        //    gluLookAt(wiiX, wiiY, wiiZ, 0, 0, 0, 0, 1, 0);
        //    gluLookAt(wiiX, wiiY, wiiZ, wiiX, wiiY, wiiZ-10, 0, 1, 0);
        
        
        
        //    wiiZ = 0;
        //    
        //    wiiX = sin(ofGetElapsedTimef()) * 25.0;
        //    wiiY = cos(ofGetElapsedTimef()) * 25.0;
        
        
        //    float defaultZoom = .01;
        //    float lookAtX = wiiX+(defaultZoom*(sin((razorYaw*PI)/180)))/10;
        //    float lookAtY = wiiY+(defaultZoom*(cos((razorYaw*PI)/180)))/10;
        //    float lookAtZ = wiiZ+(defaultZoom*(cos((razorPitch*PI)/180)))/10;
        
        //    float lookAtX = sin(razorYaw/10);
        //    float lookAtY = cos(razorYaw/10);
        //    float lookAtZ = sin(razorPitch/10);
        
        //    glMatrixMode(GL_PROJECTION);
        //what if 
        
        //    gluLookAt(wiiX, wiiY, wiiZ, lookAtY, lookAtX, 0, 0, .5, 0);
        
        
        //    smoothPZ = _smoothPZ; 
        //    smoothDiffX = _smoothDiffX;
        //    smoothDiffY = _smoothDiffY;
        //    transZ = _transZ;

        //    wiiX = _wiiX;
        //    wiiY = _wiiY;
        //    wiiZ = _wiiZ;
        //    fWiiX = _fWiiX;
        //    fWiiY = _fWiiY;
        //    fWiiZ = _fWiiZ;
        //printf("smoothPZ: %d  razorYaw: %d  razorPitch: %d  razorRoll %d\n", smoothPZ, razorYaw, razorPitch, razorRoll);
        
        //    cam.cacheMatrices(true);
        //    cam.setNearClip(.01);
        //    cam.setFarClip(1000);
        //    cam.setFov(60);
        


