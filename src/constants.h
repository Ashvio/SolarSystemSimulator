#pragma once
#include <string>

//Non mathematical constants
//MASS in kg
//DIAMETER in km

//Mathematical constants

const double G = 6.6738e-11; //N-m^2/kg^2 Universal Gravitational Constant
const double PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986;
const double AU = 149597870;    //kilometers
const double SCALE_FACTOR = 10000.0; //kilometers to gui
const double DEG_TO_RADS = PI/180.0;

const std::string months[12] = {"January", "February", "March", "April", "May", "June",  "July", "August", "September", "October", "November", "December"};

static const unsigned int num_days_month[12] = {31, 28, 31, 30, 31, 30, 31, 30, 31, 31, 30, 31};

