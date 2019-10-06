#pragma once

#include "ofMain.h"
#include "Plane.hpp"
#include "Const.hpp"

class Camera : public ofCamera{
public:
	void setup(int camID, ofVec3f position){
		this->camID = camID;
		setPosition(position);
		setNearClip(NEAR_CLIP);
		setFarClip(FAR_CLIP);
		setFov(FOV);
		setForceAspectRatio(true);
		setAspectRatio(ASPECT_RATIO);
		rotate(-90*camID -45,0,1,0);

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
        nearPlane.draw();
        farPlane.draw();
    }
    
    bool isInsideFrustum(const ofVec3f &point){
        if (top.distance(point) < 0) return false;
        if (bottom.distance(point) < 0) return false;
        if (left.distance(point) < 100) return false;
        if (right.distance(point) < 100) return false;
        if (nearPlane.distance(point) < 0) return false;
        if (farPlane.distance(point) < 0) return false;
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
        
        ofVec3f farPlaneCenter = position + lookAt * LABEL_FAR_CLIP;
        ofVec3f ftl = farPlaneCenter + (upDirection * HALF_FAR_H) - (rightDirection * HALF_FAR_W);
        ofVec3f ftr = farPlaneCenter + (upDirection * HALF_FAR_H) + (rightDirection * HALF_FAR_W);
        ofVec3f fbl = farPlaneCenter - (upDirection * HALF_FAR_H) - (rightDirection * HALF_FAR_W);
        ofVec3f fbr = farPlaneCenter - (upDirection * HALF_FAR_H) + (rightDirection * HALF_FAR_W);
        
        ofVec3f nearPlaneCenter = position + lookAt * NEAR_CLIP;
        ofVec3f ntl = nearPlaneCenter + (upDirection * HALF_NEAR_H) - (rightDirection * HALF_NEAR_W);
        ofVec3f ntr = nearPlaneCenter + (upDirection * HALF_NEAR_H) + (rightDirection * HALF_NEAR_W);
        ofVec3f nbl = nearPlaneCenter - (upDirection * HALF_NEAR_H) - (rightDirection * HALF_NEAR_W);
        ofVec3f nbr = nearPlaneCenter - (upDirection * HALF_NEAR_H) + (rightDirection * HALF_NEAR_W);
        
        top.update(ntr,ntl,ftl,ftr);
        bottom.update(nbl,nbr,fbr,fbl);
        left.update(ntl,nbl,fbl,ftl);
        right.update(nbr,ntr,ftr,fbr);
        nearPlane.update(ntl,ntr,nbr,nbl);
        farPlane.update(ftr,ftl,fbl,fbr);
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
    Plane nearPlane;
    Plane farPlane;
    int camID;
  
    std::array<bool, NUM_AREA> visibleAreas;

};

