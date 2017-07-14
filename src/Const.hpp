#pragma once


#include <utility>

#define panorama
//#define mini

//const std::string CITY_NAME = "karlsruhe";
const std::string CITY_NAME = "zurich";

const std::string MAP_DATA_FILE = "maps/" + CITY_NAME+".osm";
const std::string DATA_BASE_FILE = "pd/soundfiles/"+ CITY_NAME + ".db";
const std::string SOUND_FILE_PATH = "pd/sounrfiles/"+ CITY_NAME + "/";


#ifdef mini
const int WIDTH = 640;
const int HEIGHT = 360;

#else
const int WIDTH = 1280;
const int HEIGHT = 720;
#endif


/******* panorama mode *******/
#ifdef panorama
const int NUM_VIEWPORTS = 4;
const int SCREEN_HEIGHT = HEIGHT;

const int SCREEN_WIDTH =  WIDTH*NUM_VIEWPORTS;
const float ASPECT_RATIO = 1;

#else
const int NUM_VIEWPORTS = 4;
const int SCREEN_WIDTH =  WIDTH/NUM_VIEWPORTS;
const float SCREEN_HEIGHT = HEIGHT;
const float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT);
#endif

const float FOV = 360 / NUM_VIEWPORTS;
const int NUM_SPEAKERS = 16;


/******* area division for faster rendering *******/

const int AREA_DIVISION = 50;
const int NUM_AREA = AREA_DIVISION * AREA_DIVISION;
const float MAP_SIZE = 25000;
const float HEIGHT_LIMIT = 2500;
const float HALF_MAP_SIZE = MAP_SIZE/2.0;
const float DIVIDER = MAP_SIZE / AREA_DIVISION;

/******* camera setting *******/
const float CAMERA_HEIGHT = 250;
const float NEAR_CLIP = 1;
const float NEAR_H     = 2 * tan(FOV / 2.0) * -NEAR_CLIP;
const float NEAR_W     = NEAR_H * ASPECT_RATIO;
const float HALF_NEAR_H = NEAR_H/2;
const float HALF_NEAR_W = NEAR_W/2;

const float FAR_CLIP = 400000;
const float LABEL_FAR_CLIP = 3000;
const float FAR_H      = 2 * tan(FOV / 2.0) * -LABEL_FAR_CLIP;
const float FAR_W      = FAR_H * ASPECT_RATIO;
const float HALF_FAR_H = FAR_H/2;
const float HALF_FAR_W = FAR_W/2;
const ofVec3f INITIAL_POS = ofVec3f(0,CAMERA_HEIGHT,0);
const float AREA_RADIUS = 20000;

/******* sound setting *******/
const float CORE_RADIUS = 10;
const float SPHERE_SCALE = 2.0;
const float SPHERE_CAPTION_HEIGHT = 120;
const ofColor SPHERE_COLOR = ofColor(100,255,255, 40);
const ofColor CORE_COLOR = ofColor(100,250,255, 255);
const std::string PD_PATCH = "pd/main.pd";
const int TICKS_PER_BUFFER = 8;
const float SOUND_DISTANCE_THRESHOLD = 2500;
const std::string PD_RECEIVE_NAME = "fromOF";
const int NUM_PLAYERS = 71;
const int SAMPLE_RATE = 48000;


/******* constructions *******/
const float LEVEL_HEIGHT = 300;
const float LABEL_INDENT = 5;
const float ANTI_FLICKER_SCALE_DOWN_RATIO = 0.93;
const float BULLET_SIZE = 2;

/******* typography *******/
const std::string FONT = "zurich.ttf";
const float FONT_SIZE = 12;


/******* interaction *******/
/******* ICST *******/

const bool TUIO_ENABLED = true;
const bool INTER_SENSE_ENABLED = false;
const int CIRCLE_RESOLUTION = 50;

const std::string TRACK_MASTER_IP = "224.0.0.1";
const int TRACK_MASTER_PORT = 64000;
const int MY_TUIO_PORT = 23456;
const float TOUCH_TO_MOVE = 2.5;
/******* ZKM *******/
const std::string INERTIA_CUBE_PATH = "/dev/tty.SLAB_USBtoUART";


/******* credit *******/
const std::string CREDIT_TEXT = "(c) OpenStreetMap";
const float CREDIT_X = 20;
const float CREDIT_Y = HEIGHT - 15;
const ofVec2f CREDIT_POS = ofVec2f(CREDIT_X, CREDIT_Y);

/******* projection *******/
const std::string SYPHON_IP = "224.0.0.1";
const int SYPHON_PORT = 8400;

inline std::pair<bool, int> getArea(const ofVec3f &pos){
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
