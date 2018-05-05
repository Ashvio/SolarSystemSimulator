#include "particle.h"

int ParticleTrail::get_unused_particle() {
    // Search from last used particle to get unused particle
    for (int i = last_used_particle; i < num_particles; i++) {
        // Check if particle is dead
        if (particles[i].life <= 0.0f){
            last_used_particle = i;
            return i;
        }
    }
    // Otherwise, do a linear search
    for (int i = 0; i < last_used_particle; i++) {
        if (particles[i].life <= 0.0f){
            last_used_particle = i;
            return i;
        }
    }
    // Use first particle if all of them are alive
    last_used_particle = 0;
    return 0;
}  

void ParticleTrail::reuse_particle(int particle_index, glm::vec4 planet_position, int planet_index)
{
    Particle &p = particles[particle_index];
    float r = ((rand() % 100) - 50) / 10.0f;
    p.position = planet_position;
    p.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (planet_index == 0) {
        p.life = 30.0f;
    } else if (planet_index == 1) {
        p.life = 83.0f;
    } else if (planet_index == 2) {
        p.life = 135.0f;
    } else if (planet_index == 3) {
        p.life = 4000.0f;
    } else {
        p.life = 8000.0f;
    }
    last_used_particle = particle_index;
}

void ParticleTrail::update_particles(glm::vec4 planet_position, float time_passed, int planet_index) {
    alive_particle_colors.clear();
    alive_particle_positions.clear();
    // Add new particles
    for (int i = 0; i < num_to_spawn; i++) {
        int unused_particle_index = get_unused_particle();
        //std::cout << "unused particle index " << unused_particle_index << std::endl;
        reuse_particle(unused_particle_index, planet_position, planet_index);
    }
    // Update all particles
    for (int i = 0; i < num_particles; i++) {
        Particle &p = particles[i];
        p.life -= time_passed; // reduce life
        if (p.life > 0.0f) { // particle is alive, thus update
            p.color[3] -= time_passed * .005;
            if (p.color[3] <= 0) {
                p.life = 0.0f;
            } else {
                alive_particle_positions.push_back(p.position);
                alive_particle_colors.push_back(p.color);
            }
        }
    }
}

void ParticleTrail::kill_particles() {
    std::cout << "killing particles" << std::endl;
    for (int i = 0; i < num_particles; i++) {
        Particle &p = particles[i];
        p.life = 0.0f;
    }
}