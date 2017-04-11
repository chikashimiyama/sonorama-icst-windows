#pragma once

#include "Model.hpp"
#include "Camera.hpp"
#include "Const.hpp"
#include "ModelController.hpp"

/* abstract class for controlling multiple static 3D models using VBOs */
class VboModelController : public ModelController{

public:
    VboModelController(const std::string &styleName ):
    vboRenderer(styleName){};
    void initVbo(){
        vboRenderer.setup(allVertices,allIndices);
    }
    
protected:
    VboRenderer vboRenderer;
    virtual std::vector<ofIndexType> createIndices(int offset, size_t numVertex) = 0;
    std::vector<ofVec3f> allVertices;
    std::vector<ofIndexType> allIndices;
};
