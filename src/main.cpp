#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <exception>
#include <thread>
#include <chrono>

#include "satellite.hpp"
#include "constants.hpp"
#include "file_manager.hpp"
#include "unit_converter.hpp"

int measurementSystem = 1;

void printLogo();
void selectMeasurementSystem();
std::string selectSatelliteDataInputMode();
oct::Satellite getSatelliteData(const std::string& input = "manual");
void printErrorMessage(const std::string& message = "Invalid input! Try again.");
void summarizeSatelliteData(oct::Satellite satellite);
oct::SatelliteData printOrbitLoader(const oct::Satellite& satellite, int duration);
void summarizeSatelliteData(oct::Satellite satellite, oct::SatelliteData data);

int main(){
    printLogo();
    selectMeasurementSystem();

    std::string satelliteDataInputMode = selectSatelliteDataInputMode();
    oct::Satellite satellite = getSatelliteData(satelliteDataInputMode);
    std::cout << std::endl;

    oct::SatelliteData satelliteData = printOrbitLoader(satellite, 3);
    summarizeSatelliteData(satellite, satelliteData);

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
    measurementSystem = input;
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
    std::string message;

    if (input == "manual") {
        std::string name;
        double mass, velocity, altitude;
        std::string massUnit, velocityUnit, altitudeUnit;
        message = "Invalid input! ";

        if (measurementSystem == 1){
            massUnit = "kg";
            velocityUnit = "km/h";
            altitudeUnit = "km";
        }
        else if(measurementSystem == 2){
            massUnit = "lb";
            velocityUnit = "mph";
            altitudeUnit = "ft";
        } 
        else if (measurementSystem == 3){
            massUnit = "lb";
            velocityUnit = "kn";
            altitudeUnit = "ft";
        }
        
        printLogo();
        std::cout << "Enter satellite name: ";
        while (!(std::cin >> name)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage();
            std::cout << "Enter a valid name for the satellite: ";
        }

        printLogo();
        std::cout << "Enter satellite mass (" << massUnit << "): ";
        while (!(std::cin >> mass)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage(message);
            std::cout << "Enter a valid number for the mass (" << massUnit << "): ";
        }

        printLogo();
        std::cout << "Enter satellite velocity (" << velocityUnit << "): ";
        while (!(std::cin >> velocity)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage(message);
            std::cout << "Enter a valid number for the velocity (" << velocityUnit << "): ";
        }
        
        printLogo();
        std::cout << "Enter satellite altitude (" << altitudeUnit << "): ";
        while (!(std::cin >> altitude)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            printErrorMessage(message);
            std::cout << "Enter a valid number for altitude (" << altitudeUnit << "): ";
        }
        printLogo();

        if(measurementSystem == 1){
            return oct::Satellite(name, mass, velocity, altitude);
        }else if(measurementSystem == 2){
            mass = oct::UnitConverter::convertPoundsToKilograms(mass);
            velocity = oct::UnitConverter::convertMilesPerHourToKilometersPerHour(velocity);
            altitude = oct::UnitConverter::convertFeetToMeters(altitude);
            return oct::Satellite(name, mass, velocity, altitude);
        } else if(measurementSystem == 3){
            mass = oct::UnitConverter::convertPoundsToKilograms(mass);
            velocity = oct::UnitConverter::convertKnotsToKilometersPerHour(velocity);
            altitude = oct::UnitConverter::convertFeetToMeters(altitude);
            return oct::Satellite(name, mass, velocity, altitude);
        }

    }
    else if (input == "file") {
        std::string path;
        oct::Satellite satellite;
        bool loaded = false;

        printLogo();

        while (loaded == false) {
            std::cout << "Enter the path for the satellite json file: ";
            std::cin >> path;

            try {
                satellite = oct::FileManager::loadSatelliteFromFile(path);
                loaded = true;
            } catch (const std::exception& e) {
                message = e.what();
                printErrorMessage(message);
            }
        }

        return satellite;
    }
    
}

void printErrorMessage(const std::string& message){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
    std::cout << message;
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

oct::SatelliteData printOrbitLoader(const oct::Satellite& satellite, int duration){
    std::string frames = "|/-\\";
    int idx = 0;
    oct::SatelliteData data = satellite.calculateAll();
    auto begin = std::chrono::steady_clock::now();
    
    std::thread calculationThread([&]() {
        data = satellite.calculateAll();
    });

    while ((std::chrono::steady_clock::now() - begin) < std::chrono::seconds(duration)) {
        std::cout << "\rCalculating " << frames[idx++] << std::flush;
        idx %= 4;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    calculationThread.join(); 
    printLogo();

    return data;
}

void summarizeSatelliteData(oct::Satellite satellite, oct::SatelliteData data) {
    std::string massUnit, velocityUnit, altitudeUnit;
    std::string energyUnit = "J";
    std::string timeUnit = "s";

    if (measurementSystem == 1) {
        massUnit = "kg";
        velocityUnit = "km/h";
        altitudeUnit = "km";
    } else if (measurementSystem == 2) {
        massUnit = "lb";
        velocityUnit = "mph";
        altitudeUnit = "ft";
    } else if (measurementSystem == 3) {
        massUnit = "lb";
        velocityUnit = "kn";
        altitudeUnit = "ft";
    }

    std::cout << "\n+--------------------------+-------------------------+\n";
    std::cout << "|                 SATELLITE SUMMARY                  |\n";
    std::cout << "+--------------------------+-------------------------+\n";
    std::cout << "| Parameter                | Value                   |\n";
    std::cout << "+--------------------------+-------------------------+\n";

    std::cout << "| " << std::left << std::setw(25) << "Name"
              << "| " << std::left << std::setw(24) << satellite.name << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Mass"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.mass) + " " + massUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Altitude"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.altitude) + " " + altitudeUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Velocity"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.velocity) + " " + velocityUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Orbital Radius"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.orbitalRadius) + " " + altitudeUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Orbital Velocity"
              << "| " << std::left << std::setw(24) << (std::to_string(data.orbitalVelocity) + " " + velocityUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Orbital Period"
              << "| " << std::left << std::setw(24) << (std::to_string(data.orbitalPeriod) + " " + timeUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Mechanical Energy"
              << "| " << std::left << std::setw(24) << (std::to_string(data.mechanicalEnergy) + " " + energyUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Potential Energy"
              << "| " << std::left << std::setw(24) << (std::to_string(data.potentialEnergy) + " " + energyUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Kinetic Energy"
              << "| " << std::left << std::setw(24) << (std::to_string(data.kineticEnergy) + " " + energyUnit) << "|\n";

    std::cout << "| " << std::left << std::setw(25) << "Escape Velocity"
              << "| " << std::left << std::setw(24) << (std::to_string(data.escapeVelocity) + " " + velocityUnit) << "|\n";

    std::cout << "+-------------------------+--------------------------+\n";
}
