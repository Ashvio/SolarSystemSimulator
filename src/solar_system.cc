#include "solar_system.h"
#include <glm/gtx/string_cast.hpp>

void SolarSystem::generateSolPlanets() {
    // std::string name, float radius, float mass, bool is_sun, bool is_planet
    
    int i = 0;
    
    for (std::string name : sol_planets) {
        std::string num = std::to_string(i);
        std::string fn = "../src/config/sol/" + num + name + ".json";
        PlanetaryObject planet = loadPlanetFromConfig(fn);
        if (i > 0) {
            planets.push_back(planet);
        } else {
            sun = planet;
        }
        i++;
    }
}

void SolarSystem::generateSolPlanetPosition(PlanetaryObject& planet, double centuries_past_J2000) {
    OrbitalElements current_elements = OrbitalElements::computeCurrentElements(planet.start_elements, planet.diff_elements, planet.has_bcsf, centuries_past_J2000);
    //All our variables..
    double perihelion;
    double mean_anomaly;
    double eccentric_anomaly;
    glm::dvec3 heliocentric_coords;
    glm::dvec3 ecliptic_coords;
    glm::dvec3 equatorial_coords;
    
    //All our math..
    current_elements.computePerihelionAndMeanAnomaly(perihelion, mean_anomaly, planet.has_bcsf, centuries_past_J2000);
    current_elements.computeEccentricAnomaly(mean_anomaly, eccentric_anomaly, 1000);
    current_elements.computeHeliocentricCoordinates(eccentric_anomaly, heliocentric_coords);
    current_elements.computeEclipticCoordinates(heliocentric_coords, perihelion, ecliptic_coords);
    current_elements.computeEquatorialCoordinates(ecliptic_coords, equatorial_coords);
    glm::dvec4 new_pos = glm::dvec4(equatorial_coords * (AU/SCALE_FACTOR), 1.0);
    planet.setLastPosition(new_pos);
    // equatorial_coords[1] = 0.0;
    planet.setPosition(new_pos);
    // std::cout << planet.getName() << " " << glm::to_string(*planet.getPosition()) << std::endl;
    // std::cout << planet.getName() << " " << (glm::length(*planet.getPosition())) << std::endl;
    
}

glm::dvec4 SolarSystem::getSolPlanetPosition(PlanetaryObject *planet, double centuries_past_J2000) {
    OrbitalElements current_elements = OrbitalElements::computeCurrentElements(planet->start_elements, planet->diff_elements, planet->has_bcsf, centuries_past_J2000);
    //All our variables..
    double perihelion;
    double mean_anomaly;
    double eccentric_anomaly;
    glm::dvec3 heliocentric_coords;
    glm::dvec3 ecliptic_coords;
    glm::dvec3 equatorial_coords;
    
    //All our math..
    current_elements.computePerihelionAndMeanAnomaly(perihelion, mean_anomaly, planet->has_bcsf, centuries_past_J2000);
    current_elements.computeEccentricAnomaly(mean_anomaly, eccentric_anomaly, 10000);
    current_elements.computeHeliocentricCoordinates(eccentric_anomaly, heliocentric_coords);
    current_elements.computeEclipticCoordinates(heliocentric_coords, perihelion, ecliptic_coords);
    current_elements.computeEquatorialCoordinates(ecliptic_coords, equatorial_coords);
    // equatorial_coords[1] = 0.0;
    return glm::dvec4(equatorial_coords * (AU/SCALE_FACTOR), 1.0);
    // std::cout << planet.getName() << " " << glm::to_string(*planet.getPosition()) << std::endl;
    // std::cout << planet.getName() << " " << (glm::length(*planet.getPosition())) << std::endl;
    
}


void SolarSystem::generateSolPlanetPositions() {
    double centuries_past_J2000 = current_date.getCenturiesPastJ2000();
    for (auto& planet : planets) {
        generateSolPlanetPosition(planet, centuries_past_J2000);    
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
