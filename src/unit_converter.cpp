#include "unit_converter.hpp"

namespace oct {

    double UnitConverter::convertFeetToMeters(double feet) {
        return feet * 0.3048;
    }

    double UnitConverter::convertPoundsToKilograms(double pounds) {
        return pounds * 0.45359237;
    }

    double UnitConverter::convertMilesPerHourToKilometersPerHour(double mph) {
        return mph * 1.609344;
    }

    double UnitConverter::convertMetersToFeet(double meters) {
        return meters / 0.3048;
    }

    double UnitConverter::convertKilogramsToPounds(double kilograms) {
        return kilograms / 0.45359237;
    }

    double UnitConverter::convertKilometersPerHourToMilesPerHour(double kph) {
        return kph / 1.609344;
    }

    double UnitConverter::convertKnotsToKilometersPerHour(double knots) {
        return knots * 1.852;
    }

    double UnitConverter::convertKilometersPerHourToKnots(double kph) {
        return kph / 1.852;
    }
}
