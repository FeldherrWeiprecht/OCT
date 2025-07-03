#pragma once

namespace oct{

class UnitConverter {
public:
    static double convertFeetToMeters(double feet);
    static double convertPoundsToKilograms(double pounds);
    static double convertMilesPerHourToKilometersPerHour(double mph);
    static double convertMetersToFeet(double meters);
    static double convertKilogramsToPounds(double kilograms);
    static double convertKilometersPerHourToMilesPerHour(double kph);
    static double convertKnotsToKilometersPerHour(double knots);
    static double convertKilometersPerHourToKnots(double kph);
};

}
