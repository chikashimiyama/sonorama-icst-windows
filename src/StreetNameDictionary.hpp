#pragma once
#include "Node.hpp"

class StreetNameDictionary{
    
public:
    static StreetNameDictionary * getSingleton(){
        if(!theInstance){
            theInstance = new StreetNameDictionary;
        }
        return theInstance;
    };
    

    
private:
    StreetNameDictionary(){}; // cannot instantiate by constructor
 
    static StreetNameDictionary * theInstance;
    
};
