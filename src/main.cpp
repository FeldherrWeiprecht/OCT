#include <iostream>
#include <string>

#include "satellite.hpp"
#include "constants.hpp"

void printLogo();
void selectMeasurementSystem();

int main(){
    printLogo();

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
