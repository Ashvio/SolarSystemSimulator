#include "procedure_geometry.h"
#include "config.h"
#include <math.h>

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
	sphere_faces.push_back(glm::uvec3(1, 6, 10));

		
}
