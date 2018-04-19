#include "solar_system.h"

void SolarSystem::generateSolPlanets() {
    // std::string name, float radius, float mass, bool is_sun, bool is_planet
    PlanetaryObject planet = PlanetaryObject("Neptune", 20.0f, 100.0f, false, true);
    planets.push_back(planet);
}