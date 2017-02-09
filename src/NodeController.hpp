#pragma once

#include "Node.hpp"
#include "Bounds.hpp"

class NodeController{
public:
    void add(SInt64 id, float lat, float lon, std::unordered_map<std::string, std::string> tags){
       nodes.emplace(id, Node(lat, lon, tags));
    }
    
    ofVec3f getVertex(SInt64 id){
        Node node = nodes.at(id);
        Bounds *bounds = Bounds::getSingleton();
        return bounds->scale(node.getLat(), node.getLon());
    }
    
    size_t getNumberOfNodes() const{
        return nodes.size();
    }
    
private:
    std::unordered_map<UInt64, Node> nodes;
};
