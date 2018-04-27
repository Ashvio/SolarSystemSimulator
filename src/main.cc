#include <GL/glew.h>
#include <dirent.h>

#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "procedure_geometry.h"
#include "solar_system.h"
#include <memory>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>

int window_width = 1280;
int window_height = 720;


const std::string window_title = "mInEcrAfT";

const char* vertex_shader =
#include "shaders/default.vert"
;

const char* geometry_shader =
#include "shaders/default.geom"
;

const char* fragment_shader =
#include "shaders/default.frag"
;

const char* floor_fragment_shader =
#include "shaders/floor.frag"
;

const char* sphere_vertex_shader =
#include "shaders/sphere.vert"
;

const char* sphere_tcs_shader =
#include "shaders/sphere.tesc"
;

const char* sphere_tes_shader =
#include "shaders/sphere.tese"
;

const char* sphere_geometry_shader =
#include "shaders/sphere.geom"
;

const char* sphere_fragment_shader =
#include "shaders/sphere.frag"
;

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

GLFWwindow* init_glefw()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);
	CHECK_SUCCESS(ret != nullptr);
	glfwMakeContextCurrent(ret);
	glewExperimental = GL_TRUE;
	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	return ret;
}

int main(int argc, char* argv[])
{
	
	GLFWwindow *window = init_glefw();
	GUI gui(window, window_width, window_height);

	std::vector<glm::vec4> floor_vertices;
	std::vector<glm::uvec3> floor_faces;
	create_floor(floor_vertices, floor_faces);

	// Create solar system here
	SolarSystem sol = SolarSystem();
	sol.generateSolPlanets();

	glm::vec4 light_position = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
	MatrixPointers mats; // Define MatrixPointers here for lambda to capture
	/*
	 * In the following we are going to define several lambda functions to bind Uniforms.
	 *
	 * Introduction about lambda functions:
	 *      http://en.cppreference.com/w/cpp/language/lambda
	 *      http://www.stroustrup.com/C++11FAQ.html#lambda
	 */
	/*
	 * The following lambda functions are defined to bind uniforms
	 */
	auto matrix_binder = [](int loc, const void* data) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)data);
	};
	
	auto vector_binder = [](int loc, const void* data) {
		glUniform4fv(loc, 1, (const GLfloat*)data);
	};
	auto vector3_binder = [](int loc, const void* data) {
		glUniform3fv(loc, 1, (const GLfloat*)data);
	};
	auto float_binder = [](int loc, const void* data) {
		glUniform1fv(loc, 1, (const GLfloat*)data);
	};
	auto int_binder = [](int loc, const void* data) {
		glUniform1iv(loc, 1, (const GLint*)data);
	};

	auto textureMS_binder = [](int loc, const void* data) {
		glActiveTexture(GL_TEXTURE0); //make texture register 0 active
		GLuint *texture_data = (GLuint *)data;
 		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *texture_data); //bind textureB as out input texture
  		glUniform1i(loc, 0); //pass texture B as a sampler to the shader
	};
	auto texture_binder = [](int loc, const void* data) {
		glActiveTexture(GL_TEXTURE0); //make texture register 0 active
		GLuint *texture_data = (GLuint *)data;
 		glBindTexture(GL_TEXTURE_2D, *texture_data); //bind textureB as out input texture
  		glUniform1i(loc, 0); //pass texture B as a sampler to the shader
	};
	/*
	 * The lambda functions below are used to retrieve data
	 */
	auto std_model_data = [&mats]() -> const void* {
		return mats.model;
	}; // This returns point to model matrix
	glm::mat4 floor_model_matrix = glm::mat4(1.0f);
	auto floor_model_data = [&floor_model_matrix]() -> const void* {
		return &floor_model_matrix[0][0];
	}; // This return model matrix for the floor.
	auto std_view_data = [&mats]() -> const void* {
		return mats.view;
	};
	auto std_camera_data  = [&gui]() -> const void* {
		return &gui.getCamera()[0];
	};
	auto std_proj_data = [&mats]() -> const void* {
		return mats.projection;
	};
	
	auto std_light_data = [&light_position]() -> const void* {
		return &light_position[0];
	};
	auto tess_level_data = [&gui]() -> const void* {
		return &gui.tess_level;
	};

	ShaderUniform std_model = { "model", matrix_binder, std_model_data };
	ShaderUniform floor_model = { "model", matrix_binder, floor_model_data };
	ShaderUniform std_view = { "view", matrix_binder, std_view_data };
	ShaderUniform std_camera = { "camera_position", vector3_binder, std_camera_data };
	ShaderUniform std_proj = { "projection", matrix_binder, std_proj_data };
	ShaderUniform std_light = { "light_position", vector_binder, std_light_data };
	ShaderUniform tess_level_inner = { "tess_level_inner", float_binder, tess_level_data };
	ShaderUniform tess_level_outer = { "tess_level_outer", float_binder, tess_level_data };
	
	// FIXME: define more ShaderUniforms for RenderPass if you want to use it.
	//        Otherwise, do whatever you like here

	//ShaderUniform bone_transform {"bone_transform", bone_transform_binder, bone_transform_data};
	// Floor render pass
	RenderDataInput floor_pass_input;
	floor_pass_input.assign(0, "vertex_position", floor_vertices.data(), floor_vertices.size(), 4, GL_FLOAT);
	floor_pass_input.assignIndex(floor_faces.data(), floor_faces.size(), 3);
	RenderPass floor_pass(-1,
			floor_pass_input,
			{ vertex_shader, geometry_shader, floor_fragment_shader, nullptr, nullptr },
			{ floor_model, std_view, std_proj, std_light },
			{ "fragment_color" }
			);

	
	// PMD Model render pass
	// FIXME: initialize the input data at Mesh::loadPmd
	
	// Setup the render pass for drawing bones
	// FIXME: You won't see the bones until Skeleton::joints were properly
	//        initialized
	
	bool draw_floor = false;


	if (argc >= 3) {
		// mesh.loadAnimationFrom(argv[2]);
	}

	while (!glfwWindowShouldClose(window)) {

		

		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

		gui.updateMatrices();
		mats = gui.getMatrixPointers();
		if (draw_floor) {
			floor_pass.setup();
			// Draw our triangles.
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
			                              floor_faces.size() * 3,
			                              GL_UNSIGNED_INT, 0));
		}

		// Render solar system
		if (sol.numPlanets() > 0) {
			std::vector<glm::vec4> planet_vertices;
			std::vector<glm::uvec3> planet_faces;
			//std::vector<glm::vec4> planet_normals;
			//std::vector<glm::vec2> uv_coordinates;
				
			create_sphere(planet_vertices, planet_faces);
			// Iterate through the planets and render each of them
			for (int i = 0; i < sol.numPlanets(); i++) {
				PlanetaryObject planet = sol.planets[i];
				std::vector<glm::vec4> planet_normals;
				std::vector<glm::vec2> uv_coordinates;
				create_sphere_normals_and_uv(planet_normals, uv_coordinates, planet_vertices, planet.renderRadius);
				// Get specific radius for planet
				auto radius_data = [&planet]() -> const void * {
					return &planet.renderRadius;
				};
				auto text_binder = [](int loc, const void *data) {
					//glUniform1iv(loc, 1, (const GLint*)data);
					glUniform1i(loc, 0);
				};
				auto texture_data = [&planet]() -> const void * {
					return &planet.texture;
				};
				// Uniforms
				ShaderUniform radius = { "radius", float_binder, radius_data };
				ShaderUniform texture = { "textureSampler", text_binder, texture_data };
				// Rendering planet
				RenderDataInput planet_pass_input;
				planet_pass_input.assign(0, "vertex_position", planet_vertices.data(), planet_vertices.size(), 4, GL_FLOAT);
				planet_pass_input.assign(1, "normal", planet_normals.data(), planet_normals.size(), 4, GL_FLOAT);
				planet_pass_input.assign(2, "uv", uv_coordinates.data(), uv_coordinates.size(), 2, GL_FLOAT);
				planet_pass_input.assignIndex(planet_faces.data(), planet_faces.size(), 3);
				RenderPass planet_pass(-1,
									   planet_pass_input,
									   { sphere_vertex_shader, sphere_geometry_shader, sphere_fragment_shader, sphere_tcs_shader, sphere_tes_shader},
									   { std_model, std_view, std_proj, std_light, tess_level_inner, tess_level_outer, radius, texture },
									   { "fragment_color" });

				planet_pass.setup();

				glPatchParameteri(GL_PATCH_VERTICES, 3);

				CHECK_GL_ERROR(glDrawElements(GL_PATCHES,
											  planet_faces.size() * 3,
											  GL_UNSIGNED_INT, 0));
			}
		}
		
		
	
		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
