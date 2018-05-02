#pragma once

#include "gui.h"
#include "config.h"
#include "constants.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

class Gravity {

    //Universal Gravitation Formula http://www.physicsclassroom.com/class/circles/Lesson-3/Newton-s-Law-of-Universal-Gravitation
    static glm::dvec3 gravityBetween(double mass1, double mass2, glm::dvec3 pos1, glm::dvec3 pos2) {
        glm::dvec3 diff_vect = pos1 - pos2;
        double distance_diff = glm::length(diff_vect);

        double denom = std::pow(distance_diff, 2.0);
        double numerator = mass1 * mass2;
        double scalar_force = G * (numerator/denom);
        glm::dvec3 vector_force = scalar_force * glm::normalize(diff_vect);
        return vector_force;
    }

};



class Date {
    public:  
    Date(unsigned int year, unsigned int month, double day) {
        setDate(year, month, day);
    }

    void setDate(unsigned int year, unsigned int month, double day) {
        if (!checkValidDate(year, month, day)) {
            return;
        }
        else {
            this->year = year;
            this->month = month;
            this->day = day;
        }
    }

    void incrementDate() {
        double new_date = day + 10;
        unsigned int new_month = month;
        unsigned int new_year = year;
        if (new_date > getNumDaysMonth(month, year)) {
            new_date = 1;
            new_month = month + 1;
            if (new_month > 12) {
                new_month = 1;
                new_year = year + 1;
            }
        }
        setDate(new_year, new_month, new_date);
    }


    void decrementDate() {
        double new_date = day - 10;
        unsigned int new_month = month;
        unsigned int new_year = year;
        if (new_date < 1 ) {
            new_month = month - 1;
            if (new_month < 1) {
                new_month = 12;
                new_year = year - 1;
            }
            new_date = getNumDaysMonth(new_month, new_year);
        }
        setDate(new_year, new_month, new_date);
    }
    const std::string getDate() {
        std::string date = months[month - 1] + " " + std::to_string(day) + ", " + std::to_string(year);
        return date;
    }
    const bool checkValidDate(unsigned int year, unsigned int month, unsigned int day) {
        if (month > 12) {
            std::cout << "Invalid month " << month << std::endl;
            return false;
        }
        unsigned int num_days = getNumDaysMonth(month, year);
        if (day > num_days) {
            
            std::cout << "Invalid days" << day << std::endl;
            return false;
        }
        return true;
    }
    static unsigned int getNumDaysMonth(unsigned int month, unsigned int year) { 
        if (month == 2) {
            bool leap = false;
            
            //https://en.wikipedia.org/wiki/Leap_year#Algorithm
            if (year % 4 != 0) {
                leap = false;
            } else if (year % 100 != 0) {
                leap = true;
            } else if (year % 400 != 0) {
                leap = false;
            } else {
                leap = true;
            }
            if (leap) return 29;
            else return 28;
        } else {
            return num_days_month[month - 1];
        }
    }

    const double getTotalDays() {
        double total = 0;
        for (unsigned int m = 0; m < month - 1; m++) {
            total += getNumDaysMonth(m + 1, year);
        }
        total += day;
        return total;
    }
    const double getCenturiesPastJ2000() {
        double num_years_since_J2000 = (double) ((int)year - 2000);

        double num_days_since_J2000 = num_years_since_J2000 * YEAR_DAYS + getTotalDays();

        return num_days_since_J2000 / (YEAR_DAYS*100);
    }
private:
    unsigned int year;
    unsigned int month;
    double day;

    static constexpr double julian_date_J2000 = 2451545.0;
    static constexpr double YEAR_DAYS =  365.2425;
};


    


class Orbit {
    
    static glm::dvec3 getPositionAtDate();
};

//Keplerian Orbital Elements
struct OrbitalElements {
    double a;   //Semi-major axis
    double e;   //Eccentricity
    double I;   //Inclination
    double L;   //Mean longitude
    double lp;  //Longitude of perihelion (Small omega with a hat)
    double ln;  //Longitude of the ascending node (Big omega)

    bool is_current = false;

    //only for Jupiter-Pluto
    double b = 0;
    double c = 0;
    double s = 0;
    double f = 0;

    double obliquity = 23.43928;

    // https://ssd.jpl.nasa.gov/txt/aprx_pos_planets.pdf MATH!!!
    //Step 1
    static OrbitalElements computeCurrentElements(OrbitalElements start_elements, OrbitalElements diff_elements, bool has_bcsf, double centuries_past_J2000) {
        OrbitalElements current_elements;
        current_elements.a = start_elements.a + diff_elements.a * centuries_past_J2000;
        current_elements.e = start_elements.e + diff_elements.e * centuries_past_J2000;
        current_elements.I = start_elements.I + diff_elements.I * centuries_past_J2000;
        current_elements.L = start_elements.L + diff_elements.L * centuries_past_J2000;
        current_elements.lp = start_elements.lp + diff_elements.lp * centuries_past_J2000;
        current_elements.ln = start_elements.ln + diff_elements.ln * centuries_past_J2000;
        if (has_bcsf) {
            current_elements.b = start_elements.b;
            current_elements.c = start_elements.c;
            current_elements.s = start_elements.s;
            current_elements.f = start_elements.f;            
        }
        current_elements.is_current = true;
        return current_elements;
    }
    void checkCurrent() {
        if (!is_current) {
            std::cerr << "WARNING: Orbital Elements are not current\n";
        }
    }
    //Step 2
    void computePerihelionAndMeanAnomaly(double& perihelion, double& mean_anomaly, bool has_bcsf, double centuries_past_J2000) {
        checkCurrent();
        perihelion = lp - ln;
        mean_anomaly = L - lp;
        double T = centuries_past_J2000;
        if (has_bcsf) {
           mean_anomaly += b * pow(T, 2.0) + c * cos(DEG_TO_RADS * f * T) + s * sin(DEG_TO_RADS * f * T);
        }
    }
    //Step 3 Solve Kepler's Equation!!
    void computeEccentricAnomaly(double mean_anomaly, double& eccentric_anomaly, int iterations) {
        checkCurrent();        
        double M = fmod(mean_anomaly, 360.0) - 180.0;
        double e_degree = (1.0/DEG_TO_RADS) * e;
        double e_n = M + e_degree * sin(DEG_TO_RADS * M);
        for (int n = 0; n < iterations; n++) {
            double delta_M = M - (e_n - e_degree * sin(DEG_TO_RADS * e_n));
            double delta_E = delta_M / (1.0 - e * cos(DEG_TO_RADS * e_n));
            e_n = e_n + delta_E;
        }
        eccentric_anomaly = e_n;
    }
    //Step 4
    void computeHeliocentricCoordinates(double eccentric_anomaly, glm::dvec3& heliocentric_coords) {
        checkCurrent();
        double E = eccentric_anomaly;
        double x = a * (cos(DEG_TO_RADS * E) - e);
        double y = a * sqrt(1 - pow(e, 2.0)) * sin(DEG_TO_RADS * E);
        double z = 0.0;
        heliocentric_coords[0] = x;
        heliocentric_coords[1] = y;
        heliocentric_coords[2] = z;
    }

    //Step 5
    void computeEclipticCoordinates(glm::dvec3 heliocentric_coords, double perihelion, glm::dvec3& ecliptic_coords) {
        checkCurrent();
        double x = heliocentric_coords[0];
        double y = heliocentric_coords[1];
        double z = heliocentric_coords[2];
        double sin_peri = sin(DEG_TO_RADS * perihelion);
        double sin_ln = sin(DEG_TO_RADS * ln);
        double cos_peri = cos(DEG_TO_RADS * perihelion);
        double cos_ln = cos(DEG_TO_RADS * ln);
        double cos_I = cos(DEG_TO_RADS * I);
        double sin_I = sin(DEG_TO_RADS * I);
        ecliptic_coords[0] = (cos_peri * cos_ln - sin_peri * sin_ln * cos_I) * x 
                             + (-sin_peri*cos_ln - cos_peri * sin_ln * cos_I) * y;
        ecliptic_coords[1] = (cos_peri * sin_ln + sin_peri * cos_ln * cos_I) * x
                             + (-sin_peri * sin_ln + cos_peri * cos_ln * cos_I) * y;
        ecliptic_coords[2] = (sin_peri * sin_I) * x + (cos_peri * sin_I) * y;
        
    }

    //Step 6
    void computeEquatorialCoordinates(glm::dvec3 ecliptic_coords, glm::dvec3& equatorial_coords) {
        checkCurrent();
        double x = ecliptic_coords[0];
        double y = ecliptic_coords[1];
        double z = ecliptic_coords[2];

        double sin_o = sin(DEG_TO_RADS * obliquity);
        double cos_o = cos(DEG_TO_RADS * obliquity);
        equatorial_coords[0] = x;
        equatorial_coords[1] = cos_o * y - sin_o * z;
        equatorial_coords[2] = sin_o * y - cos_o * z;
    }
};
