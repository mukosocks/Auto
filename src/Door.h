//
//  Door.h
//  platform
//
//  Created by テレンス on 2014/12/08.
//
//

#ifndef __platforms__Door__
#define __platforms__Door__

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"         // for sending commands to Door

#define HOST "localhost"
#define SENDPORT 12345      // processing PORT Number

class Door{
    
public:
    
    Door();
    
    ofxOscSender sender;
    
    void setPosition(int ofst4, int ofst5);
    void setState(int ofst6);
    void setMaxGap(int gap);
    
    int getPosition();
    int getState();
    int getMaxGap();
    
    void open();
    void close();
    void stop();
    
private:
    
    int position;   // stores position of door
    int state;      // stores state of door
    int maxgap;     // stores maximum gap size of the door
    
};

#endif 

/* defined(__platform__Door__) */
