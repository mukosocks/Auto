#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    state = 2;
    
    // initialize dimensions
    width = 1280;
    height = 657;
    fixwidth = 325;
    screenwidth = 305;
    gapwidth = 6;
    
    // load Movie
    closed.loadMovie("movies/kiyo/closed.mov");
    closing.loadMovie("movies/kiyo/closing.mov");
    opening.loadMovie("movies/kiyo/opening.mov");
    opened.loadMovie("movies/kiyo/opened.mov");
    
    // Set Movie to play and loop
    closed.play();
    closing.play();
    opening.play();
    opened.play();
    
    // initialize pixelbasket
    lfpixelbasket 	= new unsigned char[325*657*3];
    lpixelbasket    = new unsigned char[305*657*3];
    rpixelbasket    = new unsigned char[305*657*3];
    rfpixelbasket    = new unsigned char[325*657*3];
    
    // allocate texture territory
    lfTexture.allocate(325,657, GL_RGBA);
    lTexture.allocate(305,657, GL_RGBA);
    rTexture.allocate(305,657, GL_RGBA);
    rfTexture.allocate(325,657, GL_RGBA);
    
    ofSetVerticalSync(true);
}


//--------------------------------------------------------------
void ofApp::update(){
    
    // update Movie frames
    closed.update();
    closing.update();
    opening.update();
    opened.update();
    
    if (state == 0) {
        
        // when a new frame is available. . .
        if (closed.isFrameNew()){
            
            // ------------------------------------------
            
            // left fix casting
            
            // total number of pixels within territory
            //int totalfixPixels = fixwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels = closed.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = 0; j < fixwidth; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    lfpixelbasket[(i*fixwidth+j)*3] = pixels[(i*1280+j)*3];
                    lfpixelbasket[(i*fixwidth+j)*3+1] = pixels[(i*1280+j)*3+1];
                    lfpixelbasket[(i*fixwidth+j)*3+2] = pixels[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            lfTexture.loadData(lfpixelbasket, fixwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // left screen casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels2 = closed.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth; j < fixwidth + gapwidth + screenwidth; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3] = pixels2[(i*1280+j)*3];
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+1] = pixels2[(i*1280+j)*3+1];
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+2] = pixels2[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            lTexture.loadData(lpixelbasket,screenwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // right screen casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels3 = closed.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth*2 + screenwidth; j < fixwidth + gapwidth*2 + screenwidth*2; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3] = pixels3[(i*1280+j)*3];
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+1] = pixels3[(i*1280+j)*3+1];
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+2] = pixels3[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            rTexture.loadData(rpixelbasket,screenwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // right fix casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels4 = closed.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth*3 + screenwidth*2; j < fixwidth*2 + gapwidth*3 + screenwidth*2; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3] = pixels4[(i*1280+j)*3];
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+1] = pixels4[(i*1280+j)*3+1];
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+2] = pixels4[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            rfTexture.loadData(rfpixelbasket,fixwidth,height, GL_RGB);
        }
        
    }
    
    if (state == 2) {
    
    // when a new frame is available. . .
    if (closing.isFrameNew()){
    
    // ------------------------------------------
    
    // left fix casting
        
        // total number of pixels within territory
        //int totalfixPixels = fixwidth*height*3;
        
        // collect all pixels from the frame
        unsigned char * pixels = closing.getPixels();
        
        // maneuver through the field
        
        for (int i = 0; i < height; i++){ // while stepping down the collumn,
            for (int j = 0; j < fixwidth; j++){ // go through the entirely of each row.
                
                // line up into the basket RGB triplets of picked pixlets
                
                lfpixelbasket[(i*fixwidth+j)*3] = pixels[(i*1280+j)*3];
                lfpixelbasket[(i*fixwidth+j)*3+1] = pixels[(i*1280+j)*3+1];
                lfpixelbasket[(i*fixwidth+j)*3+2] = pixels[(i*1280+j)*3+2];
                
            }
        }
        // pour into the cast.
        lfTexture.loadData(lfpixelbasket, fixwidth,height, GL_RGB);
    
    // ------------------------------------------
    
    // left screen casting
    
    // when a new frame is available. . .
        
        // total number of pixels within territory
        //int totalscreenPixels = screenwidth*height*3;
        
        // collect all pixels from the frame
        unsigned char * pixels2 = closing.getPixels();
        
        // maneuver through the field
        
        for (int i = 0; i < height; i++){ // while stepping down the collumn,
            for (int j = fixwidth + gapwidth; j < fixwidth + gapwidth + screenwidth; j++){ // go through the entirely of each row.
                
                // line up into the basket RGB triplets of picked pixlets
                
                lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3] = pixels2[(i*1280+j)*3];
                lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+1] = pixels2[(i*1280+j)*3+1];
                lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+2] = pixels2[(i*1280+j)*3+2];
                
            }
        }
        // pour into the cast.
        lTexture.loadData(lpixelbasket,screenwidth,height, GL_RGB);
    
    // ------------------------------------------
    
    // right screen casting
    
    // when a new frame is available. . .
        
        // total number of pixels within territory
        //int totalscreenPixels = screenwidth*height*3;
        
        // collect all pixels from the frame
        unsigned char * pixels3 = closing.getPixels();
        
        // maneuver through the field
        
        for (int i = 0; i < height; i++){ // while stepping down the collumn,
            for (int j = fixwidth + gapwidth*2 + screenwidth; j < fixwidth + gapwidth*2 + screenwidth*2; j++){ // go through the entirely of each row.
                
                // line up into the basket RGB triplets of picked pixlets
                
                rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3] = pixels3[(i*1280+j)*3];
                rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+1] = pixels3[(i*1280+j)*3+1];
                rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+2] = pixels3[(i*1280+j)*3+2];
                
            }
        }
        // pour into the cast.
        rTexture.loadData(rpixelbasket,screenwidth,height, GL_RGB);
        
        // ------------------------------------------
        
        // right fix casting
        
        // when a new frame is available. . .
        
        // total number of pixels within territory
        //int totalscreenPixels = screenwidth*height*3;
        
        // collect all pixels from the frame
        unsigned char * pixels4 = closing.getPixels();
        
        // maneuver through the field
        
        for (int i = 0; i < height; i++){ // while stepping down the collumn,
            for (int j = fixwidth + gapwidth*3 + screenwidth*2; j < fixwidth*2 + gapwidth*3 + screenwidth*2; j++){ // go through the entirely of each row.
                
                // line up into the basket RGB triplets of picked pixlets
                
                rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3] = pixels4[(i*1280+j)*3];
                rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+1] = pixels4[(i*1280+j)*3+1];
                rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+2] = pixels4[(i*1280+j)*3+2];
                
                }
            }
            // pour into the cast.
            rfTexture.loadData(rfpixelbasket,fixwidth,height, GL_RGB);
        }
        
    }
    
    if (state == 4) {
        
        // when a new frame is available. . .
        if (opening.isFrameNew()){
            
            // ------------------------------------------
            
            // left fix casting
            
            // total number of pixels within territory
            //int totalfixPixels = fixwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels = opening.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = 0; j < fixwidth; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    lfpixelbasket[(i*fixwidth+j)*3] = pixels[(i*1280+j)*3];
                    lfpixelbasket[(i*fixwidth+j)*3+1] = pixels[(i*1280+j)*3+1];
                    lfpixelbasket[(i*fixwidth+j)*3+2] = pixels[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            lfTexture.loadData(lfpixelbasket, fixwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // left screen casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels2 = opening.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth; j < fixwidth + gapwidth + screenwidth; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3] = pixels2[(i*1280+j)*3];
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+1] = pixels2[(i*1280+j)*3+1];
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+2] = pixels2[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            lTexture.loadData(lpixelbasket,screenwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // right screen casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels3 = opening.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth*2 + screenwidth; j < fixwidth + gapwidth*2 + screenwidth*2; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3] = pixels3[(i*1280+j)*3];
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+1] = pixels3[(i*1280+j)*3+1];
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+2] = pixels3[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            rTexture.loadData(rpixelbasket,screenwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // right fix casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels4 = opening.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth*3 + screenwidth*2; j < fixwidth*2 + gapwidth*3 + screenwidth*2; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3] = pixels4[(i*1280+j)*3];
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+1] = pixels4[(i*1280+j)*3+1];
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+2] = pixels4[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            rfTexture.loadData(rfpixelbasket,fixwidth,height, GL_RGB);
        }
        
    }
    
    if (state == 6) {
        
        // when a new frame is available. . .
        if (opened.isFrameNew()){
            
            // ------------------------------------------
            
            // left fix casting
            
            // total number of pixels within territory
            //int totalfixPixels = fixwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels = opened.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = 0; j < fixwidth; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    lfpixelbasket[(i*fixwidth+j)*3] = pixels[(i*1280+j)*3];
                    lfpixelbasket[(i*fixwidth+j)*3+1] = pixels[(i*1280+j)*3+1];
                    lfpixelbasket[(i*fixwidth+j)*3+2] = pixels[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            lfTexture.loadData(lfpixelbasket, fixwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // left screen casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels2 = opened.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth; j < fixwidth + gapwidth + screenwidth; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3] = pixels2[(i*1280+j)*3];
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+1] = pixels2[(i*1280+j)*3+1];
                    lpixelbasket[(i*screenwidth+j-(fixwidth+gapwidth))*3+2] = pixels2[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            lTexture.loadData(lpixelbasket,screenwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // right screen casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels3 = opened.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth*2 + screenwidth; j < fixwidth + gapwidth*2 + screenwidth*2; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3] = pixels3[(i*1280+j)*3];
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+1] = pixels3[(i*1280+j)*3+1];
                    rpixelbasket[(i*screenwidth+j-(fixwidth + gapwidth*2 + screenwidth))*3+2] = pixels3[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            rTexture.loadData(rpixelbasket,screenwidth,height, GL_RGB);
            
            // ------------------------------------------
            
            // right fix casting
            
            // when a new frame is available. . .
            
            // total number of pixels within territory
            //int totalscreenPixels = screenwidth*height*3;
            
            // collect all pixels from the frame
            unsigned char * pixels4 = opened.getPixels();
            
            // maneuver through the field
            
            for (int i = 0; i < height; i++){ // while stepping down the collumn,
                for (int j = fixwidth + gapwidth*3 + screenwidth*2; j < fixwidth*2 + gapwidth*3 + screenwidth*2; j++){ // go through the entirely of each row.
                    
                    // line up into the basket RGB triplets of picked pixlets
                    
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3] = pixels4[(i*1280+j)*3];
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+1] = pixels4[(i*1280+j)*3+1];
                    rfpixelbasket[(i*fixwidth+j-(fixwidth + gapwidth*3 + screenwidth*2))*3+2] = pixels4[(i*1280+j)*3+2];
                    
                }
            }
            // pour into the cast.
            rfTexture.loadData(rfpixelbasket,fixwidth,height, GL_RGB);
        }
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // placement of the completed moulds.
    
    lfTexture.draw(0,0);
    lTexture.draw(fixwidth + gapwidth,0);
    rTexture.draw(fixwidth + gapwidth*2 + screenwidth,0);
    rfTexture.draw(fixwidth + gapwidth*3 + screenwidth*2,0);
    
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    if (key == '0') {
        
        state = 0;
    
    }
    
    if (key == '2') {
        
        state = 2;
        
    }
    
    if (key == '4') {
        
        state = 4;
        
    }
    
    if (key == '6') {
        
        state = 6;
        
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
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
