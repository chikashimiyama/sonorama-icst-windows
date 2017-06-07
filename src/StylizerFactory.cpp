#include "StylizerFactory.hpp"

std::unordered_map<std::string, Stylizer> StylizerFactory::styleCatalog = {{
    {"waterway", Stylizer(ofColor(40,40,40,150), 5)},
    {"railway", Stylizer(ofColor(60,60,60,150), 2)},
    {"leisure", Stylizer(ofColor(30,30,30,150), 1)},
    {"primary", Stylizer(ofColor(160, 160, 160, 150), 6)},
    {"tertiary", Stylizer(ofColor(130,130,130,150), 5)},
    {"path", Stylizer(ofColor(100,100,100,150), 2)},
    {"footway", Stylizer(ofColor(90,90,90,150), 1)},
    {"service", Stylizer(ofColor(90,90,90,150), 1)},
    {"residential", Stylizer(ofColor(90,90,90,150), 1)},
    {"building", Stylizer(ofColor(255, 255, 255, 25), 1)},
    {"sphere", Stylizer(ofColor(255, 125, 125, 100), 1)}
}};
