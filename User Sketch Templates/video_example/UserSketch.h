#ifndef __platforms__UserSketch__
#define __platforms__UserSketch__

#pragma once

#include "ofMain.h"
#include "Door.h"
#include "Sensor.h"
#include "Screen.h"

class UserSketch {
    
public:
    
    UserSketch();
    
    void setup();
    void update();
    void draw();
    
    Door door;
    Screen *screen;
    
    // Screen Related Values
    
    float width;        // width of projection display
    float height;       // height of projection display
    int screenwidth;    // width of individual door-screen
    int screenheight;   // height of individual door-screen
    float screenpos_l;  // left screen position value (x-coordinate)
    float screenpos_r;  // right screen position value (x-coordinate)
    float fixpos_l;     // left fix position value (x-coordinate)
    float fixpos_r;     // right fix position value (x-coordinate)
    
    // Door Related Values
    
    int doorstate;      // what is the door doing now ?
    int doorposition;   // how much has the door opened ?
    int maxgap;         // how much can the door open ?
    
    // Sensor Related Values
    
    int presence;       // is anyone around ?
    
    // Video Manipulation
    
    string content;
    ofVideoPlayer l0;
    ofVideoPlayer r0;
    ofVideoPlayer l1;
    ofVideoPlayer r1;
    ofVideoPlayer l2;
    ofVideoPlayer r2;
    ofVideoPlayer l3;
    ofVideoPlayer r3;
    ofVideoPlayer l4;
    ofVideoPlayer r4;
    ofVideoPlayer l5;
    ofVideoPlayer r5;
    ofVideoPlayer l6;
    ofVideoPlayer r6;
    ofVideoPlayer lf;
    ofVideoPlayer rf;
    
    bool doorstatechanged;
    int lastdoorstate;
    int tempstate;
    int tempframe;
    float temppct = 0.0;
    
private:
    
};

#endif /* defined(__platforms__UserSketch__) */
