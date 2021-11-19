#include "RTRCamera.h"






RTRCamera::RTRCamera() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}

void RTRCamera::MoveCameraForward(float &deltaTime) {
	cameraPos += (cameraSpeed * cameraFront) * deltaTime;
	SetLookAt();
}
void RTRCamera::MoveCameraBack(float& deltaTime) {
	cameraPos -= (cameraSpeed * cameraFront) * deltaTime;
	SetLookAt();
}
void RTRCamera::MoveCameraLeft(float& deltaTime) {
	cameraPos -= (glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed) * deltaTime;
	SetLookAt();

}
void RTRCamera::MoveCameraRight(float& deltaTime) {
	cameraPos += (glm::normalize(glm::cross(cameraFront, up)) * cameraSpeed) * deltaTime;
	SetLookAt();

}
void RTRCamera::RotateCamera() {
	SDL_GetMouseState(&xPos, &yPos);

	if (isFirstMove) {
		lastX = xPos;
		lastY = yPos;
		isFirstMove = false;
	}

	//Calculate difference between last position and new position
	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	float sensitivity = 0.15f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//Add the difference to the new rotation
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 newDirection;
	newDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	newDirection.y = sin(glm::radians(pitch));
	newDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	direction = newDirection;

	SetLookAt();
}

void RTRCamera::SetLookAt() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}

glm::mat4 RTRCamera::GetViewMatrix() {
	SetLookAt();
	return view;
}

void RTRCamera::ResetCamera() {
	cameraPos = glm::vec3(0.0, 0.0, 3.0);
	glm::vec3 newDirection;
	newDirection.x = cos(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
	newDirection.y = sin(glm::radians(0.0f));
	newDirection.z = sin(glm::radians(-90.0f)) * cos(glm::radians(0.0f));
	cameraFront = glm::normalize(direction);
	direction = newDirection;

	SetLookAt();
	isFirstMove = false;
}
