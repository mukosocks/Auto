//
//  Sensor.h
//  platform
//
//  Created by テレンス on 2014/12/08.
//
//

#ifndef __platforms__Sensor__
#define __platforms__Sensor__

#pragma once

#include "ofMain.h"
#include <iostream>     // std::cout
#include <string>       // std::string
#include <bitset>       // std::bitset


class Sensor {
    
public:
    
    Sensor();
    
    void setSensorGrid(int ofst8, int ofst9, int ofst10, int ofst11, int ofst12, int ofst13); 
    
    bool checkSensorGrid(int collumn, int row);
   
    void setPresence();
    int getPresence();
    
private:
    
    string getBinary(int decimal);
    
    string area [6];
    
    int area_1;
    int area_2;
    int area_3;
    int area_4;
    int area_5;
    int area_6;
    
    int presence;
    
};

#endif /* defined(__platform__Sensor__) */
