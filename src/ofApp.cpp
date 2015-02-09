#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    receiver.setup(GETPORT);                        // setup Osc Receiver
    sender.setup(HOST, SENDPORT);                   // setup Osc Sender
    
    ofSetFrameRate(30);
    
    // initialize dimensions
    width = 1280;
    height = width/1.948;
    fixwidth = height/2.021;
    screenwidth = height/2.154;
    gapwidth = 1280*0.005;
    
    title = "kagari";
    // enable to test variations
    
    // load Movie
    closed.loadMovie("content/" + title + "/closed.mov");
    closing.loadMovie("content/" + title + "/closing.mov");
    opening.loadMovie("content/" + title + "/opening.mov");
    opened.loadMovie("content/" + title + "/opened.mov");
    
    // Set Movie to play and loop
    closed.play();
    closing.play();
    opening.play();
    opened.play();
    
    // initialize pixelbasket
    lfpixelbasket 	= new unsigned char[fixwidth*height*3];
    lpixelbasket    = new unsigned char[screenwidth*height*3];
    rpixelbasket    = new unsigned char[screenwidth*height*3];
    rfpixelbasket    = new unsigned char[fixwidth*height*3];
    
    // allocate texture territory
    lfTexture.allocate(fixwidth,height, GL_RGB);
    lTexture.allocate(screenwidth,height, GL_RGB);
    rTexture.allocate(screenwidth,height, GL_RGB);
    rfTexture.allocate(fixwidth,height, GL_RGB);
    
    ofSetVerticalSync(true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Receive Values
    
    // call Osc receiver functions
    while (receiver.hasWaitingMessages()) {
        
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        
        if (m.getAddress() == "/lan10") {
            
            // collect Door readings
            ofst4 = m.getArgAsInt32(0); // from processing: value_pos
            ofst5 = m.getArgAsInt32(1); // from processing: value_posbyte
            ofst6 = m.getArgAsInt32(2); // from processing: value_state
            
            // collect Sensor readings
            ofst8 = m.getArgAsInt32(3); // from processing: sensor_1
            ofst9 = m.getArgAsInt32(4); // from processing: sensor_2
            ofst10 = m.getArgAsInt32(5);// from processing: sensor_3
            ofst11 = m.getArgAsInt32(6);// from processing: sensor_4
            ofst12 = m.getArgAsInt32(7);// from processing: sensor_5
            ofst13 = m.getArgAsInt32(8);// from processing: sensor_6
            
        }
        
    }
    
    // Distribute Values
    
    rn60.setPosition(ofst4,ofst5);
    rn60.setState(ofst6);
    
    is6000.setSensorGrid(ofst8, ofst9, ofst10, ofst11, ofst12, ofst13);
    is6000.setPresence();
    
    doorposition = rn60.getPosition();
    doorstate = rn60.getState();
    presence = is6000.getPresence();
    
    // Frame Control
    
    // if State Change detected . . .
    
    if (doorstate != lastdoorstate) {
        
        // initialize video frames  // create fuction to calculate initial frame by pass door position
        
        tempstate = doorstate;
        temppct = ((doorposition*100)/maxgap);
        
        //cout << "gap percentage: " << temppct;
        
        switch (tempstate) {
                
            case 0: // fully closed
                tempframe = (closed.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                closed.setFrame(tempframe); // tempframe: frame at position, 0: beginning of video
                closed.setFrame(tempframe);
                break;

            case 2: // closing
                tempframe = (closing.getTotalNumFrames() * (100 - temppct)) / 100;
                //cout << " start frame: " << tempframe << endl;
                closing.setFrame(tempframe);
                closing.setFrame(tempframe);
                break;

            case 4: // opening
                tempframe = (opening.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                opening.setFrame(tempframe);
                opening.setFrame(tempframe);
                break;

            case 6: // fully opened
                tempframe = (opened.getTotalNumFrames() * temppct) / 100;
                //cout << " start frame: " << tempframe << endl;
                opened.setFrame(tempframe);
                opened.setFrame(tempframe);
                break;
            default:
                break;
        }
        
    } else {
        
        doorstatechanged = false;
        
    }
    
    temppct = NULL;
    lastdoorstate = doorstate;
    
    // Image Processing
    
    // update Movie frames
    closed.update();
    closing.update();
    opening.update();
    opened.update();
    
    if (doorstate == 0) {
        
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
    
    if (doorstate == 2) {
        
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
    
    if (doorstate == 4) {
        
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
    
    if (doorstate == 6) {
        
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
    
    // Display Images
    
    // placement of the completed moulds.
    
    lfTexture.draw(0,0);

    rfTexture.draw(fixwidth + gapwidth*3 + screenwidth*2,0);
    
    lTexture.draw((fixwidth + gapwidth)-doorposition,0);
    rTexture.draw((fixwidth + gapwidth*2 + screenwidth) + doorposition,0);
    
    
    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
    
    // Debugging
    
    // door data check
    
    //cout << doorposition << endl;
    //cout << doorstate << endl;
    //cout << presence << endl;
    
    // dimension value check
    
    //cout << width << endl;
    //cout << height << endl;
    //cout << fixwidth << endl;
    //cout << screenwidth << endl;
    //cout << gapwidth << endl;

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
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

//--------------------------------------------------------------

void ofApp::exit(){

}

