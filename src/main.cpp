#include <iostream>
#include <string>

#include "satellite.hpp"
#include "constants.hpp"
#include "file_manager.hpp"

void printLogo();
void selectMeasurementSystem();
std::string selectSatelliteDataInputMode();
oct::Satellite getSatelliteData(const std::string& input = "manual");

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
    std::string logo = R"(
        $$$$$$\    $$$$$$\   $$$$$$$$\ 
        $$  __$$\  $$  __$$\  \__$$  __|
    $$ /  $$ |  $$ /  \__|    $$ |
    $$ |  $$ |  $$ |          $$ |
    $$ |  $$ |  $$ |          $$ |
    $$ |  $$ |  $$ |  $$$$    $$ |  
    \$$$$$$  |  \$$$$$$$ /    $$ |
        \______/    \______/     \__|
    )";
    std::cout << logo << std::endl;
}

void selectMeasurementSystem() {
    int input;
    bool valid = false;

    while (valid == false) {
        std::cout << "Select a measurement system:" << std::endl;
        std::cout << "1. SI (kilogram, kilometer, km/h)" << std::endl;
        std::cout << "2. Imperial (pound, feet, mph)" << std::endl;
        std::cout << "3. Nautical (pound, feet, kn)" << std::endl;
        std::cout << "Enter your choice (1-3): ";

        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << std::endl << "Invalid input! Try again." << std::endl << std::endl;
            continue;
        }

        switch (input) {
            case 1:
                std::cout << "You selected SI measurement system." << std::endl;
                valid = true;
                break;
            case 2:
                std::cout << "You selected Imperial measurement system." << std::endl;
                valid = true;
                break;
            case 3:
                std::cout << "You selected Nautical measurement system." << std::endl;
                valid = true;
                break;
            default:
                std::cout << std::endl << "Invalid choice! Try again." << std::endl << std::endl;
        }
    }
}

std::string selectSatelliteDataInputMode(){
    int input;
    bool valid = false;


    while (valid == false) {
        std::cout << "Select satellite data input mode:" << std::endl;
        std::cout << "1. Manual input" << std::endl;
        std::cout << "2. Load from file" << std::endl;
        std::cout << "Enter your choice (1-2): ";

        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << std::endl << "Invalid input! Try again." << std::endl << std::endl;
            continue;
        }

        switch (input) {
        case 1:
            return "manual";
        case 2:
            return "file";
        default:
            std::cout << std::endl << "Invalid choice! Try again." << std::endl << std::endl;
        }
    }

    return "manual";
}

oct::Satellite getSatelliteData(const std::string& input) {
    if (input == "manual") {
        std::string name;
        double mass, velocity, altitude;

        std::cout << "Enter satellite name: ";
        while (!(std::cin >> name)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Enter a valid name for the satellite: ";
        }

        std::cout << "Enter satellite mass (kg): ";
        while (!(std::cin >> mass)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Enter a valid number for the mass: ";
        }

        std::cout << "Enter satellite velocity (km/h): ";
        while (!(std::cin >> velocity)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Enter a valid number for the velocity: ";
        }

        std::cout << "Enter satellite altitude (km): ";
        while (!(std::cin >> altitude)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input! Enter a valid number for altitude: ";
        }

        return oct::Satellite(name, mass, velocity, altitude);
    }
    else if (input == "file") {
        std::string path;
        std::cout << "Enter the path to the satellite json file: ";
        std::cin >> path;

        return oct::FileManager::loadSatelliteFromFile(path);
    }
    else {
        throw std::invalid_argument("Invalid input: " + input);
    }
}
