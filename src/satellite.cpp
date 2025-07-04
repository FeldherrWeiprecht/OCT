#include <iostream>
#include "satellite.hpp"
#include "constants.hpp"

namespace oct {
    Satellite::Satellite(){
        this->name = "";
        this->mass = mass;
        this->velocity = 0;
        this->altitude = 0;
        this->orbitalRadius = 0;
    }

    Satellite::Satellite(const std::string& name, double mass, double velocity, double altitude){
        this->name = name;
        this->mass = mass;
        this->velocity = velocity;
        this->altitude = altitude;
        this->orbitalRadius = (EARTH_RADIUS+altitude)*1000;
    }
    
    double Satellite::orbitalVelocity() const{
        double result = sqrt((GRAVITY_CONSTANT*EARTH_MASS)/orbitalRadius);
        std::cout << result << std::endl;
        return result;
    }

    double Satellite::orbitalPeriod() const{
        double result = 2*PI*sqrt(pow(orbitalRadius, 3) / (GRAVITY_CONSTANT*EARTH_MASS));
        std::cout << result << std::endl;
        return result;
    }
    
    double Satellite::mechanicalEnergy() const {
        double result = -((GRAVITY_CONSTANT*EARTH_MASS*mass)/(2*orbitalRadius));
        std:: cout << result << std::endl;
        return result;
    }

    double Satellite::potentialEnergy() const{
        double result = -((GRAVITY_CONSTANT*EARTH_MASS*mass)/orbitalRadius);
        std::cout << result << std::endl;
        return result;
    }

    double Satellite::kineticEnergy() const{
        double result = 0.5*mass*pow(orbitalVelocity(), 2);
        std::cout << result << std::endl;
        return result;
    }

    double Satellite::escapeVelocity() const{
        double result = sqrt((2*GRAVITY_CONSTANT*EARTH_MASS)/orbitalRadius);
        std::cout << result << std::endl;
        return result;
    }
}