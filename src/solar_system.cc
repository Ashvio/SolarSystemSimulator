#include "solar_system.h"

void SolarSystem::generateSolPlanets() {
    // std::string name, float radius, float mass, bool is_sun, bool is_planet
    
    int i = 1;
    
    for (std::string name : sol_planets) {
        std::string num = std::to_string(i);
        std::string fn = "../src/config/sol/" + num + name + ".json";
        PlanetaryObject planet = loadPlanetFromConfig(fn);
        planets.push_back(planet);
        i++;
    }
}

void PlanetaryObject::loadTexture(const std::string& file_name) {
    // Get image
    Image* img = loadImage(file_name);
    if (img == NULL) {
        std::cout << "image not found" << std::endl;
        return;
    }
    texture = 0;
    glGenTextures(1, &texture);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->bytes.data());
}
