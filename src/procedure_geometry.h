#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>


void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);

void create_rectangle(std::vector<glm::vec4>& rectangle_vertices, std::vector<glm::uvec3>& rectangle_faces, std::vector<glm::vec2>& tex_coords);

void create_sphere(std::vector<glm::vec4>& sphere_vertices, std::vector<glm::uvec3>& sphere_faces, std::vector<glm::vec4>& sphere_normals);

#endif
