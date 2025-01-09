#ifndef CAMERA_H
#define CAMERA_H

#include "include/glad/glad.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/GLFW/glfw3.h"

#include <iostream>
#include <deque>

const float TurnSpeed = 80.0f;

class Camera {
public:
    Camera(glm::vec3 cPos = glm::vec3(0.0f, 0.0f, 0.0f) ):cameraPos(cPos) {};
	~Camera() {};
    void CameraMove(GLFWwindow *window);
    void setDeltaTime();
    void shakewindow();
    
	glm::mat4 CameraView();
    void setshake(bool s);
	void MovewithMouse(float xoffset, float yoffset);
	void Zoom(float yoffset);
	glm::mat4 getProjection(float width, float height);
	void ForwardEffect();
	void BackEffect();
	void ZoomRecover();
	float deltaTime = 0.0f; 
	void FixView(glm::vec3 carPos, glm::vec3 carFront);
	bool isFixed();
	void setFixed(glm::vec3 &carfront) {
		fixed = !fixed;
		yaw = 0;
		FixedYaw = 0;
		pitch = 0;
		lastFrontX.push_back(carfront.x);
		lastFrontZ.push_back(carfront.z);
	}

	float getYaw() {
		return yaw;
	}
	glm::vec3 cameraPos;
//private:
	float currentTime;
    float lastTime = 0.0f; 
	bool shakeflag = true; 
    bool shake = false; 
    float yaw = -90.0f;
    float pitch = 0.0f;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	float fov = 80.0f;
	float sensitivity = 0.05;
	bool fixed = false;
	float radius = 1.0f;
	float FixedYaw = 0.0f;
	std::deque<float>lastFrontX;
	std::deque<float>lastFrontZ;
};

#endif
