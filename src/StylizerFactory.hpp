#pragma once

#include <unordered_map>
#include <Stylizer.hpp>

#include "ofMain.h"

class StylizerFactory{
public:
    static Stylizer &getStylizer(const std::string &name){
        return styleCatalog.at(name);
    };
private:
    static std::unordered_map<std::string, Stylizer> styleCatalog;
};
