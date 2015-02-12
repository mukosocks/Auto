#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    receiver.setup(GETPORT);                        // setup Osc Receiver
    sender.setup(HOST, SENDPORT);                   // setup Osc Sender
    
    ofSetFrameRate(30);
    
    // projection dimensions
    
    totalwidth = ofGetScreenWidth();
    totalheight = ofGetScreenHeight();
    totalheight2 = totalwidth/1.948;
    
    // video dimensions
    width = 1280;
    height = width/1.948;
    fixwidth = height/2.021;
    screenwidth = height/2.154;
    gapwidth = 1280*0.005;
    
    cout << "ナブコ: 本日は披露したいこと。" << endl;
    cout << "\n早川さん: ";
    cin >> title;
    
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
    
    // FBO
    projection.allocate(width,height, GL_RGB);
    projection.setAnchorPoint(width/2, height/2);
    
    projection.begin();
    ofClear(255,255,255,0);
    projection.end();
    
    ofSetVerticalSync(true);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Receive Values
    
    // call Osc receiver functions
    while (receiver.hasWaitingMessages()) {
        
        receiving = true;
        
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
    
    // Give values to Door and Sensor Classes
    
    rn60.setPosition(ofst4,ofst5);
    rn60.setState(ofst6);
    
    is6000.setSensorGrid(ofst8, ofst9, ofst10, ofst11, ofst12, ofst13);
    is6000.setPresence();
    
    if (receiving == false) {
        
        cout << "\nナブコ: 恐れ入りますがLA-N10とは接続されてますか。" << endl;
        std::exit(1);
        
    }
    
    // if Values are coming from the Door, initiate Boot Procedure
    
    if (receiving == true && startup == false && startupcheck == false) {
        
        startup = true;
        startupcheck = true;
        
        if (closed.isLoaded() == true && closing.isLoaded() == true && opening.isLoaded() == true && opened.isLoaded() == true) {
            
            cout << "\nナブコ: " << title << "ですね。" << "承知しました! " << endl;
        } else {
            
            cout << "\nナブコ: ちょっとコンテンツを見つけない。" << endl;
            std::exit(1);
        }
        
        cout << "\nナブコ: 一回ドアを開けて、少々お待ちください…" << endl;
        
        ofxOscMessage m;
        m.setAddress("/open");
        m.addIntArg(1);
        sender.sendMessage(m);
        
    }
    
    // if Door is Fully Opened during Boot Procedure
    
    if (startupcheck == true && rn60.getState() == 6) {
        
        rn60.setMaxGap(rn60.getPosition()); // gives max gap value to Door Class
        
        startupchecked = true;
        
        ofxOscMessage m;
        m.setAddress("/close");
        m.addIntArg(2);
        sender.sendMessage(m);
        
    }
    
    // if Max Gap is collected and Door is Closed
    
    if (startupchecked == true && rn60.getState() == 0) {
        
        cout << "\nよし。" << endl;
        startupchecked = false;
        startupcheck = false;
        
    }
    
    if (startupcheck == false && startupchecked == false && startup == true) {
        
        // Distribute Values
        
        doorposition = rn60.getPosition();
        maxgap = rn60.getMaxGap();
        doorstate = rn60.getState();
        presence = is6000.getPresence();
        
        switch (doorstate) {
                
            case 0:
                doorstate_str = "CLOSED";
                break;
                
            case 1:
                doorstate_str = "SLOW CLOSING";
                break;
                
            case 2:
                doorstate_str = "CLOSING";
                break;
                
            case 3:
                doorstate_str = "STOPPED";
                break;
                
            case 4:
                doorstate_str = "OPENING";
                break;
                
            case 5:
                doorstate_str = "SLOW OPENING";
                break;
                
            case 6:
                doorstate_str = "OPENED";
                break;
                
            default:
                doorstate_str = "null";
                break;
                
        }
        
        switch (presence) {
                
            case 0:
                presence_str = "nobody.";
                break;
                
            case 1:
                presence_str = "someone is HERE!";
                break;
                
            case 2:
                presence_str = "someone is THERE!";
                break;
                
            default:
                presence_str = "null";
                break;
                
        }
        
        if (interrupt == true) {
            
            interruptswitchr = 160;
            interruptswitchg = 230;
            
        } else {
            
            interruptswitchr = 0;
            interruptswitchg = 0;
            
        }
        
        // Frame Control
        
        // if State Change detected . . .
        
        if (doorstate != lastdoorstate) {
            
            if (doorstate == 4 && lastdoorstate == 2) {
                
                interruptpos = doorposition;
                interruptpct = ((interruptpos*100)/maxgap);
                interrupt = true;
                
            }
            
            // initialize video frames  // create fuction to calculate initial frame by pass door position
            
            tempstate = doorstate;
            temppct = ((doorposition*100)/maxgap);
            
            //cout << "gap percentage: " << temppct;
            
            switch (tempstate) {
                    
                case 0: // fully closed
                    tempframe = (closed.getTotalNumFrames() * temppct) / 100;
                    //cout << " start frame: " << tempframe << endl;
                    //closed.setFrame(tempframe); // tempframe: frame at position, 0: beginning of video
                    closed.setFrame(0);
                    break;
                    
                case 2: // closing
                    tempframe = (closing.getTotalNumFrames() * (100 - temppct)) / 100;
                    //cout << " start frame: " << tempframe << endl;
                    //closing.setFrame(tempframe);
                    closing.setFrame(0);
                    break;
                    
                case 4: // opening
                    tempframe = (opening.getTotalNumFrames() * temppct) / 100;
                    //cout << " start frame: " << tempframe << endl;
                    //opening.setFrame(tempframe);
                    opening.setFrame(0);
                    break;
                    
                case 6: // fully opened
                    tempframe = (opened.getTotalNumFrames() * temppct) / 100;
                    //cout << " start frame: " << tempframe << endl;
                    //opened.setFrame(tempframe);
                    opened.setFrame(0);
                    break;
                default:
                    break;
            }
            
        } else {
            
            doorstatechanged = false;
            
        }
        
        temppct = NULL;
        lastdoorstate = doorstate;
        
        if (interrupt == true && doorstate == 6) {
            
            interrupt = false;
            
        }
        
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
    
    // placement of the completed moulds.
    
    projection.begin();
    ofBackground(0);
    lfTexture.draw(0,0);
    rfTexture.draw(fixwidth + gapwidth*3 + screenwidth*2,0);
    lTexture.draw((fixwidth + gapwidth)-doorposition,0);
    rTexture.draw((fixwidth + gapwidth*2 + screenwidth) + doorposition,0);
    projection.end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (startupcheck == false && startupchecked == false && startup == true) {
        
        // Display Images
        
        projection.draw(totalwidth/2,totalheight/2);
        
        // Monitoring Values
        
        ofDrawBitmapString(title + " at " + ofToString(ofGetFrameRate())+ "fps", 10, 50);
        ofSetColor(0,160,230);
        ofDrawBitmapString("'1' - OPEN  '2' - CLOSE  '3' - STOP ", 980, 50);
        ofSetColor(255,255,255);
        ofDrawBitmapString("Door Position: " + ofToString(doorposition), 10, 100 + totalheight2);
        ofDrawBitmapString("Door State: " + doorstate_str, 210, 100 + totalheight2);
        ofDrawBitmapString("Sensor says: " + presence_str, 410, 100 + totalheight2);
        ofSetColor(interruptswitchr,interruptswitchg,0);
        ofDrawBitmapString("Interrupted @ " + ofToString(interruptpct) + "%", 980, 100 + totalheight2);
        ofSetColor(255,255,255);
        
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
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == '1') {
        
        ofxOscMessage m;
        m.setAddress("/open");
        m.addIntArg(1);
        sender.sendMessage(m);
        
    }
    
    if (key == '2') {
        
        ofxOscMessage m;
        m.setAddress("/close");
        m.addIntArg(2);
        sender.sendMessage(m);
        
    }
    
    if (key == '3') {
        
        ofxOscMessage m;
        m.setAddress("/stop");
        m.addIntArg(3);
        sender.sendMessage(m);
        
    }
    
    if (key == 'k' || key == 'K') {
        
        cout << "\nナブコ: ＊＊＊お先に失礼します＊＊＊\n" << endl;
        std::exit(1);
        
    }
    
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

