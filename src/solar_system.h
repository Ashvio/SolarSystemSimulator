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
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include "physics.h"

class PlanetaryObject {
public:
    PlanetaryObject(std::string name, float diameter, float mass, bool is_sun, bool is_planet, 
    OrbitalElements start_elements, OrbitalElements diff_elements, std::string texture_name, bool hasBcsf, glm::vec4 color, int orbitPeriod) 
    : name(name), diameter(diameter), mass(mass), is_sun(is_sun), is_planet(is_planet), start_elements(start_elements), 
    diff_elements(diff_elements), has_bcsf(hasBcsf), color(color), orbitPeriod(orbitPeriod) {
        loadTexture(texture_name);
        renderRadius =  diameter / 2.0f / SCALE_FACTOR;
        if (!is_sun) renderRadius *= 28.0;
        position = glm::vec4(0.0, 0.0, 0.0, 1.0);
    }

    PlanetaryObject() {
        renderRadius = 1.0;
        position = glm::vec4(0.0, 0.0, 0.0, 1.0);
    }
    std::string getName() { return name; }
    const float* getDiameter() { return &diameter; }
    const glm::dvec4* getPosition() {
        return &position;
    }
    const glm::dvec4* getLastPosition() {
        return &last_position;
    }
    const float getMass() { return mass; }
    const bool isSun() { return is_sun; }
    const bool isPlanet() { return is_planet; }
    void setPosition(glm::dvec4 pos) { position = pos; }
    void setLastPosition() { last_position = position; }

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
    OrbitalElements start_elements;
    OrbitalElements diff_elements;
    bool has_bcsf = false;
    glm::vec4 color;
private:
    std::string name;
    float diameter;
    float mass;
    bool is_sun;
    bool is_planet;
    int orbitPeriod;

    glm::dvec4 position;
    glm::dvec4 velocity;
    glm::dvec4 accel;
    glm::dvec4 last_position;
    
};

class SolarSystem {
public:
    SolarSystem() : current_date(2000, 1, 1), sun() {
    
    }

    void generateSolPlanets();
    void generateSolPlanetPositions();
    void updateSolPlanetPositionsGravity();
    glm::dvec4 getSolPlanetPosition(PlanetaryObject *planet, double centuries_past_J2000);
    PlanetaryObject loadPlanetFromConfig(std::string config_file);
    static void create_planetary_object(std::vector<glm::vec4>& planet_vertices, std::vector<glm::vec3>& planetary_faces);
    int numPlanets() { return planets.size(); }
    void incrementDate(double inc) { current_date.incrementDate(inc); }
    void decrementDate(double dec) { current_date.decrementDate(dec); }
    Date getDate() {return current_date;}
    std::vector<PlanetaryObject> planets;
    PlanetaryObject sun;
private:
    void generateSolPlanetPosition(PlanetaryObject& planet, double centuries_past_J2000);
    Date current_date;    
    std::string sol_planets[9] = {"sun", "mercury", "venus", "earth", "mars", "jupiter", "saturn", "uranus", "neptune"};


};
