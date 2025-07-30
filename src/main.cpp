#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include <thread>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <csignal>

#include "satellite.hpp"
#include "constants.hpp"
#include "file_manager.hpp"
#include "unit_converter.hpp"

int measurementSystem = 1;

void clearScreen();
void printLogo();
void selectMeasurementSystem();
std::string selectSatelliteDataInputMode();
oct::Satellite getSatelliteData(const std::string& input = "manual");
void printErrorMessage(const std::string& message);
void summarizeSatelliteData(oct::Satellite satellite, oct::SatelliteData data);
oct::SatelliteData printOrbitLoader(const oct::Satellite& satellite, int duration);

void resetTerminalColor(int signum);

int main() {
    std::signal(SIGINT, resetTerminalColor);

    bool repeat = true;
    std::string errorMessage = "";

    while (repeat) {
        printLogo();

        selectMeasurementSystem();

        std::string satelliteDataInputMode = selectSatelliteDataInputMode();

        oct::Satellite satellite = getSatelliteData(satelliteDataInputMode);

        std::cout << std::endl;

        oct::SatelliteData satelliteData = printOrbitLoader(satellite, 3);

        summarizeSatelliteData(satellite, satelliteData);

        bool validSaveChoice = false;

        while (!validSaveChoice) {
            if (errorMessage != "") {
                clearScreen();
                printLogo();
                printErrorMessage(errorMessage);
                errorMessage.clear();
            }

            std::cout << "\nDo you want to save calculation results as JSON?" << std::endl;
            std::cout << "1. Yes" << std::endl;
            std::cout << "2. No" << std::endl;
            std::cout << "Enter your choice (1-2): ";

            std::string saveInput;
            std::getline(std::cin, saveInput);

            if (saveInput == "1") {
                bool validPath = false;

                while (!validPath) {
                    clearScreen();
                    printLogo();

                    std::cout << "\nEnter file path and name to save results as JSON (example: results.json): ";

                    std::string pathInput;
                    std::getline(std::cin, pathInput);

                    if (pathInput.empty()) {
                        errorMessage = "Invalid input! It has to be a valid file path.\n";
                        break;
                    }

                    std::ofstream file(pathInput.c_str());

                    if (!file.is_open()) {
                        errorMessage = "Invalid input! It has to be a valid file path.\n";
                        break;
                    }

                    file << "{\n";
                    file << "  \"name\": \"" << satellite.name << "\",\n";
                    file << "  \"mass\": " << satellite.mass << ",\n";
                    file << "  \"velocity\": " << satellite.velocity << ",\n";
                    file << "  \"altitude\": " << satellite.altitude << ",\n";
                    file << "  \"orbital_radius\": " << satellite.orbitalRadius << ",\n";
                    file << "  \"orbital_velocity\": " << satelliteData.orbitalVelocity << ",\n";
                    file << "  \"orbital_period\": " << satelliteData.orbitalPeriod << ",\n";
                    file << "  \"mechanical_energy\": " << satelliteData.mechanicalEnergy << ",\n";
                    file << "  \"potential_energy\": " << satelliteData.potentialEnergy << ",\n";
                    file << "  \"kinetic_energy\": " << satelliteData.kineticEnergy << ",\n";
                    file << "  \"escape_velocity\": " << satelliteData.escapeVelocity << "\n";
                    file << "}\n";

                    file.close();

                    std::cout << "\n\nResults successfully saved as " << pathInput << "\n\n";

                    validPath = true;
                    validSaveChoice = true;
                    errorMessage.clear();
                }
            } else if (saveInput == "2") {
                validSaveChoice = true;
            } else {
                errorMessage = "Invalid input! Please enter 1 or 2.\n";
            }
        }

        bool validRepeatChoice = false;

        errorMessage.clear();

        while (!validRepeatChoice) {
            if (errorMessage != "") {
                clearScreen();
                printLogo();
                printErrorMessage(errorMessage);
                errorMessage.clear();
            }

            std::cout << "\nDo you want to run the program again?" << std::endl;
            std::cout << "1. Yes" << std::endl;
            std::cout << "2. No" << std::endl;
            std::cout << "Enter your choice (1-2): ";

            std::string input;
            std::getline(std::cin, input);

            if (input == "1") {
                repeat = true;
                validRepeatChoice = true;
            } else if (input == "2") {
                repeat = false;
                std::cout << "\033[0m";
                return 0;
            } else {
                errorMessage = "Invalid input! Please enter 1 or 2.\n";
            }
        }
    }

    return 0;
}

void resetTerminalColor(int signum) {
    (void)signum;
    std::cout << "\033[0m" << std::endl;
    std::exit(signum);
}

void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void printLogo() {
    clearScreen();

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
    std::string errorMessage = "\nInvalid choice! Try again.\n\n";

    while (!valid) {
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
            printErrorMessage(errorMessage);
        }
    }

    printLogo();
}

std::string selectSatelliteDataInputMode() {
    std::string input;
    bool valid = false;
    std::string errorMessage = "\nInvalid choice! Try again.\n\n";

    while (!valid) {
        std::cout << "Select satellite data input mode:" << std::endl;
        std::cout << "1. Manual input" << std::endl;
        std::cout << "2. Load from file" << std::endl;
        std::cout << "Enter your choice (1-2): ";

        std::getline(std::cin, input);

        if (input == "1") {
            return "manual";
        } else if (input == "2") {
            return "file";
        } else {
            printLogo();
            printErrorMessage(errorMessage);
        }
    }

    printLogo();

    return "manual";
}

oct::Satellite getSatelliteData(const std::string& input) {
    std::string message;
    std::string massUnit;
    std::string velocityUnit;
    std::string altitudeUnit;

    if (measurementSystem == 1) {
        massUnit = "kg";
        velocityUnit = "km/h";
        altitudeUnit = "km";
    } else if (measurementSystem == 2) {
        massUnit = "lb";
        velocityUnit = "mph";
        altitudeUnit = "ft";
    } else {
        massUnit = "lb";
        velocityUnit = "kn";
        altitudeUnit = "ft";
    }

    if (input == "manual") {
        std::string name;
        std::string massString;
        std::string velocityString;
        std::string altitudeString;
        double mass = 0.0;
        double velocity = 0.0;
        double altitude = 0.0;
        bool valid = false;

        printLogo();
        std::cout << "Enter satellite name: ";
        std::getline(std::cin, name);

        printLogo();

        while (!valid) {
            std::cout << "Enter satellite mass (" << massUnit << "): ";
            std::getline(std::cin, massString);

            try {
                size_t pos = 0;
                mass = std::stod(massString, &pos);

                if (pos == massString.size()) {
                    valid = true;
                } else {
                    throw std::invalid_argument("Wrong input.");
                }
            } catch (...) {
                printErrorMessage("\nInvalid input! Only numbers are allowed. Use a dot '.' as decimal separator if needed. Example: 1250.000\n");
            }
        }

        printLogo();

        valid = false;

        while (!valid) {
            std::cout << "Enter satellite velocity (" << velocityUnit << "): ";
            std::getline(std::cin, velocityString);

            try {
                size_t pos = 0;
                velocity = std::stod(velocityString, &pos);

                if (pos == velocityString.size()) {
                    valid = true;
                } else {
                    throw std::invalid_argument("Wrong input.");
                }
            } catch (...) {
                printErrorMessage("\nInvalid input! Only numbers are allowed. Use a dot '.' as decimal separator if needed. Example: 27600.000\n");
            }
        }

        printLogo();

        valid = false;

        while (!valid) {
            std::cout << "Enter satellite altitude (" << altitudeUnit << "): ";
            std::getline(std::cin, altitudeString);

            try {
                size_t pos = 0;
                altitude = std::stod(altitudeString, &pos);

                if (pos == altitudeString.size()) {
                    valid = true;
                } else {
                    throw std::invalid_argument("Wrong input.");
                }
            } catch (...) {
                printErrorMessage("\nInvalid input! Only numbers are allowed. Use a dot '.' as decimal separator if needed. Example: 550.000\n");
            }
        }

        printLogo();

        if (measurementSystem == 1) {
            return oct::Satellite(name, mass, velocity, altitude);
        } else if (measurementSystem == 2) {
            mass = oct::UnitConverter::convertPoundsToKilograms(mass);
            velocity = oct::UnitConverter::convertMilesPerHourToKilometersPerHour(velocity);
            altitude = oct::UnitConverter::convertFeetToMeters(altitude);
        } else {
            mass = oct::UnitConverter::convertPoundsToKilograms(mass);
            velocity = oct::UnitConverter::convertKnotsToKilometersPerHour(velocity);
            altitude = oct::UnitConverter::convertFeetToMeters(altitude);
        }

        return oct::Satellite(name, mass, velocity, altitude);
    } else if (input == "file") {
        std::string path;
        oct::Satellite satellite;
        bool loaded = false;

        printLogo();

        std::cout << "Expected file format (JSON) - see example below:\n";
        std::cout << "{\n";
        std::cout << "  \"name\": \"ISS\",\n";
        std::cout << "  \"mass\": 419725,\n";
        std::cout << "  \"velocity\": 27600,\n";
        std::cout << "  \"altitude\": 408\n";
        std::cout << "}\n\n";

        std::cout << "Please enter the full file path:\n";
        std::cout << "  C:\\Users\\Username\\Desktop\\Data\\iss.json\n\n";
        std::cout << "Or place the file in the same directory as the program and enter:\n";
        std::cout << "  iss.json\n\n\n";

        while (!loaded) {
            std::cout << "Enter the path for the satellite JSON file: ";
            std::getline(std::cin, path);

            try {
                satellite = oct::FileManager::loadSatelliteFromFile(path);
                loaded = true;
            } catch (const std::exception& e) {
                std::string message = e.what();
                printErrorMessage(message);
            }
        }

        return satellite;
    }

    return oct::Satellite();
}

void printErrorMessage(const std::string& message) {
    std::cout << "\033[1;31m" << message << "\033[0m";
}

oct::SatelliteData printOrbitLoader(const oct::Satellite& satellite, int duration) {
    std::string frames = "|/-\\";
    int idx = 0;
    oct::SatelliteData data = satellite.calculateAll();
    auto begin = std::chrono::steady_clock::now();

    std::thread calculationThread([&]() {
        data = satellite.calculateAll();
    });

    while ((std::chrono::steady_clock::now() - begin) < std::chrono::seconds(duration)) {
        std::cout << "\rCalculating " << frames[idx] << std::flush;
        idx++;

        if (idx >= 4) {
            idx = 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    calculationThread.join();
    printLogo();

    return data;
}

void summarizeSatelliteData(oct::Satellite satellite, oct::SatelliteData data) {
    std::string massUnit;
    std::string velocityUnit;
    std::string altitudeUnit;
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
    } else {
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
