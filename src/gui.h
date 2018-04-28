#ifndef SKINNING_GUI_H
#define SKINNING_GUI_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "tictoc.h"
#include <cstdio>
#include <map>

/*
 * Hint: call glUniformMatrix4fv on thest pointers
 */
struct MatrixPointers {
	const float *projection, *model, *view, *ortho;
};

class GUI {
public:
	GUI(GLFWwindow*, int view_width = -1, int view_height = -1);
	~GUI();

	void keyCallback(int key, int scancode, int action, int mods);
	void mousePosCallback(double mouse_x, double mouse_y);
	void mouseButtonCallback(int button, int action, int mods);
	void mouseScrollCallback(double dx, double dy);
	void updateMatrices();
	MatrixPointers getMatrixPointers() const;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallback(GLFWwindow* window, double dx, double dy);

	void updateViewingAngles();
	glm::vec3 getCenter() const { return center_; }
	const glm::vec3& getCamera() const { return eye_; }
	bool isPoseDirty() const { return pose_changed_; }
	void clearPose() { pose_changed_ = false; }
	void setPoseDirty() { pose_changed_ = true; }
	const float* getLightPositionPtr() const { return &light_position_[0]; }
	
	
	bool isTransparent() const { return transparent_; }
	bool isPlaying() const { return animation_enabled; }
	void togglePlaying() {
		animation_enabled  = !animation_enabled;
	}

	void toggleRecording();
	FILE* ffmpeg;
	int* buffer;
	bool is_recording = false;

	float tess_level = 20.0f;
	// TODO: move radius to be specific for each planet
	float radius = 1.0f;
	
private:
	GLFWwindow* window_;

	int window_width_, window_height_;
	int view_width_, view_height_;

	bool drag_state_ = false;
	bool fps_mode_ = false;
	bool pose_changed_ = true;
	bool transparent_ = false;
	bool free_mode = true;

	int current_button_ = -1;
	float roll_speed_ = M_PI / 64.0f;
	float last_x_ = 0.0f, last_y_ = 0.0f, current_x_ = 0.0f, current_y_ = 0.0f;
	float camera_distance_ = 100.0f;
	float pan_speed_ = 0.08f;
	float rotation_speed_ = 0.003f;
	float zoom_speed_ = 0.10f;
	float aspect_;

	glm::vec3 eye_ = glm::vec3(0.0f, 0.1f, camera_distance_);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 tangent_ = glm::cross(look_, up_);
	glm::vec3 center_ = eye_ - camera_distance_ * look_;
	glm::mat3 orientation_ = glm::mat3(tangent_, up_, look_);
	glm::vec4 light_position_;

	glm::mat4 view_matrix_ = glm::lookAt(eye_, center_, up_);
	glm::mat4 projection_matrix_;
	glm::mat4 model_matrix_ = glm::mat4(1.0f);
	glm::mat4 ortho_matrix_;

	bool captureWASDUPDOWN(int key, int action);

	bool animation_enabled = false;
	bool first_animation = true;	
	TicTocTimer timer;
	double elapsed_time = 0;

	glm::mat3 view_x_rotation;
	glm::mat3 view_y_rotation;
	std::map<std::string, bool> active_keys;
	glm::vec3 movement_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif
