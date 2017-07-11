#include "SoundEvent.hpp"

class SphereModel : public Model{

public:
    SphereModel(SInt64 id, std::vector<ofVec3f> vertices, std::unordered_map<std::string, std::string> tags):
    Model(id,vertices,tags),
    radius(100.0),
    playing(false){}
    
    virtual void draw() const override{
        ofSetColor(SPHERE_COLOR);
        ofDrawSphere(position, radius);
        ofSetColor(CORE_COLOR);
        ofDrawSphere(position, CORE_RADIUS);
    }
    
    virtual void label(const Camera &camera) const override{
        ofSetColor(CORE_COLOR);
        ofSetLineWidth(1);
        if(name.size() == 0 )return;
        ofRectangle viewPort = ofRectangle(camera.getCameraID() * WIDTH, 0, WIDTH, HEIGHT);
        ofVec2f posOnScreen = camera.worldToScreen(position, viewPort);
        if(!viewPort.inside(posOnScreen)) return;
        
        ofVec2f captionPos = posOnScreen;
        captionPos.y -= SPHERE_CAPTION_HEIGHT;
        
        ofDrawLine(posOnScreen, captionPos);
        ofDrawCircle(captionPos.x, captionPos.y, BULLET_SIZE);
        captionPos.x += LABEL_INDENT;
        FontServer * fontServer = FontServer::getSingleton();
        fontServer->drawText(captionPos, name);
    }
    
    void setRadius(float r){
        radius = r;
    }
    
    void update(const ofVec3f &cameraPos) {
        float distance = position.distance(cameraPos);
        
        if(distance <= SOUND_DISTANCE_THRESHOLD){
            float ratio = 1.0 - (distance / SOUND_DISTANCE_THRESHOLD);
            if(ratio < 0.0){
                ofLog() << "negative";
            }
            setVolume(ratio);
            setAngle(calcAngleOn2D(cameraPos, position));
            if(!playing) play();

        }else if(distance > SOUND_DISTANCE_THRESHOLD && playing){
            stop();
        }
    }
    
    
private:
    
    float radius;
    bool playing;
    
    void play(){
        playing = true;
        SoundEvent soundEvent(SoundEventType::PLAY, id);
        ofNotifyEvent(SoundEvent::events, soundEvent);
    }
    
    void stop(){
        playing =false;
        SoundEvent soundEvent(SoundEventType::STOP, id);
        ofNotifyEvent(SoundEvent::events, soundEvent);
    }
    
    void setVolume(float vol){
        SoundEvent soundEvent = SoundEvent(SoundEventType::VOLUME, id, vol);
        ofNotifyEvent(SoundEvent::events, soundEvent);
    }
    
    void setAngle(float angle){
        SoundEvent soundEvent = SoundEvent(SoundEventType::ANGLE, id, angle);
        ofNotifyEvent(SoundEvent::events, soundEvent);
    }
    
    float calcAngleOn2D(const ofPoint &p1, const ofPoint &p2){
        float rad = atan2(p2.z - p1.z, p2.x - p1.x);
        return rad / PI * 180.0 - 45;
    }

};
