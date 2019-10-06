#pragma once

#include "ofMain.h"
#include "Camera.hpp"
#include "SphereModel.h"
#include "ModelController.hpp"
#include "SoundEngine.hpp"

class SoundSphereController : public ModelController{
    
public:
    SoundSphereController(const SoundEngine &se):
    soundEngine(se){}
    
    virtual void add(long long id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) override{
        auto model = std::make_shared<SphereModel>(id, vertices, tags);
        ofVec3f pos = model->getPosition();
        std::pair<bool, int> area = getArea(model->getPosition());
        if(area.first){
            distribution[area.second].push_back(model);
            //ofLog() << "added to area " << area.second;
        }else{
            ofLog() << "outside of area ";

        }
    }
    
    virtual void draw() const override{
        for(auto &area : distribution){
            for(auto &model :area){
                model->draw();
            }
        }
    }
    
    virtual void draw(Camera &camera) const override{
        auto visibleAreas = camera.getVisibleAreas();
        for(int i = 0; i < NUM_AREA; i++){
            if(!visibleAreas[i]) continue;
            for(auto &model :distribution[i]){
                model->draw();
            }
        }
    }
    
    void updateSound(const ofVec3f &cameraPos){
        for(auto &area : distribution){
            for(auto &model :area){
                static_cast<SphereModel*>(model.get())->update(cameraPos);
            }
        }
    }

    void updateLevels(){
        const std::vector<float> &amplitude = soundEngine.getAmplitude();
        
        for(auto &area : distribution){
            for(auto &model :area){
                SphereModel * sphereModel = static_cast<SphereModel*>(model.get());
                auto idNumber = sphereModel->getId();
                sphereModel->setRadius(amplitude[idNumber] * SPHERE_SCALE);
            }
        }
    }
	

    
private:
    const SoundEngine &soundEngine;
    
};
