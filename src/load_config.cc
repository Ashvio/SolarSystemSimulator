#include "json.hpp"
#include "solar_system.h"
#include <fstream>


using json = nlohmann::json;

PlanetaryObject SolarSystem::loadPlanetFromConfig(std::string config_file) {
    std::cout << "Processing " + config_file + "\n";
    std::ifstream i(config_file);
    json j;
    i >> j;
    
    std::string name = j.at("planet");

    float diameter = j.at("diameter");
    float mass = j.at("mass");
    std::string texture_name = j.at("texture");
    bool hasBcsf = j.at("hasBcsf");
    float r = j.at("color").at("r");
    float g = j.at("color").at("g");
    float b = j.at("color").at("b");
    glm::vec3 color (r, g, b);
    bool is_sun = name == "Sun";

    
    OrbitalElements start_elements;
    start_elements.a = j.at("elements").at("start").at("a");
    start_elements.e = j.at("elements").at("start").at("e");
    start_elements.I = j.at("elements").at("start").at("I");
    start_elements.L = j.at("elements").at("start").at("L");
    start_elements.lp = j.at("elements").at("start").at("lp");
    start_elements.ln = j.at("elements").at("start").at("ln");
    if (hasBcsf) {
        start_elements.b = j.at("elements").at("start").at("b");
        start_elements.c = j.at("elements").at("start").at("c");
        start_elements.s = j.at("elements").at("start").at("s");
        start_elements.f = j.at("elements").at("start").at("f");        
    }
    
    OrbitalElements diff_elements;
    diff_elements.a = j.at("elements").at("diff").at("a");
    diff_elements.e = j.at("elements").at("diff").at("e");
    diff_elements.I = j.at("elements").at("diff").at("I");
    diff_elements.L = j.at("elements").at("diff").at("L");
    diff_elements.lp = j.at("elements").at("diff").at("lp");
    diff_elements.ln = j.at("elements").at("diff").at("ln");

    PlanetaryObject planet(name, diameter, mass, is_sun, !is_sun, start_elements, diff_elements, texture_name, hasBcsf, color);
    return planet;
}
