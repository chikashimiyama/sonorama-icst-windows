#include "StylizerFactory.hpp"

std::unordered_map<std::string, Stylizer> StylizerFactory::styleCatalog = {{
    {"waterway", Stylizer(ofColor(40), 5)},
    {"railway", Stylizer(ofColor(60), 2)},
    {"leisure", Stylizer(ofColor(30), 1)},
    {"primary", Stylizer(ofColor(160), 6)},
    {"tertiary", Stylizer(ofColor(130), 5)},
    {"path", Stylizer(ofColor(100), 2)},
    {"footway", Stylizer(ofColor(90), 1)},
    {"service", Stylizer(ofColor(90), 1)},
    {"residential", Stylizer(ofColor(90), 1)},
    {"building", Stylizer(ofColor(255, 255, 255, 25), 1)},
    {"sphere", Stylizer(ofColor(255, 125, 125, 100), 1)}
}};
