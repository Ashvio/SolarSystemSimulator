#include "procedure_geometry.h"
#include "config.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <glm/gtx/string_cast.hpp>

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

void create_rectangle(std::vector<glm::vec4>& rectangle_vertices, std::vector<glm::uvec3>& rectangle_faces, std::vector<glm::vec2>& tex_coords) {
	rectangle_vertices.push_back(glm::vec4(-1.0, 1.0, 0.0, 1.0));
	rectangle_vertices.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));
	rectangle_vertices.push_back(glm::vec4(1.0, -1.0, 0.0, 1.0));
	rectangle_vertices.push_back(glm::vec4(-1.0, -1.0, 0.0, 1.0));
	
	rectangle_faces.push_back(glm::uvec3(0, 2, 1));
	rectangle_faces.push_back(glm::uvec3(2, 0, 3));

	tex_coords.push_back(glm::vec2(0.0, 1.0));
	tex_coords.push_back(glm::vec2(1.0, 1.0));
	tex_coords.push_back(glm::vec2(1.0, 0.0));
	tex_coords.push_back(glm::vec2(0.0, 0.0));
}

void create_sphere(std::vector<glm::vec4>& sphere_vertices, std::vector<glm::uvec3>& sphere_faces, std::vector<glm::vec4>& sphere_normals) {

	float p = (1.0 + sqrt(5.0)) / 2.0;
	sphere_vertices.push_back(glm::vec4(-1.0f, p, 0.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(1.0f, p, 0.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-1.0f, -p, 0.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(1.0f, -p, 0.0f, 1.0f));

	sphere_vertices.push_back(glm::vec4(0.0f, -1.0f, p, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.0f, 1.0f, p, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.0f, -1.0f, -p, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.0f, 1.0f, -p, 1.0f));

	sphere_vertices.push_back(glm::vec4(p, 0.0f, -1.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(p, 0.0f, 1.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-p, 0.0f, -1.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-p, 0.0f, 1.0f, 1.0f));

	
	// 5 faces around point 0
	sphere_faces.push_back(glm::uvec3(0, 11, 5));
	sphere_faces.push_back(glm::uvec3(0, 5, 1));
	sphere_faces.push_back(glm::uvec3(0, 1, 7));
	sphere_faces.push_back(glm::uvec3(0, 7, 10));
	sphere_faces.push_back(glm::uvec3(0, 10, 11));

	sphere_faces.push_back(glm::uvec3(1, 5, 9));
	sphere_faces.push_back(glm::uvec3(5, 11, 4));
	sphere_faces.push_back(glm::uvec3(11, 10, 2));
	sphere_faces.push_back(glm::uvec3(10, 7, 6));
	sphere_faces.push_back(glm::uvec3(7, 1, 8));

	sphere_faces.push_back(glm::uvec3(3, 9, 4));
	sphere_faces.push_back(glm::uvec3(3, 4, 2));
	sphere_faces.push_back(glm::uvec3(3, 2, 6));
	sphere_faces.push_back(glm::uvec3(3, 6, 8));
	sphere_faces.push_back(glm::uvec3(3, 8, 9));

	sphere_faces.push_back(glm::uvec3(4, 9, 5));
	sphere_faces.push_back(glm::uvec3(2, 4, 11));
	sphere_faces.push_back(glm::uvec3(6, 2, 10));
	sphere_faces.push_back(glm::uvec3(8, 6, 7));
	sphere_faces.push_back(glm::uvec3(9, 8, 1));

	for (uint i = 0; i < sphere_vertices.size(); i++) {
		glm::vec4 vert = sphere_vertices[i];
		glm::vec3 p = glm::vec3(vert.x, vert.y, vert.z);
		p = glm::normalize(p);
		glm::vec4 norm (p, 0.0f);
		sphere_normals.push_back(norm);
	}

}

