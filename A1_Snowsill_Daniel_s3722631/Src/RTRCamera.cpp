#include "RTRCamera.h"






RTRCamera::RTRCamera() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}

void RTRCamera::MoveCameraForward(float &deltaTime) {
	cameraPos += (cameraSpeed * deltaTime) * cameraFront;
	SetLookAt();
}
void RTRCamera::MoveCameraBack(float& deltaTime) {
	cameraPos -= (cameraSpeed * deltaTime) * cameraFront;
	SetLookAt();

}
void RTRCamera::MoveCameraLeft(float& deltaTime) {
	cameraPos -= glm::normalize(glm::cross(cameraFront, up)) * (cameraSpeed * deltaTime);
	SetLookAt();

}
void RTRCamera::MoveCameraRight(float& deltaTime) {
	cameraPos += glm::normalize(glm::cross(cameraFront, up)) * (cameraSpeed * deltaTime);
	SetLookAt();

}
void RTRCamera::RotateCamera() {
	SDL_GetMouseState(&xPos, &yPos);

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

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	SetLookAt();
}

void RTRCamera::SetLookAt() {
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
}

glm::mat4 RTRCamera::GetViewMatrix() {
	SetLookAt();
	return view;
}
