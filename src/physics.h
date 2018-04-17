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

class Gravity {

    //Universal Gravitation Formula http://www.physicsclassroom.com/class/circles/Lesson-3/Newton-s-Law-of-Universal-Gravitation
    static glm::dvec3 gravityBetween(double mass1, double mass2, glm::dvec3 pos1, glm::dvec3 pos2) {
        glm::dvec3 diff_vect = pos1 - pos2;
        double distance_diff = glm::length(diff_vect);

        double denom = std::pow(distance_diff, 2.0);
        double numerator = mass1 * mass2;
        double scalar_force = G * (numerator/denom);
        glm::dvec3 vector_force = scalar_force * glm::normalize(diff_vect);
        return vector_force;
    }

}