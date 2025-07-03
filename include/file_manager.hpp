#pragma once

#include <string>
#include "satellite.hpp"

namespace oct {
    
class FileManager {
public:
    static Satellite loadSatelliteFromFile(std::string& path);
    static void saveSatelliteToFile(Satellite& satellite, std::string& path);
};

}