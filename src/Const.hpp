#pragma once

#include <utility>

const int WIDTH = 1024;
const int HEIGHT = 768;
const float ASPECT_RATIO = static_cast<float>(WIDTH)/static_cast<float>(HEIGHT);
const float FOV = 90;

const int AREA_DIVISION = 40;
const int NUM_AREA = AREA_DIVISION * AREA_DIVISION;
const float MAP_SIZE = 2000.0;
const float HEIGHT_LIMIT = 1500;
const float HALF_MAP_SIZE = MAP_SIZE/2.0;
const float DIVIDER = MAP_SIZE / AREA_DIVISION;

//frustum
const float NEAR_CLIP = 20;
const float NEAR_H     = 2 * tan(FOV / 2.0) * -NEAR_CLIP;
const float NEAR_W     = NEAR_H * ASPECT_RATIO;
const float HALF_NEAR_H = NEAR_H/2;
const float HALF_NEAR_W = NEAR_W/2;

const float FAR_CLIP = 2000;
const float FAR_H      = 2 * tan(FOV / 2.0) * -FAR_CLIP;
const float FAR_W      = FAR_H * ASPECT_RATIO;
const float HALF_FAR_H = FAR_H/2;
const float HALF_FAR_W = FAR_W/2;




const std::string FONT = "alien.ttf";

inline std::pair<bool, int> getArea(ofVec3f &pos){
    if(pos.x < -HALF_MAP_SIZE || HALF_MAP_SIZE < pos.x) return std::make_pair(false,-1);
    if(pos.z < -HALF_MAP_SIZE || HALF_MAP_SIZE < pos.z) return std::make_pair(false,-1);
    if(pos.y > HEIGHT_LIMIT) return std::make_pair(false,-1);
    int x = (pos.x + HALF_MAP_SIZE) / DIVIDER;
    int z = (pos.z + HALF_MAP_SIZE) / DIVIDER;
    int index = z * AREA_DIVISION + x;
    return std::make_pair(true, index);
}

inline std::tuple<bool, int, int> areaToXY(std::pair<bool,int> area){
    if(!area.first){
        return std::make_tuple(false, 0,0);
    }
    int x = area.second % AREA_DIVISION;
    int z = area.second / AREA_DIVISION;
    return std::make_tuple(true, x,z);
}

inline std::tuple<bool, int, int> getAreaXY(ofVec3f &pos){
    return areaToXY(getArea(pos));
}
