#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <exception>
#include <thread>
#include <chrono>
#include <iomanip>

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

int main() {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
   
    bool repeat = true;
    std::string message = "\nInvalid choice! Try again.\n";

    while (repeat) {
        printLogo();
        selectMeasurementSystem();

        std::string satelliteDataInputMode = selectSatelliteDataInputMode();
        oct::Satellite satellite = getSatelliteData(satelliteDataInputMode);
        std::cout << std::endl;

        oct::SatelliteData satelliteData = printOrbitLoader(satellite, 3);
        summarizeSatelliteData(satellite, satelliteData);

        std::string input;
        bool valid = false;

        while (valid == false) {
            std::cout << "\nDo you want to run the program again?" << std::endl;
            std::cout << "1. Yes" << std::endl;
            std::cout << "2. No" << std::endl;
            std::cout << "Enter your choice (1-2): ";

            std::getline(std::cin, input);

            if (input == "1") {
                repeat = true;
                valid = true;
            } else if (input == "2") {
                repeat = false;
                valid = true;
                return 0;
            } else {
                printLogo();
                printErrorMessage(message);
            }
        }
    }

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
    std::string input;
    bool valid = false;
    std::string message = "\nInvalid choice! Try again.\n\n";

    while (valid == false) {
        std::cout << "Select a measurement system:" << std::endl;
        std::cout << "1. SI (kilogram, kilometer, km/h)" << std::endl;
        std::cout << "2. Imperial (pound, feet, mph)" << std::endl;
        std::cout << "3. Nautical (pound, feet, kn)" << std::endl;
        std::cout << "Enter your choice (1-3): ";

        std::getline(std::cin, input);

        if (input == "1") {
            measurementSystem = 1;
            valid = true;
        } else if (input == "2") {
            measurementSystem = 2;
            valid = true;
        } else if (input == "3") {
            measurementSystem = 3;
            valid = true;
        } else {
            printLogo();
            printErrorMessage(message);
        }
    }

    printLogo();
}

std::string selectSatelliteDataInputMode() {
    std::string input;
    bool valid = false;
    std::string message = "\nInvalid choice! Try again.\n\n";

    while (valid == false) {
        std::cout << "Select satellite data input mode:" << std::endl;
        std::cout << "1. Manual input" << std::endl;
        std::cout << "2. Load from file" << std::endl;
        std::cout << "Enter your choice (1-2): ";

        std::getline(std::cin, input);

        if (input == "1") 
        {
            return "manual";
        }
        else if (input == "2")
        {
            return "file";
        }
        printLogo();
        printErrorMessage(message);
    }

    printLogo();

    return "manual";
}

oct::Satellite getSatelliteData(const std::string& input) {
    std::string message;
    std::string massUnit, velocityUnit, altitudeUnit;

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

    if (input == "manual") {
        std::string name, massString, velocityString, altitudeString;
        double mass, velocity, altitude;
        bool valid = false;

        printLogo();
        std::cout << "Enter satellite name: ";
        std::getline(std::cin, name);

        printLogo();
        while (valid == false) {
            std::cout << "Enter satellite mass (" << massUnit << "): ";
            std::getline(std::cin, massString);
            try {
                size_t pos;
                mass = std::stod(massString, &pos);
                if (pos == massString.size()) {
                    valid = true;
                } else {
                    throw std::invalid_argument("Wrong input.");
                }
            } catch (...) {
                printErrorMessage("\nInvalid input! Use only numbers. Use a dot '.' as decimal separator if needed.\n");
            }
        }

        printLogo();
        valid = false;
        while (valid == false) {
            std::cout << "Enter satellite velocity (" << velocityUnit << "): ";
            std::getline(std::cin, velocityString);
            try {
                size_t pos;
                velocity = std::stod(velocityString, &pos);
                if (pos == velocityString.size()) {
                    valid = true;
                } else {
                    throw std::invalid_argument("Wrong input.");
                }
            } catch (...) {
                printErrorMessage("\nInvalid input! Use only numbers. Use a dot '.' as decimal separator if needed.\n");
            }
        }

        printLogo();
        valid = false;
        while (valid == false) {
            std::cout << "Enter satellite altitude (" << altitudeUnit << "): ";
            std::getline(std::cin, altitudeString);
            try {
                size_t pos;
                altitude = std::stod(altitudeString, &pos);
                if (pos == altitudeString.size()) {
                    valid = true;
                } else {
                    throw std::invalid_argument("Wrong input.");
                }
            } catch (...) {
                printErrorMessage("\nInvalid input! Use only numbers. Use a dot '.' as decimal separator if needed.\n");
            }
        }

        printLogo();

        if (measurementSystem == 1) {
            return oct::Satellite(name, mass, velocity, altitude);
        } else if (measurementSystem == 2) {
            mass = oct::UnitConverter::convertPoundsToKilograms(mass);
            velocity = oct::UnitConverter::convertMilesPerHourToKilometersPerHour(velocity);
            altitude = oct::UnitConverter::convertFeetToMeters(altitude);
        } else if (measurementSystem == 3) {
            mass = oct::UnitConverter::convertPoundsToKilograms(mass);
            velocity = oct::UnitConverter::convertKnotsToKilometersPerHour(velocity);
            altitude = oct::UnitConverter::convertFeetToMeters(altitude);
        }

        return oct::Satellite(name, mass, velocity, altitude);
    }
    else if (input == "file") {
        std::string path;
        oct::Satellite satellite;
        bool loaded = false;

        printLogo();
        std::cout << "Expected file format (JSON) - see example below:\n"
            << "{\n"
            << "  \"name\": \"Telestar\",           <~ satellite name as text\n"
            << "  \"mass\": 1250.000,             <~ mass in " << massUnit << "\n"
            << "  \"velocity\": 27600.000,        <~ velocity in " << velocityUnit << "\n"
            << "  \"altitude\": 550.000           <~ altitude in " << altitudeUnit << "\n"
            << "}\n\n"
            << "Please enter the full file path:\n"
            << "  C:\\Users\\Username\\Desktop\\Data\\satellite.json\n\n"
            << "Or place the file in the same directory as the program and enter:\n"
            << "  satellite.json\n\n\n";


        while (loaded == false) {
            std::cout << "Enter the path for the satellite JSON file: ";
            std::getline(std::cin, path);

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

        satellite.mass = oct::UnitConverter::convertKilogramsToPounds(satellite.mass);
        satellite.velocity = oct::UnitConverter::convertKilometersPerHourToMilesPerHour(satellite.velocity);
        satellite.altitude = oct::UnitConverter::convertMetersToFeet(satellite.altitude);
        satellite.orbitalRadius = oct::UnitConverter::convertMetersToFeet(satellite.orbitalRadius);
        data.orbitalVelocity = oct::UnitConverter::convertKilometersPerHourToMilesPerHour(data.orbitalVelocity);
        data.escapeVelocity = oct::UnitConverter::convertKilometersPerHourToMilesPerHour(data.escapeVelocity);
    } else if (measurementSystem == 3) {
        massUnit = "lb";
        velocityUnit = "kn";
        altitudeUnit = "ft";

        satellite.mass = oct::UnitConverter::convertKilogramsToPounds(satellite.mass);
        satellite.velocity = oct::UnitConverter::convertKilometersPerHourToKnots(satellite.velocity);
        satellite.altitude = oct::UnitConverter::convertMetersToFeet(satellite.altitude);
        satellite.orbitalRadius = oct::UnitConverter::convertMetersToFeet(satellite.orbitalRadius);
        data.orbitalVelocity = oct::UnitConverter::convertKilometersPerHourToKnots(data.orbitalVelocity);
        data.escapeVelocity = oct::UnitConverter::convertKilometersPerHourToKnots(data.escapeVelocity);
    }

    std::cout << "\n+--------------------------+--------------------------+\n";
    std::cout << "|                 SATELLITE SUMMARY                   |\n";
    std::cout << "+--------------------------+--------------------------+\n";
    std::cout << "| Parameter                | Value                    |\n";
    std::cout << "+--------------------------+--------------------------+\n";

    std::cout << "| " << std::left << std::setw(25) << "Name"
              << "| " << std::left << std::setw(24) << satellite.name << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Mass"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.mass) + " " + massUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Altitude"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.altitude) + " " + altitudeUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Velocity"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.velocity) + " " + velocityUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Orbital Radius"
              << "| " << std::left << std::setw(24) << (std::to_string(satellite.orbitalRadius) + " " + altitudeUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Orbital Velocity"
              << "| " << std::left << std::setw(24) << (std::to_string(data.orbitalVelocity) + " " + velocityUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Orbital Period"
              << "| " << std::left << std::setw(24) << (std::to_string(data.orbitalPeriod) + " " + timeUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Mechanical Energy"
              << "| " << std::left << std::setw(24) << (std::to_string(data.mechanicalEnergy) + " " + energyUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Potential Energy"
              << "| " << std::left << std::setw(24) << (std::to_string(data.potentialEnergy) + " " + energyUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Kinetic Energy"
              << "| " << std::left << std::setw(24) << (std::to_string(data.kineticEnergy) + " " + energyUnit) << " |\n";

    std::cout << "| " << std::left << std::setw(25) << "Escape Velocity"
              << "| " << std::left << std::setw(24) << (std::to_string(data.escapeVelocity) + " " + velocityUnit) << " |\n";

    std::cout << "+--------------------------+--------------------------+\n";
}
