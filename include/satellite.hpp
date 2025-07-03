#pragma once

#include <string>

namespace oct {
    
class Satellite {
public:
    std::string name;
    double mass; // kg
    double velocity; // km/h
    double altitude; // km
    double orbitalRadius; // km

    Satellite(std::string& name, double mass, double velocity, double altitude);

    double orbitalVelocity() const;
    double orbitalPeriod() const;
    double mechanicalEnergy() const;
    double potentialEnergy() const;
    double kineticEnergy() const;
    double escapeVelocity() const;
};

}
