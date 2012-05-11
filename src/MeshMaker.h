#pragma once

//#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxGameCamera.h"
//#include "time.h"

    
class MeshMaker {    
        
public:
    
    MeshMaker();
    
    void recordTime( ofxKinect* kinect );
    void updateMesh( bool _mode3D, int _frameCounter, int _smoothPZ, int _razorYaw, int _razorPitch, int _razorRoll, int _wiiX, int _wiiY, int _wiiZ, float _fWiiX, float _fWiiY, float _fWiiZ );
    void setupMesh();
    void exit();
    void timeControl();
    void gridMaker();
    
    ofImage presentImg;
    ofImage pastImg;
    ofMesh mesh; 
    ofxGameCamera cam;

    bool mode3D;


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
    
    bool showGrid;
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
    int recordIntervalChange;
    
    //------demoModes---------------
    bool playbackMode;
    bool liveRecordMode;
    bool stepThroughMode;

};
