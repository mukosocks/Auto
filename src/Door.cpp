//
//  Door.cpp
//  platform
//
//  Created by テレンス on 2014/12/08.
//
//

#include "Door.h"

Door::Door() {
    
    // initialize Door Object
    
    sender.setup(HOST, SENDPORT);
    
    position = 0;
    state = 0;
    maxgap = 0;
    
}


void Door::setPosition(int _ofst4, int _ofst5) {
    
    // intakes Door values, sets Door Position
    
    int total = 0;
    int high = 255;
    int low = 0;
    int pos_a = _ofst4;
    int pos_b = _ofst5;
    
    if (pos_b == 1){
        total = pos_a + high;
    }
    
    else if (pos_b == 2) {
        total = pos_a + (high*2);
    }
    
    else if (pos_b == 3) {
        total = pos_a + (high*3);
    }
    
    else {
        total = pos_a + low;
    }
    
    position = total;
    
}


void Door::setState(int _ofst6) {
    
    // intakes Door values, sets State
    
    state = _ofst6;

}

void Door::setMaxGap(int _gap) {
    
    // intakes Door values, sets Maximum Gap size
    
    maxgap = _gap;
    
}


int Door::getPosition() {
    
    // collects Position
    
    return position;
}


int Door::getState() {
    
    // collects State
    
    return state;
}

int Door::getMaxGap() {
    
    // collects MaxGap
    
    return maxgap;
    
}

void Door::open() {
    
    // sends Open Command to Door
    
    ofxOscMessage m;
    m.setAddress("/open");
    m.addIntArg(1);
    sender.sendMessage(m);
    
}

void Door::close() {
    
    // sends Close Command to Door
    
    ofxOscMessage m;
    m.setAddress("/open");
    m.addIntArg(2);
    sender.sendMessage(m);
    
}

void Door::stop() {
    
    // sends Stop Command to Door
    
    ofxOscMessage m;
    m.setAddress("/open");
    m.addIntArg(3);
    sender.sendMessage(m);
    
}