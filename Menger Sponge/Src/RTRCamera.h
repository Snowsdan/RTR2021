#pragma once
#include <sdl/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


class RTRCamera {
public:
	RTRCamera();
	void MoveCameraForward(float& deltaTime);
	void MoveCameraBack(float& deltaTime);
	void MoveCameraLeft(float& deltaTime);
	void MoveCameraRight(float& deltaTime);
	void RotateCamera();
	void ResetCamera();
	glm::mat4 GetViewMatrix();
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 direction;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	void SetLookAt();
	bool isFirstMove = true;
protected:
	
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	float yaw = -90;
	float pitch = 0;
	float cameraSpeed = 0.001f;
	float lastX = 0, lastY = 0;
	int xPos, yPos;
};

