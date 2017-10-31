#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxCvHaarFinder.h"
#include "images.hpp"
#include "Clone.h"
using namespace ofxCv;
using namespace cv;

#include "ofxFaceTracker.h"


class ofApp : public ofBaseApp{

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void loadFace(string face);

    int camWidth, camHeight;
    
    //normal cam
    ofVideoGrabber mycam;
    
    ofxFaceTracker tracker;
    ofVec2f position;
    float scale;
    ofVec3f orientation;
    ofMatrix4x4 rotationMatrix;
    Mat translation, rotation;
    ofMatrix4x4 pose;
    
    //face subsitutation
    ofImage src;
    vector<ofVec2f> srcPoints;
    bool cloneReady;
    Clone clone;
    ofFbo srcFbo, maskFbo;
    
    ofDirectory faces;
    int currentFace;
    
    //load sound
    ofSoundPlayer music;
    
    float * fftSmooth;
    int bands;
    
    
    
};
