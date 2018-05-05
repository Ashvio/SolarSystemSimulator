#include "gui.h"
#include "config.h"
#include <jpegio.h>
#include <cmath>
#include <iostream>
#include <debuggl.h>
#include <string>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cstdio>
#include "solar_system.h"

namespace {
	// FIXME: Implement a function that performs proper
	//        ray-cylinder intersection detection
	// TIPS: The implement is provided by the ray-tracer starter code.
}

GUI::GUI(GLFWwindow* window, int view_width, int view_height,SolarSystem* sol)
	:window_(window), sol(sol)
{
	if (window) {
	glfwSetWindowUserPointer(window_, this);
	glfwSetKeyCallback(window_, KeyCallback);
	glfwSetCursorPosCallback(window_, MousePosCallback);
	glfwSetMouseButtonCallback(window_, MouseButtonCallback);
	glfwSetScrollCallback(window_, MouseScrollCallback);

	glfwGetWindowSize(window_, &window_width_, &window_height_);
	}
	if (view_width < 0 || view_height < 0) {
		view_width_ = window_width_;
		view_height_ = window_height_;
	} else {
		view_width_ = view_width;
		view_height_ = view_height;
	}
	float aspect_ = static_cast<float>(view_width_) / view_height_;
	projection_matrix_ = glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	timer = tic();
	active_keys["W"] = false;
	active_keys["A"] = false;
	active_keys["S"] = false;
	active_keys["D"] = false;
	active_keys["UP"] = false;
	active_keys["DOWN"] = false;
	active_keys["SPACE"] = false;

	movement_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	deceleration_velocity = 0.0f;
}

GUI::~GUI()
{
}

void GUI::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, GL_TRUE);
		return ;
	}
	if (key == GLFW_KEY_J && action == GLFW_RELEASE) {
		std::cout << "taking screenshot" << std::endl;
		//FIXME save out a screenshot using SaveJPEG
		int dataSize = window_width_ * window_height_ * 3;
		unsigned char* pixels = (unsigned char *)malloc(dataSize * sizeof(unsigned char));
		if (!pixels) {
			std::cout << "error mallocing" << std::endl;
		}
		glReadPixels((GLint)0, (GLint)0, window_width_, window_height_, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		if (!SaveJPEG("Screenshot.jpeg", window_width_, window_height_, pixels)) {
			std::cout << "error saving image" << std::endl;
		}
	}
	if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
		std::cout << "Toggling reverse mode\n";
		forwards = !forwards;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		is_playing = !is_playing;
	}
	if (key == GLFW_KEY_MINUS && action != GLFW_RELEASE) {
		if (scalePlanetRadius >= 5.0) {
			scalePlanetRadius -= 2;
		}
	}
	if (key == GLFW_KEY_EQUAL && action != GLFW_RELEASE) {
		if (scalePlanetRadius <= 50.0) {
			scalePlanetRadius += 2.0;
		}
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
		scalePlanetRadius = 1.0;
	}
	if (key == GLFW_KEY_1 && action != GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[0].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_2 && action!= GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[1].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_3 && action != GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[2].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_4 && action != GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[3].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_5 && action != GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[4].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_6 && action != GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[5].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_7 && action!= GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[6].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_8 && action != GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->planets[7].getPosition();
		eye_[2] += 170000.0;
	}
	if (key == GLFW_KEY_9 && action!= GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		eye_ = *sol->sun.getPosition();
		eye_[2] += 1300000.0;
	}
	if (key == GLFW_KEY_GRAVE_ACCENT && action!= GLFW_RELEASE) {
		look_ = glm::vec3(0.0f, 0.0f, -1.0f);
		camera_distance_ = 30000000.0f;
		eye_ = glm::vec3(0.0f, 0.1f, camera_distance_);
	}
	if (key == GLFW_KEY_PAGE_UP  && action != GLFW_RELEASE) {
		if (system_speed < - 2 || system_speed > 0)
		
		system_speed += 2;
	}
	if (key == GLFW_KEY_PAGE_DOWN  && action != GLFW_RELEASE) {
		if (system_speed > 2 || system_speed < 0)
		system_speed -= 2;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		// toggle between showing and not showing orbit
		show_orbit = !show_orbit;
	}

	if (mods == 0 && captureWASDUPDOWN(key, action))
		return ;

	 else if (key == GLFW_KEY_F && (mods & GLFW_MOD_CONTROL) && action == GLFW_PRESS) {
		free_mode = !free_mode;
	 }

}

void GUI::toggleRecording() {
    is_recording = !is_recording;
    if (is_recording) {
		std::cout << "Starting recording...\n";
		
        const char* cmd = "ffmpeg -r 24 -f rawvideo -pix_fmt rgba -s 1280x720 -i - "
                          "-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip animation.mp4";
        ffmpeg = popen(cmd, "w");

        buffer = new int[window_width_*window_height_];
    } else {
		std::cout << "Stopping recording... saving to animation.mp4\n";
		
        pclose(ffmpeg);

    }
}

int count = 0;
bool was_space = false;
void GUI::updateViewingAngles() {
	count++;
	time_delta = toc(&timer);
	glm::vec3 next_eye = eye_;

	// in free mode, increment eye with constant velocity while key is pressed
	if (free_mode) {
		float velocity = 1000000.0f;
		// KEY MOVEMENTS
		if (active_keys["W"] == true) {
			next_eye += velocity * (float)time_delta * look_;
			//std::cout << "time delta " << time_delta << std::endl;
		}
		if (active_keys["S"] == true) {
			next_eye -= velocity * (float)time_delta * look_;
		}
		if (active_keys["D"] == true) {
			next_eye += velocity * (float)time_delta * tangent_;
		}
		if (active_keys["A"] == true) {
			next_eye -= velocity * (float)time_delta * tangent_;
		}
		if (active_keys["UP"] == true) {
			next_eye += velocity * (float)time_delta * up_;
		}
		if (active_keys["DOWN"] == true) {
			next_eye -= velocity * (float)time_delta * up_;
		}
	// in spaceship mode, accelerate when key pressed, keeps going at constant velocity when key not pressed
	} else {

		if (!was_space) {
			deceleration_velocity = 0.0f;
		}

		auto velocity_delta = time_delta * 200000.0f;
		// move x component
		if (movement_velocity[0] != 0.0) {
			next_eye += movement_velocity[0] * (float)time_delta * tangent_;
			// accelerate if key is pressed
			if (!was_space) {
				if (active_keys["A"] == true) {
					// accelerate in negative direction, going left
					movement_velocity[0] -= velocity_delta;
				} else if (active_keys["D"] == true) {
					// accelerate in positive direction, going right
					movement_velocity[0] += velocity_delta;
				}
			// decelerate with space
			} else {
				if (movement_velocity[0] < 0) {
					movement_velocity[0] += deceleration_velocity;
					if (active_keys["SPACE"] == true) {
						deceleration_velocity += velocity_delta;
					}
					// decelerate until 0
					movement_velocity[0] = glm::min(movement_velocity[0], 0.0f);
				} else if (movement_velocity[0] > 0){
					movement_velocity[0] -= deceleration_velocity;
					if (active_keys["SPACE"] == true) {
						deceleration_velocity += velocity_delta;
					}
					movement_velocity[0] = glm::max(movement_velocity[0], 0.0f);
				}
			}
			
		}
		// move y component
		if (movement_velocity[1] != 0.0) {
			next_eye += movement_velocity[1] * (float)time_delta * up_;
			// accelerate if key is pressed
			if (!was_space) {
				if (active_keys["DOWN"] == true) {
					// accelerate in negative direction, going left
					movement_velocity[1] -= velocity_delta;
				} else if (active_keys["UP"] == true) {
					// accelerate in positive direction, going right
					movement_velocity[1] += velocity_delta;
				}
				// decelerate with space
			} else {
				if (movement_velocity[1] < 0) {
					movement_velocity[1] += deceleration_velocity;
					if (active_keys["SPACE"] == true) {
						deceleration_velocity += velocity_delta;
					}
					// decelerate until 0
					movement_velocity[1] = glm::min(movement_velocity[1], 0.0f);
				} else if (movement_velocity[1] > 0){
					movement_velocity[1] -= deceleration_velocity;
					if (active_keys["SPACE"] == true) {
						deceleration_velocity += velocity_delta;
					}
					movement_velocity[1] = glm::max(movement_velocity[1], 0.0f);
				}
			}
			
		}
		// move z component
		if (movement_velocity[2] != 0.0) {
			next_eye += movement_velocity[2] * (float)time_delta * look_;
			if (!was_space) {
				// accelerate if key is pressed
				if (active_keys["S"] == true) {
					// accelerate in negative direction, going left
					movement_velocity[2] -= velocity_delta;
				} else if (active_keys["W"] == true) {
					// accelerate in positive direction, going right
					movement_velocity[2] += velocity_delta;
				}
			} else {
				if (movement_velocity[2] < 0) {
					movement_velocity[2] += deceleration_velocity;
					if (active_keys["SPACE"] == true) {
						deceleration_velocity += velocity_delta;
					}
					// decelerate until 0
					movement_velocity[2] = glm::min(movement_velocity[2], 0.0f);
				} else if (movement_velocity[2] > 0){
					movement_velocity[2] -= deceleration_velocity;
					if (active_keys["SPACE"] == true) {
						deceleration_velocity += velocity_delta;
					}
					movement_velocity[2] = glm::max(movement_velocity[2], 0.0f);
				}
			}
			
		}

		if (movement_velocity == glm::vec3(0.0, 0.0, 0.0)) {
			was_space = false;
			deceleration_velocity = 0.0f;
		}

	}
	

	if (next_eye != eye_) {
		eye_ = next_eye;
	}

	if (view_y_rotation != glm::mat3(1.0f) || view_x_rotation != glm::mat3(1.0f)) {
		orientation_ = view_y_rotation * orientation_;
		orientation_ = view_x_rotation * orientation_;

		tangent_ = glm::column(orientation_, 0);
		up_ = glm::column(orientation_, 1);
		look_ = glm::column(orientation_, 2);
		//	std::cout << "look y " << look_.y << std::endl;
	}
	/*
	if (was_space) { 
		current_space--;
		if (current_space > 0) {
			eye_[1] += 25.0f * zoom_speed_ * ((float)current_space) / 10.0f;
			cyl->updateEye(eye_);
		 }
		 if (current_space <= 0) {
			was_space = false;			
		 }
	}*/

/*
	if (gravity_ && (!was_space || current_space <= 0) && newEyeHeight > eyeGroundHeight) {
		auto velocity_delta = time_delta * -30.0f;
		newEyeHeight += (velocity + velocity_delta)*time_delta; 
		velocity += velocity_delta;
		// velocity += .005;
		if (newEyeHeight <= eyeGroundHeight) { //Replace with hitting the gruond
			current_space = MAX_SPACE; //Reset jump potential when you hit the ground.
			velocity = 0;
			newEyeHeight = eyeGroundHeight;
			was_space = false;
		}
	}
*/
}

bool was_dragging = false;
void GUI::mousePosCallback(double mouse_x, double mouse_y)
{
	//current_bone_ = -1;
	last_x_ = current_x_;
	last_y_ = current_y_;
	current_x_ = mouse_x;
	current_y_ = window_height_ - mouse_y;
	float delta_x = current_x_ - last_x_;
	float delta_y = current_y_ - last_y_;
	//if (sqrt(delta_x * delta_x + delta_y * delta_y) < 1e-15)
	//	return;
	if (mouse_x > view_width_)
		return ;
	//glm::vec3 mouse_direction = glm::normalize(glm::vec3(delta_x, delta_y, 0.0f));
	glm::vec2 mouse_start = glm::vec2(last_x_, last_y_);
	glm::vec2 mouse_end = glm::vec2(current_x_, current_y_);

	view_x_rotation = glm::mat3(1.0f);
	view_y_rotation = glm::mat3(1.0f);

	// Mouse on lower side of screen
	if (mouse_y > view_height_ - 100) {
		view_y_rotation = glm::mat3(glm::rotate(-8.0f * rotation_speed_, tangent_));
	// Mouse on upper side of screen
	} else if (mouse_y < 100 && mouse_y >= 0) {
		view_y_rotation = glm::mat3(glm::rotate(8.0f * rotation_speed_, tangent_));
	}
	// Mouse on right side of screen
	if (mouse_x > view_width_ - 100) {
		view_x_rotation = glm::mat3(glm::rotate(-8.0f * rotation_speed_, glm::vec3(0.0f, 1.0f, 0.0f)));
	// Mouse on left side of screen
	} else if (mouse_x < 100 && mouse_x >= 0) {
		view_x_rotation = glm::mat3(glm::rotate(8.0f * rotation_speed_, glm::vec3(0.0f, 1.0f, 0.0f)));
	}

}

void GUI::mouseButtonCallback(int button, int action, int mods)
{
	if (current_x_ <= view_width_) {
		drag_state_ = (action == GLFW_PRESS);
		current_button_ = button;
		return ;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
	}

	// FIXME: Key Frame Selection
}

void GUI::mouseScrollCallback(double dx, double dy)
{
	if (current_x_ < view_width_)
		return;
	
	// FIXME: Mouse Scrolling
}

void GUI::updateMatrices()
{
	updateViewingAngles();
	// Compute our view, and projection matrices.
	center_ = eye_ + camera_distance_ * look_;

	view_matrix_ = glm::lookAt(eye_, center_, up_);
	light_position_ = glm::vec4(eye_, 1.0f);

	aspect_ = static_cast<float>(view_width_) / view_height_;
	projection_matrix_ =
		glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	model_matrix_ = glm::mat4(1.0f);
}

MatrixPointers GUI::getMatrixPointers() const
{
	MatrixPointers ret;
	ret.projection = &projection_matrix_[0][0];
	ret.model= &model_matrix_[0][0];
	ret.view = &view_matrix_[0][0];
	return ret;
}

bool GUI::captureWASDUPDOWN(int key, int action)
{
	
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			active_keys["W"] = true;
			if (!free_mode && movement_velocity[2] == 0.0) {
				movement_velocity[2] = 5.0;
			}
			was_space = false;
		} else if (action == GLFW_RELEASE) {
			active_keys["W"] = false;
		}
		return true;
	} else if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			active_keys["S"] = true;
			if (!free_mode && movement_velocity[2] == 0.0) {
				movement_velocity[2] = -5.0;
			}
			was_space = false;
		} else if (action == GLFW_RELEASE) {
			active_keys["S"] = false;
		}
		return true;
	} else if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			active_keys["A"] = true;
			if (!free_mode && movement_velocity[0] == 0.0) {
				movement_velocity[0] = -5.0;
			}
			was_space = false;
		} else if (action == GLFW_RELEASE) {
			active_keys["A"] = false;
		}
		return true;
	} else if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			active_keys["D"] = true;
			if (!free_mode && movement_velocity[0] == 0.0) {
				movement_velocity[0] = 5.0;
			}
			was_space = false;
		} else if (action == GLFW_RELEASE) {
			active_keys["D"] = false;
		}
		return true;
	} else if (key == GLFW_KEY_DOWN) {
		if (action == GLFW_PRESS) {
			active_keys["DOWN"] = true;
			if (!free_mode && movement_velocity[1] == 0.0) {
				movement_velocity[1] = -5.0;
			}
			was_space = false;
		} else if (action == GLFW_RELEASE) {
			active_keys["DOWN"] = false;
		}
		return true;
	} else if (key == GLFW_KEY_UP) {
		if (action == GLFW_PRESS) {
			active_keys["UP"] = true;
			if (!free_mode && movement_velocity[1] == 0.0) {
				movement_velocity[1] = 5.0;
			}
			was_space = false;
		} else if (action == GLFW_RELEASE) {
			active_keys["UP"] = false;
		}
		return true;
	} else  if (key == GLFW_KEY_SPACE) {
		if (action == GLFW_PRESS) {
			active_keys["SPACE"] = true;
			was_space = true;
			if (!free_mode && deceleration_velocity == 0.0) {
				deceleration_velocity = 1.0;
			}
		} else if (action == GLFW_RELEASE) {
			active_keys["SPACE"] = false;
		}
		return true;
	}
	return false;
}


// Delegrate to the actual GUI object.
void GUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->keyCallback(key, scancode, action, mods);
}

void GUI::MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mousePosCallback(mouse_x, mouse_y);
}

void GUI::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseButtonCallback(button, action, mods);
}

void GUI::MouseScrollCallback(GLFWwindow* window, double dx, double dy)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseScrollCallback(dx, dy);
}
