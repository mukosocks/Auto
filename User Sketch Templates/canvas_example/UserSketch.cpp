#include "UserSketch.h"

UserSketch::UserSketch() {
    
}

//--------------------------------------------------------------

void UserSketch::setup() {
    
    //Setup Screen Dimensions (width, ratio w:h, door-projection speed calib, monitor mode)
    screen = new Screen(640, 1.33, 0.625, false);
    
}

//--------------------------------------------------------------
void UserSketch::update() {
    
    
}

//--------------------------------------------------------------
void UserSketch::draw() {
    
    // User inputs Custom Codes Here.
    
    ofBackground(0);
    
    
    for (int i = 0; i < 200; i = i + 10) {
        
        ofSetColor(i,i,doorposition);
        ofRect(screenpos_r + 100 , i*3,i,i);
        
    }
    
    for (int i = 0; i < 100; i = i + 10) {
        
        ofSetColor(doorposition,110,i);
        ofCircle((screenpos_l + i*3),600,i);
        
    }
    
    ofSetColor(255,doorposition,110);
    ofTriangle((screenpos_l + screenwidth),10,10,40,screenpos_l,(screenheight/2));
    
}