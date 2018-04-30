#include "solar_system.h"

void SolarSystem::generateSolPlanets() {
    // std::string name, float radius, float mass, bool is_sun, bool is_planet
    
    int i = 1;
    /*
    for (std::string name : sol_planets) {
        std::string num = std::to_string(i);
        std::string fn = "../src/config/sol/" + num + name + ".json";
        PlanetaryObject planet = loadPlanetFromConfig(fn);
        std::cout << "name " << planet.getName() << " radius " << planet.radius << std::endl;
        //PlanetaryObject planet = PlanetaryObject("Neptune", 20.0f, 100.0f, false, true);
        //std::cout << "radius " << planet.radius << std::endl;
        planet.loadTexture("../assets/neptune.jpg");
        planets.push_back(planet);
        i++;
    }*/
    std::string num = std::to_string(1);
        std::string fn = "../src/config/sol/" + num + sol_planets[0] + ".json";
        //PlanetaryObject planet = PlanetaryObject("Neptune", 4000.0f, 100.0f, false, true);
        PlanetaryObject planet = loadPlanetFromConfig(fn);
        planet.loadTexture("../assets/neptune.jpg");
        planets.push_back(planet);
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
    planet.setPosition(glm::dvec4(equatorial_coords, 1.0));
    

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
    texture = 0;
    glGenTextures(1, &texture);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width, img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->bytes.data());
}
