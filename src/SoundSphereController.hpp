#include "ofMain.h"
#include "Camera.hpp"
#include "SphereModel.h"
#include "ModelController.hpp"

class SoundSphereController : public ModelController{
    
public:
    virtual void add(SInt64 id, std::vector<ofVec3f> vertices, unordered_map<std::string, std::string>tags) override{
        auto model = std::make_shared<SphereModel>(id, vertices, tags);
        ofVec3f pos = model->getPosition();
        std::pair<bool, int> area = getArea(model->getPosition());
        if(area.first){
            distribution[area.second].push_back(model);
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
};
