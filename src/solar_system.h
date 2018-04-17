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

class PlanetaryObject {
    PlanetaryObject(std::string name, float radius, float mass, bool is_sun, bool is_planet) : name(name), radius(radius), mass(mass), is_sun(is_sun), is_planet(is_planet) {
        
    }
    std::string getName() { return name; }
    const float getRadius() { return radius; }
    const float getMass() { return mass; }
    const bool isSun() { return is_sun; }
    const bool isPlanet() { return is_planet; }

private:
    std::string name;
    float radius;
    float mass;
    bool is_sun;
    bool is_planet;

    glm::vec4 position;
    glm::vec4 velocity;
    GLuint texture;
};

class SolarSystem {
public:
    SolarSystem() {}

    void generateSolPlanets();
    static void create_planetary_object(std::vector<glm::vec4>& planet_vertices, std::vector<glm::vec3>& planetary_faces);
private:
    std::vector<PlanetaryObject> planets;


};