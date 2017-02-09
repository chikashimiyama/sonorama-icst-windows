#include "StylizerFactory.hpp"

std::unordered_map<std::string, Stylizer> StylizerFactory::styleCatalog = {{
    {"waterway", Stylizer(ofColor(30,30,50), 4)},
    {"railway", Stylizer(ofColor(90,30,20), 1)},
    {"leisure", Stylizer(ofColor(30,50,20), 1)},
    {"primary", Stylizer(ofColor(245), 4)},
    {"tertiary", Stylizer(ofColor(230), 3)},
    {"path", Stylizer(ofColor(215), 2)},
    {"footway", Stylizer(ofColor(200), 1)},
    {"service", Stylizer(ofColor(185), 1)},
    {"residential", Stylizer(ofColor(185), 1)},
    {"building", Stylizer(ofColor(60), 1)}
}};
