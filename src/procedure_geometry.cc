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

void create_sphere(std::vector<glm::vec4>& sphere_vertices, std::vector<glm::uvec3>& sphere_faces) {
	std::cout << "start" << std::endl;
/*
	sphere_vertices.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.894f, 0.0f, 0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.276f, 0.851f, 0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-0.724f, 0.526f, 0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-0.724f, -0.526f, 0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.276f, -0.851f, 0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.724f, 0.526f, -0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-0.276f, 0.851f, -0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-0.894f, 0.0f, -0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(-0.276f, -0.851f, -0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.724f, -0.526f, -0.447f, 1.0f));
	sphere_vertices.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));

	sphere_faces.push_back(glm::uvec3(2, 1, 0));
	sphere_faces.push_back(glm::uvec3(3, 2, 0));
	sphere_faces.push_back(glm::uvec3(4, 3, 0));
	sphere_faces.push_back(glm::uvec3(5, 4, 0));
	sphere_faces.push_back(glm::uvec3(1, 5, 0));
	
	sphere_faces.push_back(glm::uvec3(11, 6, 7));
	sphere_faces.push_back(glm::uvec3(11, 7, 8));
	sphere_faces.push_back(glm::uvec3(11, 8, 9));
	sphere_faces.push_back(glm::uvec3(11, 9, 10));
	sphere_faces.push_back(glm::uvec3(11, 10, 6));

	sphere_faces.push_back(glm::uvec3(1, 2, 6));
	sphere_faces.push_back(glm::uvec3(2, 3, 7));
	sphere_faces.push_back(glm::uvec3(3, 4, 8));
	sphere_faces.push_back(glm::uvec3(4, 5, 9));
	sphere_faces.push_back(glm::uvec3(5, 1, 10));

	sphere_faces.push_back(glm::uvec3(2, 7, 6));
	sphere_faces.push_back(glm::uvec3(3, 8, 7));
	sphere_faces.push_back(glm::uvec3(4, 9, 8));
	sphere_faces.push_back(glm::uvec3(5, 10, 9));
	sphere_faces.push_back(glm::uvec3(1, 6, 10));*/

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

}

glm::vec2 get_uv_coord(glm::vec3 vert, float radius) {
	glm::vec2 uv;
	//uv[0] = asinf(vert.x)/M_PI + 0.5;
	//uv[1] = asinf(vert.y)/M_PI + 0.5;
	//uv[0] = atan2f(vert.x, vert.z)/(2.0*M_PI) + 0.5;
	//uv[1] = vert.y * 0.5 + 0.5;
	/*float theta = atan2f(-vert.z, vert.x);
	uv[0] = glm::normalize((theta + M_PI)/(2.0*M_PI));
	float phi = acosf(-vert.y/radius);
	uv[1] = glm::normalize(phi/M_PI);*/
	//uv[0] = atan2f(vert.z, vert.x)/(2.0 * M_PI) + 0.5;
	//uv[1] = 0.5 - asinf(vert.y)/M_PI;
	uv[0] = atan2f(vert.z, vert.x)/(2.0*M_PI) + 0.5;
	uv[1] = acosf(vert.y)/(M_PI);
	//uv[0] = atan2(vert.z, vert.x)/(2.0 * M_PI) + 0.5;
	//uv[1] = asin(vert.y)/M_PI + 0.5;
	//uv[0] = atan2(vert.x, vert.z)/(2.0 * M_PI) + 0.5;
	//uv[1] = asin(vert.y)/M_PI + 0.5;
	/*if (uv[0] < 0.75 && uv[1] > 0.75) {
		uv[0] += 1.0;
	} else if (uv[0] > 0.75 && uv[0] < 0.75) {
		uv[0] -= 1.0;
	}*/
	std::cout << "uv coordinates for " << glm::to_string(vert) << ": " << glm::to_string(uv) << std::endl;
	return uv;
}

void create_sphere_normals_and_uv(std::vector<glm::vec4>& sphere_normals, std::vector<glm::vec2>& sphere_uv, std::vector<glm::vec4>& sphere_vertices, float radius) {
	for (uint i = 0; i < sphere_vertices.size(); i++) {
		glm::vec4 vert = sphere_vertices[i];
		glm::vec3 p = glm::vec3(vert.x, vert.y, vert.z);
		p = glm::normalize(p);
		glm::vec4 norm (p, 0.0f);
		sphere_normals.push_back(norm);
		glm::vec2 uv = get_uv_coord(p, radius);
		sphere_uv.push_back(uv);
	}
}
