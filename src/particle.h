#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "solar_system.h"

class Particle {
public:
    Particle() : position(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), color(glm::vec4(1.0, 1.0, 1.0, 1.0)), life(0.0f), radius (300.0) { }
    Particle(glm::vec4 pos, glm::vec3 v, glm::vec4 c, float l, float r) : position(pos), color(c), life(l), radius(r) { }

    glm::vec4 position;
    glm::vec4 color;
    float radius;
    float life;
};

class ParticleTrail {
public:
    ParticleTrail() : num_particles(50000), num_to_spawn(1) {
        for (int i = 0; i < num_particles; i++) {
            particles.push_back(Particle());
        }
    }
    ParticleTrail(int num_p, int num_s) : num_particles(num_p), num_to_spawn(num_s) {
        for (int i = 0; i < num_particles; i++) {
            particles.push_back(Particle());
        }
    }
    int get_unused_particle();
    void reuse_particle(int particle_index, glm::vec4 planet_position, int planet_index);
    void update_particles(glm::vec4 planet_position, float time_passed, int planet_index);
    void kill_particles();

    int getNumParticles() { return num_particles; } 
    std::vector<glm::vec4> alive_particle_colors;
    std::vector<glm::vec4> alive_particle_positions;
    std::vector<Particle> particles;
private:
    
    int num_particles;
    int last_used_particle = 0;
    int num_to_spawn;
};
