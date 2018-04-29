#pragma once


#include "gui.h"
#include "config.h"
#include "constants.h"
#include "../lib/utgraphicsutil/image.h"
#include "../lib/utgraphicsutil/jpegio.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "physics.h"

class PlanetaryObject {
public:
    PlanetaryObject(std::string name, float diameter, float mass, bool is_sun, bool is_planet, OrbitalElements start_elements, OrbitalElements diff_elements, bool hasBcsf) 
    : name(name), diameter(diameter), mass(mass), is_sun(is_sun), is_planet(is_planet), start_elements(start_elements), diff_elements(diff_elements), has_bcsf(hasBcsf) {
        
    }
    std::string getName() { return name; }
    const float* getDiameter() { return &diameter; }
    const float getMass() { return mass; }
    const bool isSun() { return is_sun; }
    const bool isPlanet() { return is_planet; }
    void setPosition(glm::vec4 pos) { position = pos; }
    void setTexture(GLuint text) { texture = text; };
    Image* loadImage(const std::string& file_name) {
		Image *im = new Image();
		if (!LoadJPEG(file_name, im)) {
			return NULL;
		}
		return im;
    }

private:
    std::string name;
    float diameter;
    float mass;
    OrbitalElements start_elements;
    OrbitalElements diff_elements;
    bool is_sun;
    bool is_planet;
    bool has_bcsf = false;

    glm::vec4 position;
    glm::vec4 velocity;
    GLuint texture;
};

class SolarSystem {
public:
    SolarSystem() {}

    void generateSolPlanets();
    PlanetaryObject loadPlanetFromConfig(std::string config_file);
    static void create_planetary_object(std::vector<glm::vec4>& planet_vertices, std::vector<glm::vec3>& planetary_faces);
    int numPlanets() { return planets.size(); }

    std::vector<PlanetaryObject> planets;
private:
    std::string sol_planets[8] = {"mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune"};


};