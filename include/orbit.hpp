#pragma once

namespace oct {

class Orbit
{
public:
    double periapsis; // km
    double apoapsis; // km
    double inclination; // degrees

    Orbit(double periapsis, double apoapsis, double inclination);

    double semiMajorAxis() const;
    double eccentricity() const;
    double orbitalPeriod() const;
    double velocityAt(double radius) const;
};

}