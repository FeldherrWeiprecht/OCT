#include <iostream>
#include <string>

#include "satellite.hpp"
#include "constants.hpp"

void printLogo();
void selectMeasurementSystem();
std::string selectSatelliteDataInputMode();
oct::Satellite getSatelliteData(std::string& input = "manual");

int main(){
    printLogo();
    selectMeasurementSystem();

    std::string satelliteDataInputMode = selectSatelliteDataInputMode();
    oct::Satellite satellite = getSatelliteData(satelliteDataInputMode);

    std::string name = "SPER3";
    double mass = 100;
    double velocity = 20;
    double altitude = 100;
    oct::Satellite sattelite(name, mass, velocity, altitude);
    sattelite.orbitalVelocity();
    sattelite.orbitalPeriod();
    sattelite.mechanicalEnergy();
    sattelite.potentialEnergy();
    sattelite.kineticEnergy();

    return 0;
}

void printLogo(){
    std::string logo = R"(  ____   ____ _____ 
 / __ \ / ___|_   _|
| |  | | |     | |  
| |__| | |___  | |  
 \____/ \____| |_|)";
    std::cout << logo << std::endl;
}

void selectMeasurementSystem() {
    std::cout << "Select a measurement system:" << std::endl;
    std::cout << "1. SI (kilogram, kilometer, km/h)" << std::endl;
    std::cout << "2. Imperial (pound, feet, mph)" << std::endl;
    std::cout << "3. Nautical (pound, feet, kn)" << std::endl;
    std::cout << "Enter your choice (1–3): ";

    int input;
    std::cin >> input;

    // omg switch statement
    switch (input) {
        case 1:
            std::cout << "You selected SI measurement system." << std::endl;
            break;
        case 2:
            std::cout << "You selected Imperial measurement system." << std::endl;
            break;
        case 3:
            std::cout << "You selected Nautical measurement system." << std::endl;
            break;
        default:
            std::cout << "Invalid choice! Please try again." << std::endl;
            selectMeasurementSystem();
            break;
    }
}

std::string selectSatelliteDataInputMode(){
    std::cout << "Select satellite data input mode:" << std::endl;
    std::cout << "1. Manual input" << std::endl;
    std::cout << "2. Load from file" << std::endl;
    std::cout << "Enter your choice (1–2): ";

    int input;
    std::cin >> input;

    switch (input) {
        case 1:
            return "manual";
        case 2:
            return "file";
        default:
            std::cout << "Invalid choice! Please try again." << std::endl;
            return selectSatelliteDataInputMode();
    }
}

// todo: data validation, reenter input if invalid and show which measurement unit is used
oct::Satellite getSatelliteData(std::string& input = "manual") {
    if (input == "manual") {
        std::string name;
        double mass, velocity, altitude;

        std::cout << "Enter satellite name: ";
        std::cin >> name;
        std::cout << "Enter satellite mass: ";
        std::cin >> mass;
        std::cout << "Enter satellite velocity: ";
        std::cin >> velocity;
        std::cout << "Enter satellite altitude: ";
        std::cin >> altitude;

        return oct::Satellite(name, mass, velocity, altitude);
    } else if (input == "file") {
        std::string path;
        std::cout << "Enter the path to the satellite json file: ";
        std::cin >> path;

        return oct::FileManager::loadSatelliteFromFile(path);
    } else {
        throw std::invalid_argument("Invalid input: " + input);
    }
}
