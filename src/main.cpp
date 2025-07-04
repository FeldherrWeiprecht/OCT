#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>

#include "satellite.hpp"
#include "constants.hpp"
#include "file_manager.hpp"

void printLogo();
void selectMeasurementSystem();
std::string selectSatelliteDataInputMode();
oct::Satellite getSatelliteData(const std::string& input = "manual");
void printErrorMessage(const std::string& message = "Invalid input! Try again.");

int main(){
    printLogo();
    selectMeasurementSystem();

    std::string satelliteDataInputMode = selectSatelliteDataInputMode();
    oct::Satellite satellite = getSatelliteData(satelliteDataInputMode);
    std::cout << std::endl;
    satellite.orbitalVelocity();
    satellite.orbitalPeriod();
    satellite.mechanicalEnergy();
    satellite.potentialEnergy();
    satellite.kineticEnergy();
    satellite.escapeVelocity();

    return 0;
}

void printLogo(){
    std::system("cls");
    std::string logo = R"(
      $$$$$$\    $$$$$\    $$$$$$$$\ 
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
    std::string message = "\nInvalid choice! Try again.\n\n";

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
            printLogo();
            printErrorMessage(message);
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
                printLogo();
                printErrorMessage(message);
        }
    }
    printLogo();
}

std::string selectSatelliteDataInputMode(){
    int input;
    bool valid = false;
    std::string message = "\nInvalid choice! Try again.\n\n";

    while (valid == false) {
        std::cout << "Select satellite data input mode:" << std::endl;
        std::cout << "1. Manual input" << std::endl;
        std::cout << "2. Load from file" << std::endl;
        std::cout << "Enter your choice (1-2): ";

        std::cin >> input;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printLogo();
            printErrorMessage(message);
            continue;
        }

        switch (input) {
        case 1:
            return "manual";
        case 2:
            return "file";
        default:
            printLogo();
            printErrorMessage(message);
        }
    }
    printLogo();
    return "manual";
}

oct::Satellite getSatelliteData(const std::string& input) {
    std::cout << std::endl;

    if (input == "manual") {
        std::string name;
        double mass, velocity, altitude;
        std::string message = "Invalid input! ";

        printLogo();
        std::cout << "Enter satellite name: ";
        while (!(std::cin >> name)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage();
            std::cout << "Enter a valid name for the satellite: ";
        }

        printLogo();
        std::cout << "Enter satellite mass (kg): ";
        while (!(std::cin >> mass)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage(message);
            std::cout << "Enter a valid number for the mass: ";
        }

        printLogo();
        std::cout << "Enter satellite velocity (km/h): ";
        while (!(std::cin >> velocity)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage(message);
            std::cout << "Enter a valid number for the velocity: ";
        }
        
        printLogo();
        std::cout << "Enter satellite altitude (km): ";
        while (!(std::cin >> altitude)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage(message);
            std::cout << "Enter a valid number for altitude: ";
        }
        printLogo();

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

void printErrorMessage(const std::string& message){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << message;
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
