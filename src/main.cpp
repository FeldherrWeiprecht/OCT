#include <iostream>
#include <string>

#include "satellite.hpp"
#include "constants.hpp"

void printLogo();

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
