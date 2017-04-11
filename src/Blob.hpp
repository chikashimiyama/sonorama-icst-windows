#pragma once

typedef struct {
    int ID;
    int	parentID;
    double x;
    double y;
    double z;
    bool active;
    double age;
    int impactPartner;
    bool bound;
    double originDistance;
    double parentDistance;
    double angle;
    bool newTrigger;
    bool prevTrigger;
    bool canTrigger;
    int colorCode;
    ofColor color;
    
    ofRectangle rect;
    bool direction;
} Blob;
