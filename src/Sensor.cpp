//
//  Sensor.cpp
//  platform
//
//  Created by テレンス on 2014/12/08.
//
//

#include "Sensor.h"


Sensor::Sensor() {
    
}


void Sensor::setSensorGrid(int _ofst8, int _ofst9, int _ofst10, int _ofst11, int _ofst12, int _ofst13) {
    
    // intakes sensor values, stores as int values
    
    area_1 = _ofst8;
    area_2 = _ofst9;
    area_3 = _ofst10;
    area_4 = _ofst11;
    area_5 = _ofst12;
    area_6 = _ofst13;
    
    // converts int -> int (binary form) -> stores as string in 2 dimensional array
    
    area[0] = getBinary(area_1);
    area[1] = getBinary(area_2);
    area[2] = getBinary(area_3);
    area[3] = getBinary(area_4);
    area[4] = getBinary(area_5);
    area[5] = getBinary(area_6);
    
}


void Sensor::setPresence() {
    
    // calculates Presence with sensor values
    
    int checker;
    
    if (area_1 != 0 || area_2 != 0 || area_3 != 0 || area_4 != 0) {
        
        checker = 1; // here-ness
        
    }
    
    else if (area_5 != 0 || area_6 != 0){
        
        checker = 2; // there-ness
        
    }
    
    else {
        
        checker = 0; // not here-ness
        
    }
    
    presence = checker;
    
}


int Sensor::getPresence() {
    
    // collects Presence
    
    return presence;
    
}


bool Sensor::checkSensorGrid(int _collumn, int _row) {
    
    // inquires with Sensor for presence in specific locations
    
    bool check = false;
    int x = _collumn;
    int y = _row;
    
    if (area[x].at(y) == '1') {
        check = true;
    }
    else {
        check = false;
    }
    
    return check;
    
}


string Sensor::getBinary(int _decimal) {
    
    // int -> string converter
    
    string result;
    int decimal = _decimal;
    
    std::bitset<8> bin_x(decimal);
    result = ofToString(bin_x);
    
    return result;
    
}

