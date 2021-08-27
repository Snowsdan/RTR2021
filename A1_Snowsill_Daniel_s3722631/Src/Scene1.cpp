#include "Scene1.h"

void Scene1::InitialiseScene() {
	
}

void Scene1::DeactivateScene() {

}
void  Scene1::DrawSponge() {

}
void  Scene1::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix,
	glm::vec3 cameraPos, glm::vec3 cameraDirection) {
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 10.0);
	//gluPerspective(75.0, ((float)width / (float)height), 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float len = 2.0;

	//m::lookAt();
	//glLoadMatrix();
	glTranslatef(0.0, 0.0, 0.5);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(len, 0.0, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, -1.0, 0.0);
	glVertex3f(0.0, len, 0.0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, len);
	glEnd();
}

void  Scene1::DrawCube(Cube* cube) {

}


