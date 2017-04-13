#pragma once

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
