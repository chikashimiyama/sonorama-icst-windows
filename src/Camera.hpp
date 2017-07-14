#pragma once

#include "ofMain.h"
#include "Plane.hpp"
#include "Const.hpp"

class Camera : public ofCamera{
public:
	void setup(int camID, ofVec3f position){
		this->camID = camID;
		setPosition(position);

	}
	int getCameraID() const{
		return camID;
	}
    void update(){
        updateFrustum();
        updateVisibleArea();
    }
    
    void drawFrustum(){
        top.draw();
        bottom.draw();
        left.draw();
        right.draw();
        near.draw();
        far.draw();
    }
    
    bool isInsideFrustum(const ofVec3f &point){
        if (top.distance(point) < 0) return false;
        if (bottom.distance(point) < 0) return false;
        if (left.distance(point) < 100) return false;
        if (right.distance(point) < 100) return false;
        if (near.distance(point) < 0) return false;
        if (far.distance(point) < 0) return false;
        return true;
    }
    
    const std::array<bool, NUM_AREA> &getVisibleAreas() const{
        return visibleAreas;
    }

    
    int getNumVisibleAreas() const{
        int i = 0;
        for(auto area : visibleAreas){
            if(area)i++;
        }
        return i;
    }
    
private:
    void updateFrustum(){
        
        ofVec3f position  = getPosition();
        ofVec3f lookAt  = getLookAtDir();
        ofVec3f upDirection  = getUpDir();
        ofVec3f rightDirection = getXAxis();
        
        ofVec3f farCenter = position + lookAt * LABEL_FAR_CLIP;
        ofVec3f ftl = farCenter + (upDirection * HALF_FAR_H) - (rightDirection * HALF_FAR_W);
        ofVec3f ftr = farCenter + (upDirection * HALF_FAR_H) + (rightDirection * HALF_FAR_W);
        ofVec3f fbl = farCenter - (upDirection * HALF_FAR_H) - (rightDirection * HALF_FAR_W);
        ofVec3f fbr = farCenter - (upDirection * HALF_FAR_H) + (rightDirection * HALF_FAR_W);
        
        ofVec3f nearCenter = position + lookAt * NEAR_CLIP;
        ofVec3f ntl = nearCenter + (upDirection * HALF_NEAR_H) - (rightDirection * HALF_NEAR_W);
        ofVec3f ntr = nearCenter + (upDirection * HALF_NEAR_H) + (rightDirection * HALF_NEAR_W);
        ofVec3f nbl = nearCenter - (upDirection * HALF_NEAR_H) - (rightDirection * HALF_NEAR_W);
        ofVec3f nbr = nearCenter - (upDirection * HALF_NEAR_H) + (rightDirection * HALF_NEAR_W);
        
        top.update(ntr,ntl,ftl,ftr);
        bottom.update(nbl,nbr,fbr,fbl);
        left.update(ntl,nbl,fbl,ftl);
        right.update(nbr,ntr,ftr,fbr);
        near.update(ntl,ntr,nbr,nbl);
        far.update(ftr,ftl,fbl,fbr);
    }
    
    void updateVisibleArea(){
        int numVisible = 0;
        for(int i = 0; i < NUM_AREA; i++){
            int x = i % AREA_DIVISION;
            int z = i / AREA_DIVISION;
            ofVec3f areaCenter = ofVec3f(x * DIVIDER - HALF_MAP_SIZE,0, z * DIVIDER - HALF_MAP_SIZE);
            visibleAreas[i] = isInsideFrustum(areaCenter);
            if(visibleAreas[i]) numVisible++;
        }
    }

    Plane top;
    Plane bottom;
    Plane left;
    Plane right;
    Plane near;
    Plane far;
    int camID;
  
    std::array<bool, NUM_AREA> visibleAreas;

};

