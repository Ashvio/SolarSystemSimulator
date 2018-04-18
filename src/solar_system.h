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
public:
    PlanetaryObject(std::string name, float radius, float mass, bool is_sun, bool is_planet) : name(name), radius(radius), mass(mass), is_sun(is_sun), is_planet(is_planet) {
        
    }
    std::string getName() { return name; }
    const float getRadius() { return radius; }
    const float getMass() { return mass; }
    const bool isSun() { return is_sun; }
    const bool isPlanet() { return is_planet; }
    void setPosition(glm::vec4 pos) { position = pos; }
    void setTexture(GLuint text) { texture = text; };
    
    float radius;
private:
    std::string name;
    
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
    int numPlanets() { return planets.size(); }

    std::vector<PlanetaryObject> planets;
private:
    


};