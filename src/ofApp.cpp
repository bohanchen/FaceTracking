#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    /*
    Yellow: 247,197,46
    Blue: 8,26,91
    Red: 183,43,13
    Grey:227 225 228
     */
    
    fftSmooth = new float [8192];
    for (int i = 0; i<8192; ++i) {
        fftSmooth[i] = 0;
    }
    
    bands = 64;
    
    music.load("music.mp3");
    chen.load("chen.mp3");
    
    //Grab the camera at this size
    camWidth = 1280;
    camHeight = 960;
    
    mycam.setDeviceID(0);
    mycam.setDesiredFrameRate(30);
    mycam.initGrabber(camWidth, camHeight);
    
    ofSetVerticalSync(true);
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
    tracker.setup();
    
#ifdef TARGET_OSX

#endif
    ofSetVerticalSync(true);
    cloneReady = false;
    clone.setup(mycam.getWidth(), mycam.getHeight());
    ofFbo::Settings settings;
    settings.width = mycam.getWidth();
    settings.height = mycam.getHeight();
    maskFbo.allocate(settings);
    srcFbo.allocate(settings);
    tracker.setup();
    tracker.setIterations(25);
    tracker.setAttempts(4);
    
    //Allow jpg and png files as input
    faces.allowExt("jpg");
    faces.allowExt("png");
    faces.listDir("faces");
    currentFace = 0;
    if(faces.size()!=0){
        loadFace(faces.getPath(currentFace));
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    //background color
    ofBackground(253, 255, 252);
    
    float * value = ofSoundGetSpectrum(bands);
    
    for (int i = 0; i<bands; i++) {
        fftSmooth[i] *=0.9f;
        
        if (fftSmooth[i]<value[i]){
            fftSmooth[i] = value[i];
        }
        
    }
    
    //update camera frame
    mycam.update();
    
    //Tracking and Clone
    if(mycam.isFrameNew()) {
        tracker.update(toCv(mycam));
        position = tracker.getPosition();
        scale = tracker.getScale();
        orientation = tracker.getOrientation();
        rotationMatrix = tracker.getRotationMatrix();
        
        cloneReady = tracker.getFound();
        
        if(cloneReady) {
            ofMesh camMesh = tracker.getImageMesh();
            camMesh.clearTexCoords();
            camMesh.addTexCoords(srcPoints);
            
            maskFbo.begin();
            ofClear(0, 255);
            camMesh.draw();
            maskFbo.end();
            
            srcFbo.begin();
            ofClear(0, 255);
            src.bind();
            camMesh.draw();
            
            src.unbind();
            srcFbo.end();
            clone.setStrength(16);
            clone.update(srcFbo.getTextureReference(),
                         mycam.getTextureReference(),
                         maskFbo.getTextureReference());
        }//cloneReady 'if'
    }//mycam 'if'
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    ofSetLineWidth(2);
//    Jack.play();
    
    
    int Jaw =tracker.getGesture(ofxFaceTracker::JAW_OPENNESS);
    
    //try to use the eye openness but the accuracy is not good enough
    int left_eyeOpen =tracker.getGesture(ofxFaceTracker::LEFT_EYE_OPENNESS);
    int right_eyeOpen =tracker.getGesture(ofxFaceTracker::RIGHT_EYE_OPENNESS);
    
     //cout<<Jaw<<endl;
    cout<<left_eyeOpen<<endl;

    //Assign values for eye, nose, mouse, and face
    ofPolyline lefteye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
    ofPolyline righteye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
    
    ofPolyline nose = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE);
    
    ofPolyline left_eyebrow = tracker.getImageFeature(ofxFaceTracker::LEFT_EYEBROW);
    ofPolyline right_eyebrow = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYEBROW);
    
    ofPolyline mouse = tracker.getImageFeature(ofxFaceTracker::INNER_MOUTH);
    
    ofPolyline face = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    
    for (int i = 0; i < bands; ++i) {
        ofSetColor(247,197,46);//yellow
        ofDrawCircle(lefteye.getCentroid2D(),-(fftSmooth[i]*120));
        ofDrawRectangle(right_eyebrow.getCentroid2D(),-(fftSmooth[i]*200),-(fftSmooth[i]*50));
        
        ofSetColor(183,43,13);//red
        ofDrawCircle(righteye.getCentroid2D(),-(fftSmooth[i]*120));
        ofDrawRectangle(left_eyebrow.getCentroid2D(),-(fftSmooth[i]*200),-(fftSmooth[i]*50));
        
        ofSetColor(8,26,91);//blue
        ofDrawRectangle(nose.getCentroid2D(), -(fftSmooth[i]*50),-(fftSmooth[i]*100));
        ofDrawCircle(mouse.getCentroid2D(), -(fftSmooth[i]*50));
        
//        ofSetColor(227,225,228 );
//        ofFill();
//        face.draw();
    }
    
    ofSetColor(255,255,255);
    if ((Jaw<22)) {
        //Play music when mouse is open
        music.play();
        
        //Draw the face Sub
        if(src.getWidth() > 0 && cloneReady) {
            clone.draw(0, 0);
        } else {
            mycam.draw(0, 0);
        }
        if(!tracker.getFound()) {//error messages
            ofDrawBitmapString("camera face not found", 10, 10);
        }
        if(src.getWidth() == 0) {//Write instructions
            ofSetColor(183,43,13);
            ofDrawRectangle(10, 30, 250, 40);
            ofSetColor(247,197,46);
            ofDrawRectangle(10, 70, 250, 40);
            ofSetColor(255, 255, 255);
            ofDrawBitmapString("  Option 1: Drag images", 10, 50);
            ofDrawBitmapString("  Option 2: Open Mouth", 10, 90);
            
        } else if(!tracker.getFound()) {//error messages
            ofDrawBitmapString("image face not found", 10, 30);
        }
    }

    
    
    
    
    
    


}
//--------------------------------------------------------------
void ofApp::loadFace(string face){
    
    //Face sub function
    src.loadImage(face);
    if(src.getWidth() > 0) {
        tracker.update(toCv(src));
        srcPoints = tracker.getImagePoints();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'r') {
        tracker.reset();
    }
    
    switch(key){
        case OF_KEY_UP:
            currentFace++;
            break;
        case OF_KEY_DOWN:
            currentFace--;
            break;
    }
    currentFace = ofClamp(currentFace,
                          0,
                          faces.size());
    if(faces.size()!=0){
        loadFace(faces.getPath(currentFace));
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    loadFace(dragInfo.files[0]);
}
