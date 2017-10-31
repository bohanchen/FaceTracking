//
//  images.cpp
//  Project2
//
//  Created by Bohan Chen  on 10/24/17.
//

#include "images.hpp"
Images::Images(){
    Images(ofColor::white,
           ofVec2f(1,1),
           ofVec2f(ofGetWidth()/2, ofGetHeight()/2)
       );
}

Images::Images(ofColor c,
               ofVec2f s,
               ofVec2f p){
    
    color = c;
    scale = s;
    position = p;

}

void Images::draw(){
    ofPushMatrix();
    ofTranslate(position.x, position.y);
    ofScale(scale.x, scale.y);
    ofSetColor(color);
    draw();
    ofPopMatrix();
}
