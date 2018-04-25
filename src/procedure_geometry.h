#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>


void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);

void create_sphere(std::vector<glm::vec4>& sphere_vertices, std::vector<glm::uvec3>& sphere_faces);

void create_sphere_normals_and_uv(std::vector<glm::vec4>& sphere_normals, std::vector<glm::vec2>& sphere_uv, std::vector<glm::vec4>& sphere_vertices, float radius);
#endif
