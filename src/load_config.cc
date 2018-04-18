#include "json.hpp"
#include "solar_system.h"
#include <fstream>


using json = nlohmann::json;

PlanetaryObject loadPlanetFromConfig(std::string config_file) {
    std::ifstream i(config_file);
    json j;
    i >> j;
    
    std::string name = j.at("planet");
    float diameter = j.at("diameter");
    float mass = j.at("mass");
    OrbitalElements start_elements;
    start_elements.a = j.at("start").at("a");
    start_elements.e = j.at("start").at("e");
    start_elements.I = j.at("start").at("I");
    start_elements.L = j.at("start").at("L");
    start_elements.lp = j.at("start").at("lp");
    start_elements.ln = j.at("start").at("ln");
    
    OrbitalElements diff_elements;
    diff_elements.a = j.at("diff").at("a");
    diff_elements.e = j.at("diff").at("e");
    diff_elements.I = j.at("diff").at("I");
    diff_elements.L = j.at("diff").at("L");
    diff_elements.lp = j.at("diff").at("lp");
    diff_elements.ln = j.at("diff").at("ln");

    PlanetaryObject planet(name, diameter, mass, false, true, start_elements, diff_elements);
    return planet;
}
