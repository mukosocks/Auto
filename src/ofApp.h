#pragma once

#include "ofMain.h"

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
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
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
    
    int width;
    int height;
    int fixwidth;
    int screenwidth;
    int gapwidth;

    int state;
    
};

// video changer
// Integrating with virtual mode
// integrating with Door Unit