
#pragma once


#include "ofMain.h"
#include "Const.hpp"
//Singleton class Bounds

class Bounds{
    
public:
    static Bounds * getSingleton(){
        if(!theInstance)
            theInstance = new Bounds;
        return theInstance;
    };
    
    void setup(float minLat, float maxLat, float minLon, float maxLon){
        this->minLat = minLat;
        this->distLat = maxLat - minLat;
        this->minLon = minLon;
        this->distLon = maxLon - minLon;
    }

    ofVec3f scale(float lat, float lon){
        float x = ((lat - minLat) / distLat ) * MAP_SIZE - HALF_MAP_SIZE;
        float z = ((lon - minLon) / distLon ) * MAP_SIZE - HALF_MAP_SIZE;
        return ofVec3f(x, 0, z);
    };

private:
    Bounds(){}; // cannot instantiate by constructor
    float minLat;
    float minLon;
    float distLat;
    float distLon;
    
    static Bounds * theInstance;
};

