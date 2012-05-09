#pragma once

//#include "ofMain.h"
#include "ofxKinect.h"
//#include "time.h"

//1 for recording new frames, 0 for not 
#define RECORDTIMEFRAMES 0

    
class MeshMaker {    
        
public:
    
    MeshMaker();
    
    void recordTime( ofxKinect* kinect );
    void updateMesh( int _frameCounter, int _smoothPZ, int _razorYaw, int _razorPitch, int _razorRoll, int _wiiX, int _wiiY, int _wiiZ, float _fWiiX, float _fWiiY, float _fWiiZ );
    void setupMesh();
    void exit();
    void timeControl();
    

	
//    ofxKinect *kinect;
    ofImage presentImg;
    ofImage pastImg;
    ofMesh mesh;
    ofCamera cam;
    
    //---------variables to pass in to control the mesh---
    
    int initialZtranslation;
    int smoothPZ;
    int razorYaw;
    int razorPitch;
    int razorRoll;
    int wiiX;
    int wiiY;
    int wiiZ;
    
    float fWiiX, fWiiY, fWiiZ;
    
//    int smoothDiffX;
//    int smoothDiffY;
//    int transZ;
    
    
    int lastTime;
    int recordInterval;
    int currentTime;
    int  numberOfFramesToRecord;
    bool recordingOn;
    bool kinectDisplayEnabled;
    int mostRecentFrame;  //will make the first frame recorded "2"
    ostringstream fileNameToSave;
    int picX; //short for pixelIndexCounterX
    int picY; //short for pixelIndexCounterY
    int numberOfFramesRecorded;
    bool recordReady;
    
    //---------showing the present or past-----------
    
    int frameCounter;    
    int timeOffsetFrames;
    int frameToShow;
    int previousFrame;
    string frameResult;
    ostringstream fileNameToLoad;
    int skip;	
    int width;
    int height;
    int startY;
    int startX;
    int endBufferY;
    int endBufferX;



};
