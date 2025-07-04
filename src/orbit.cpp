#include "orbit.hpp"
#include "constants.hpp"
#include <iostream>
#include <cmath>

namespace oct{
    Orbit::Orbit(double periapsis, double apoapsis, double inclination){
        this->periapsis = periapsis;
        this->apoapsis = apoapsis;
        this->inclination = inclination;
    }

    double Orbit::semiMajorAxis() const {
        double result = (periapsis + apoapsis) / 2.0;
        std::cout << result << std::endl;
        return result;
    }

    double Orbit::eccentricity() const {
        double result = (apoapsis - periapsis) / (apoapsis + periapsis);
        std::cout << result << std::endl;
        return result;
    }

    double Orbit::orbitalPeriod() const {
        double x = semiMajorAxis() * 1000;;
        double result = 2 * PI * sqrt(pow(x, 3) / (GRAVITY_CONSTANT * EARTH_MASS));
        std::cout << result << std::endl;
        return result;
    }

    double Orbit::velocityAt(double radius) const {
        radius *= 1000;
        double x = semiMajorAxis() * 1000;
        double result = sqrt(GRAVITY_CONSTANT * EARTH_MASS * (2.0 / radius - 1.0 / x));
        std::cout << result << std::endl;
        return result;
    }
}

