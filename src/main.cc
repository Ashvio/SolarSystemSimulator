#include <GL/glew.h>
#include <dirent.h>

#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "procedure_geometry.h"
#include "solar_system.h"
#include "particle.h"
#include <memory>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "../lib/utgraphicsutil/image.h"
#include "../lib/utgraphicsutil/jpegio.h"

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>


int window_width = 1880;
int window_height = 1000;
int main_view_width = 1560;
int main_view_height = 1000;
int preview_width = window_width - main_view_width; // 320
int preview_height = preview_width / 4 * 3; // 320 / 4 * 3 = 240
int bar_width = 3;

const std::string window_title = "Solar System Simulator";

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

const char* bar_frag_shader =
#include "shaders/bar.frag"
;

const char* sky_frag_shader =
#include "shaders/sky.frag"
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
	SolarSystem sol = SolarSystem();
	
	GUI gui(window, main_view_width, main_view_height, preview_height, &sol);
	GUI minimap_gui(nullptr, preview_width, preview_width, preview_height, &sol);

	std::vector<glm::vec4> floor_vertices;
	std::vector<glm::uvec3> floor_faces;
	create_floor(floor_vertices, floor_faces);

	std::vector<glm::vec4> rectangle_vertices;
	std::vector<glm::uvec3> rectangle_faces;
	std::vector<glm::vec2> tex_coords;
	create_rectangle(rectangle_vertices, rectangle_faces, tex_coords);

	// sky texture
	Image* sky_img = PlanetaryObject::loadImage("../assets/stars.jpg");
	GLuint sky_texture = 0;
	if (sky_img == NULL) {
		std::cout << "image could not be loaded" << std::endl;
	} else {
		glGenTextures(1, &sky_texture);
		// bind texture
		glBindTexture(GL_TEXTURE_2D, sky_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// Give the image to OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sky_img->width, sky_img->height, 0, GL_RGB, GL_UNSIGNED_BYTE, sky_img->bytes.data());
	}

	// Create solar system here
	sol.generateSolPlanets();

	// ParticleTrail used to generate particles
	ParticleTrail particles = ParticleTrail();

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
	auto texture_binder = [](int loc, const void *data) {
		glUniform1i(loc, 0);
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
	auto sky_texture_data = [&sky_texture]() -> const void * {
		return &sky_texture;
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

	RenderDataInput sky_input;
	// Scroll bar will use the same vertices and faces as preview since we just want a rectangle
	sky_input.assign(0, "vertex_position", rectangle_vertices.data(), rectangle_vertices.size(), 4, GL_FLOAT);
	sky_input.assign(1, "uv", tex_coords.data(), tex_coords.size(), 2, GL_FLOAT);
	sky_input.assignIndex(rectangle_faces.data(), rectangle_faces.size(), 3);
	
	ShaderUniform sky_texture_sampler = { "textureSampler", texture_binder, sky_texture_data };

	RenderPass sky_pass(-1, sky_input,
						{ vertex_shader, nullptr, sky_frag_shader, nullptr, nullptr },
						{ sky_texture_sampler },
						{ "fragment_color" });

	RenderDataInput bar_input;
	// Scroll bar will use the same vertices and faces as preview since we just want a rectangle
	bar_input.assign(0, "vertex_position", rectangle_vertices.data(), rectangle_vertices.size(), 4, GL_FLOAT);
	bar_input.assignIndex(rectangle_faces.data(), rectangle_faces.size(), 3);
	
	RenderPass bar_pass(-1, bar_input,
						{ vertex_shader, nullptr, bar_frag_shader, nullptr, nullptr },
						{ },
						{ "fragment_color" });
			
	// PMD Model render pass
	// FIXME: initialize the input data at Mesh::loadPmd
	
	// Setup the render pass for drawing bones
	// FIXME: You won't see the bones until Skeleton::joints were properly
	//        initialized
	
	bool draw_floor = false;
	bool draw_bar = true;
	bool draw_sky = false;
	bool draw_planets = true;

	if (argc >= 3) {
		// mesh.loadAnimationFrom(argv[2]);
	}
	int m = 0;
	TicTocTimer timer = tic();
	
	while (!glfwWindowShouldClose(window)) {

		auto time_delta = toc(&timer);
		

		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, main_view_width, main_view_height);
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

		if (draw_sky) {
			glBindTexture(GL_TEXTURE_2D, sky_texture);
			sky_pass.setup();

			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
										rectangle_faces.size() * 3,
										GL_UNSIGNED_INT, 0));

		}		

		if (draw_bar) {
			glViewport(main_view_width, 0, bar_width, main_view_height);
			
			bar_pass.setup();

			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
										rectangle_faces.size() * 3,
										GL_UNSIGNED_INT, 0));

			glViewport(0, 0, main_view_width, main_view_height);	
		}

		// Render solar system
		if (sol.numPlanets() > 0 && draw_planets) {
			if (gui.is_playing) {
				if (m % 25 == 0)
				std::cout << sol.getDate().getDate() << "\tFPS: " << 1 / time_delta << std::endl;
				if (gui.forwards) {
					sol.incrementDate(time_delta * gui.system_speed);
				} else {
					sol.decrementDate(time_delta * gui.system_speed);
				}
			    double centuries_past_J2000 = sol.getDate().getCenturiesPastJ2000();		
				// std::cout << "Time: " << centuries_past_J2000 << "\n";
			}
			m++;
			
            sol.generateSolPlanetPositions();
			std::vector<glm::vec4> planet_vertices;
			std::vector<glm::uvec3> planet_faces;
			std::vector<glm::vec4> planet_normals;
				
			create_sphere(planet_vertices, planet_faces, planet_normals);
			// Iterate through the planets and render each of them
			for (int i = -1; i < sol.numPlanets(); i++) {
				PlanetaryObject planet;
				if (i == -1) {
					planet = sol.sun;
				} else {
					planet = sol.planets[i];
				}
				// don't use color for planets
				int is_particle = 0;
				glm::vec4 pos = glm::vec4(*planet.getPosition());
				glBindTexture(GL_TEXTURE_2D, planet.texture);
				// Get specific radius for planet
				auto radius_data = [&planet]() -> const void * {
					return &planet.renderRadius;
				};
                auto planet_position_data = [&pos]() -> const void * { 
                    return &pos;
				};
				auto default_color_data = [&planet]() -> const void * { 
                    return &planet.color;
                };
				auto scale_data = [&gui]() -> const void * {
					return &gui.scalePlanetRadius;
				};
				auto text_binder = [](int loc, const void *data) {
					//glUniform1iv(loc, 1, (const GLint*)data);
					glUniform1i(loc, 0);
				};
				auto texture_data = [&planet]() -> const void * {
					return &planet.texture;
				};
				auto use_color_data = [&is_particle]() -> const void * {
					return &is_particle;
				};
				// Uniforms
				ShaderUniform radius = { "radius", float_binder, radius_data };
				ShaderUniform scaleFactor = { "scaleFactor", float_binder, scale_data };
				ShaderUniform texture = { "textureSampler", texture_binder, texture_data };
                ShaderUniform planet_position = { "planet_position", vector_binder, planet_position_data };
				ShaderUniform default_color = { "default_color", vector_binder, default_color_data };
				ShaderUniform use_color = { "use_color", int_binder, use_color_data };
				// Rendering planet
				RenderDataInput planet_pass_input;
				planet_pass_input.assign(0, "vertex_position", planet_vertices.data(), planet_vertices.size(), 4, GL_FLOAT);
				planet_pass_input.assign(1, "normal", planet_normals.data(), planet_normals.size(), 4, GL_FLOAT);
				planet_pass_input.assignIndex(planet_faces.data(), planet_faces.size(), 3);
				RenderPass planet_pass(-1,
									   planet_pass_input,
									   { sphere_vertex_shader, sphere_geometry_shader, sphere_fragment_shader, sphere_tcs_shader, sphere_tes_shader},
									   { std_model, std_view, std_proj, std_light, tess_level_inner, tess_level_outer, radius, texture, scaleFactor, planet_position, default_color, use_color },
									   { "fragment_color" });
				
				planet_pass.setup();

				glPatchParameteri(GL_PATCH_VERTICES, 3);

				CHECK_GL_ERROR(glDrawElements(GL_PATCHES,
											  planet_faces.size() * 3,
											  GL_UNSIGNED_INT, 0));
				/*
				// Render particles to show orbits
				if (gui.show_orbit) {
					float timePassed = gui.time_delta;
					particles.update_particles(planet, timePassed);
					// indicate that this is a particle when rendering
					is_particle = 1;
					// loop through all the particles that are alive and render them
					for (int i = 0; i < particles.getNumParticles(); i++) {
						Particle p = particles.particles[i];
						// Render particle if it's alive
						glm::vec4 particle_position = glm::vec4(p.position, 1.0);
						if (p.life > 0.0f) {
							auto particle_radius_data = [&p]() -> const void * {
								return &p.radius;
							};
							auto particle_position_data = [&particle_position]() -> const void * {
								return &particle_position;
							};
							auto particle_color_data = [&p]() -> const void * {
								return &p.color;
							};

							ShaderUniform p_radius = { "radius", float_binder, particle_radius_data };
							ShaderUniform p_position = {"planet_position", vector_binder, particle_position_data};
							ShaderUniform p_color = {"default_color", vector_binder, particle_color_data};

							// Rendering planet
							RenderDataInput particle_pass_input;
							// Use same verticles and faces as planet since you're trying to render spheres
							particle_pass_input.assign(0, "vertex_position", planet_vertices.data(), planet_vertices.size(), 4, GL_FLOAT);
							particle_pass_input.assign(1, "normal", planet_normals.data(), planet_normals.size(), 4, GL_FLOAT);
							particle_pass_input.assignIndex(planet_faces.data(), planet_faces.size(), 3);
							RenderPass particle_pass(-1,
													 particle_pass_input,
													 {sphere_vertex_shader, sphere_geometry_shader, sphere_fragment_shader, sphere_tcs_shader, sphere_tes_shader},
													 {std_model, std_view, std_proj, std_light, tess_level_inner, tess_level_outer, p_radius, p_position, p_color, use_color},
													 {"fragment_color"});

							particle_pass.setup();

							glPatchParameteri(GL_PATCH_VERTICES, 3);

							CHECK_GL_ERROR(glDrawElements(GL_PATCHES,
														  planet_faces.size() * 3,
														  GL_UNSIGNED_INT, 0));
						}
					}
				} else {
					particles.kill_particles();
				}*/

			}
		}
		/*
		mats = minimap_gui.getMatrixPointers();
		
			// Render Minimap solar system
			if (sol.numPlanets() > 0 && draw_planets) {
				glViewport(main_view_width, main_view_height - 300, 300, 300);
				// if (gui.is_playing) {
				// 	if (m % 25 == 0)
				// 	std::cout << sol.getDate().getDate() << "\tFPS: " << 1 / time_delta << std::endl;
				// 	if (gui.forwards) {
				// 		sol.incrementDate(time_delta * gui.system_speed);
				// 	} else {
				// 		sol.decrementDate(time_delta * gui.system_speed);
				// 	}
				//     double centuries_past_J2000 = sol.getDate().getCenturiesPastJ2000();		
				// 	// std::cout << "Time: " << centuries_past_J2000 << "\n";
				// }
				//m++;
				
				// sol.generateSolPlanetPositions();
				std::vector<glm::vec4> planet_vertices;
				std::vector<glm::uvec3> planet_faces;
				std::vector<glm::vec4> planet_normals;
					
				create_sphere(planet_vertices, planet_faces, planet_normals);
				// Iterate through the planets and render each of them
				for (int i = -1; i < sol.numPlanets(); i++) {
					PlanetaryObject planet;
					if (i == -1) {
						planet = sol.sun;
					} else {
						planet = sol.planets[i];
					}
					glm::vec4 pos = glm::vec4(*planet.getPosition());
					glBindTexture(GL_TEXTURE_2D, planet.texture);
					// Get specific radius for planet
					double r = planet.renderRadius * 1.0;
					auto radius_data = [&planet]() -> const void * {
						return &planet.renderRadius;
					};
					auto planet_position_data = [&pos]() -> const void * { 
						return &pos;
					};
					auto default_color_data = [&planet]() -> const void * { 
						return &planet.color;
					};
					auto scale_data = [&gui]() -> const void * {
						return &gui.scalePlanetRadius;
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
					ShaderUniform scaleFactor = { "scaleFactor", float_binder, scale_data };
					ShaderUniform texture = { "textureSampler", texture_binder, texture_data };
					ShaderUniform planet_position = { "planet_position", vector_binder, planet_position_data };
					ShaderUniform default_color = { "default_color", vector3_binder, default_color_data };
					// Rendering planet
					RenderDataInput planet_pass_input;
					planet_pass_input.assign(0, "vertex_position", planet_vertices.data(), planet_vertices.size(), 4, GL_FLOAT);
					planet_pass_input.assign(1, "normal", planet_normals.data(), planet_normals.size(), 4, GL_FLOAT);
					planet_pass_input.assignIndex(planet_faces.data(), planet_faces.size(), 3);
					RenderPass planet_pass(-1,
											planet_pass_input,
											{ sphere_vertex_shader, sphere_geometry_shader, sphere_fragment_shader, sphere_tcs_shader, sphere_tes_shader},
											{ std_model, std_view, std_proj, std_light, tess_level_inner, tess_level_outer, radius, texture, scaleFactor, planet_position, default_color },
											{ "fragment_color" });
					
					planet_pass.setup();
	
					glPatchParameteri(GL_PATCH_VERTICES, 3);
	
					CHECK_GL_ERROR(glDrawElements(GL_PATCHES,
													planet_faces.size() * 3,
													GL_UNSIGNED_INT, 0));
													
				}
				glViewport(0, 0, main_view_width, main_view_height);	
				
			}

*/
	
		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
