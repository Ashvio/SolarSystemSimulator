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

struct OrbitalElements {
    double a;
    double e;
    double I;
    double L;
    double lp;
    double ln;
};

class PlanetaryObject {
public:
    PlanetaryObject(std::string name, float diameter, float mass, bool is_sun, bool is_planet, OrbitalElements start_elements, OrbitalElements diff_elements) 
    : name(name), diameter(diameter), mass(mass), is_sun(is_sun), is_planet(is_planet), start_elements(start_elements), diff_elements(diff_elements) {
        renderRadius = diameter / 2.0f / 100.0f;
    }
    PlanetaryObject(std::string name, float diameter, float mass, bool is_sun, bool is_planet) : name(name), diameter(diameter), mass(mass), is_sun(is_sun), is_planet(is_planet) {
        renderRadius = diameter / 2.0f / 100.0f;
    }
    std::string getName() { return name; }
    const float* getDiameter() { return &diameter; }
    const float getMass() { return mass; }
    const bool isSun() { return is_sun; }
    const bool isPlanet() { return is_planet; }
    void setPosition(glm::vec4 pos) { position = pos; }

    static Image* loadImage(const std::string& file_name) {
		Image *im = new Image();
		if (!LoadJPEG(file_name, im)) {
			return NULL;
		}
		return im;
    }
    void loadTexture(const std::string& file_name);

    float renderRadius;
    GLuint texture;
private:
    std::string name;
    float diameter;
    float mass;
    OrbitalElements start_elements;
    OrbitalElements diff_elements;
    bool is_sun;
    bool is_planet;

    glm::vec4 position;
    glm::vec4 velocity;
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