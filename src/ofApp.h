#pragma once

#include "ofMain.h"         // for OF to run
#include "ofxOsc.h"         // for Sending + Receiving through Osc
#include "Door.h"           // for manipulating Door values
#include "Sensor.h"         // for manipulating Sensor values

#define HOST "localhost"
#define GETPORT 8000        // my PORT Number
#define SENDPORT 12345      // processing PORT Number

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();

private:
    
    // Class Declarations
    
    Door rn60;
    Sensor is6000;
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    // Image Processing variables
    
    ofVideoPlayer closed;
    ofVideoPlayer closing;
    ofVideoPlayer opening;
    ofVideoPlayer opened;
    
    unsigned char * 	lfpixelbasket;
    unsigned char * 	lpixelbasket;
    unsigned char * 	rpixelbasket;
    unsigned char * 	rfpixelbasket;
    
    ofTexture			lfTexture;
    ofTexture           lTexture;
    ofTexture           rTexture;
    ofTexture           rfTexture;
    
    ofFbo projection;
    
    // Dimension variables
    
    int width;
    int height;
    int fixwidth;
    int screenwidth;
    int gapwidth;
    int totalwidth;
    int totalheight;
    int totalheight2;
    
    // Door Readings
    int ofst4;
    int ofst5;
    int ofst6;
    
    // Sensor Readings
    int ofst8;
    int ofst9;
    int ofst10;
    int ofst11;
    int ofst12;
    int ofst13;
    
    // usable values
    int doorposition;
    int doorstate;
    int presence;
    
    // Content Control variables
    
    string title;
    int maxgap;
    int tempstate;
    int tempframe;
    int temppct;
    int lastdoorstate;
    int interruptpos;
    int interruptpct;
    bool interrupt;
    bool doorstatechanged;
    
    
    // Boot States
    
    bool startup = false;
    bool startupcheck = false;
    bool startupchecked = false;
    bool receiving = false;
    
    // Value Monitoring Strings
    
    string doorstate_str;
    string presence_str;
    string interrupt_str;
    int interruptswitchr;
    int interruptswitchg;
    
    
};
