#pragma once

#include "ofMain.h"
#include "Const.hpp"

class Plane {
    
public:
    void update(ofVec3f pt0, ofVec3f pt1, ofVec3f pt2, ofVec3f pt3) {
        p0 = pt0;
        p1 = pt1;
        p2 = pt2;
        p3 = pt3;
    }
    
    float distance(ofVec3f point) {
        ofVec3f v = p1 - p0;
        ofVec3f u = p2 - p0;
        ofVec3f n = v.cross(u);
        n.normalize();
        
        float A = n.x;
        float B = n.y;
        float C = n.z;
        float D = - n.dot(p1);
        
        return (A * point.x) + (B * point.y) + (C * point.z) + D;
    }
    
    void draw() {
        ofSetColor(ofColor::lightBlue);
        glBegin(GL_LINE_LOOP);
            glVertex3f(p0.x, p0.y, p0.z);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        glEnd();
    }
    
private:
    ofVec3f p0;
    ofVec3f p1;
    ofVec3f p2;
    ofVec3f p3;
};

class Camera : public ofEasyCam{
public:
    
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
        if (top.distance(point) < -100) return false;
        if (bottom.distance(point) < -100) return false;
        if (left.distance(point) < -100) return false;
        if (right.distance(point) < -100) return false;
        if (near.distance(point) < -100) return false;
        if (far.distance(point) < -100) return false;
        return true;
    }
    
    bool isInsideLabelFrustum(const ofVec3f &point){
        if (top.distance(point) < 0) return false;
        if (bottom.distance(point) < 0) return false;
        if (left.distance(point) < 0) return false;
        if (right.distance(point) < 0) return false;
        if (near.distance(point) < 0) return false;
        if (labelFar.distance(point) < 0) return false;
        return true;
    }
    
    
    const std::array<bool, NUM_AREA> &getVisibleAreas() const{
        return visibleAreas;
    }
    
    const std::array<bool, NUM_AREA> &getVisibleLabelAreas() const{
        return visibleLabelAreas;
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
        
        ofVec3f farCenter = position + lookAt * FAR_CLIP;
        ofVec3f ftl = farCenter + (upDirection * HALF_FAR_H) - (rightDirection * HALF_FAR_W);
        ofVec3f ftr = farCenter + (upDirection * HALF_FAR_H) + (rightDirection * HALF_FAR_W);
        ofVec3f fbl = farCenter - (upDirection * HALF_FAR_H) - (rightDirection * HALF_FAR_W);
        ofVec3f fbr = farCenter - (upDirection * HALF_FAR_H) + (rightDirection * HALF_FAR_W);
        
        ofVec3f farLabelCenter = position + lookAt * FAR_LABEL_CLIP;
        ofVec3f fltl = farLabelCenter + (upDirection * HALF_FAR_LABEL_H) - (rightDirection * HALF_FAR_LABEL_W);
        ofVec3f fltr = farLabelCenter + (upDirection * HALF_FAR_LABEL_H) + (rightDirection * HALF_FAR_LABEL_W);
        ofVec3f flbl = farLabelCenter - (upDirection * HALF_FAR_LABEL_H) - (rightDirection * HALF_FAR_LABEL_W);
        ofVec3f flbr = farLabelCenter - (upDirection * HALF_FAR_LABEL_H) + (rightDirection * HALF_FAR_LABEL_W);

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
        labelFar.update(fltr, fltl, flbl, flbr);
    }
    
    void updateVisibleArea(){
        int numVisible = 0;
        for(int i = 0; i < NUM_AREA; i++){
            int x = i % AREA_DIVISION;
            int z = i / AREA_DIVISION;
            ofVec3f areaCenter = ofVec3f(x * DIVIDER - HALF_MAP_SIZE,0, z * DIVIDER - HALF_MAP_SIZE);
            visibleAreas[i] = isInsideFrustum(areaCenter);
            visibleLabelAreas[i] = isInsideLabelFrustum(areaCenter);
            if(visibleAreas[i]) numVisible++;
        }
    
    }

    
    Plane top;
    Plane bottom;
    Plane left;
    Plane right;
    Plane near;
    Plane far;
    Plane labelFar;
    
    std::array<bool, NUM_AREA> visibleAreas;
    std::array<bool, NUM_AREA> visibleLabelAreas;

};

