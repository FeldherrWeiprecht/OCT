#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "file_manager.hpp"
#include "satellite.hpp"

namespace oct {
 
    Satellite FileManager::loadSatelliteFromFile(std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file with the path: " + path);
        }
        nlohmann::json data;
        file >> data;

        oct::Satellite satellite(
            data["name"].get<std::string>(),
            data["mass"].get<double>(),
            data["velocity"].get<double>(),
            data["altitude"].get<double>()
        );

        return satellite;
    }

    void FileManager::saveSatelliteToFile(Satellite& satellite, std::string& path) {
        std::ofstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the file with the path: " + path);
        }
        nlohmann::json data;

        data["name"] = satellite.name;
        data["mass"] = satellite.mass;
        data["velocity"] = satellite.velocity;
        data["altitude"] = satellite.altitude;

        file << data.dump(4);
    }

}