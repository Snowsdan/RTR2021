#include "Scene2.h"



void Scene2::InitialiseScene() {
	GenerateNewSponge();

}

void Scene2::DrawSponge() {
	for (Cube* currentCube : *spongeList) {
	
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, currentCube->position);
		sceneTwoShader->setMat4("model", modelMat);
		DrawCube(currentCube->size);

	}
}
void Scene2::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix) {

	//std::cout << "VIEW" << std::endl;

	sceneTwoShader->setMat4("view", cameraMatrix);


	//std::cout << "PROJECTION" << std::endl;


	sceneTwoShader->setMat4("projection", projectionMatrix);
	sceneTwoShader->Use();

	DrawSponge();

}

void Scene2::DrawCube(float size) {
	float radius = size / 2;
	//std::cout << "START DRAWING" << std::endl;

	unsigned int faces[] = {
		//Front face indices
		0, 2,1,
		0, 3, 2,
		//Left face indices
		4, 6, 5,
		4, 7, 6,
		//Back face indices
		9, 10, 8,
		8, 10, 11,
		//Right face indices
		12, 14, 13,
		12, 15, 14,
		////Top face indices
		16, 18, 17,
		16, 19, 18,
		////Bottom face indices
		20, 22, 21,
		20, 23, 22
	};

	float vertices[] = {
		//Front Face
		//     points              colours
		 -radius, radius, radius,    1.0f, 1.0f, 0.0f, //Top Left - 0
		  radius, radius, radius,    0.0f,1.0f, 0.0f, //Top Right - 1
		 radius, -radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 2
		-radius, -radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 3

		//Left Face
		//     points              colours
		 -radius, radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 4
		-radius, radius, radius,    0.0f,1.0f, 0.0f, //Top Right - 5
	   -radius, -radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 6
		-radius, -radius, -radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 7

		//Back Face
		//     points              colours
		-radius, radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 8
		 radius, radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 9
		radius, -radius, -radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 10
	   -radius, -radius, -radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 11

		//Right Face
		//     points              colours
		radius, radius, radius,    1.0f, 1.0f, 0.0f,  //Top Left - 12
	   radius, radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 13
	   radius, -radius,-radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 14
	   radius, -radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 15

		//Top Face
		//     points              colours
		-radius, radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 16
		 radius, radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 17
		  radius, radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 18
		 -radius, radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 19

		//Bottom Face
		//     points              colours
		radius, -radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 20
	   -radius, -radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 21
		-radius, -radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 22
		 radius, -radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 23
	};


	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int vertexArrayObject = 0;
	unsigned int colourBuffer = 0;
	unsigned int faceElementBuffer = 0;

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Generate vertex array object
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Specify attribute locations
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	

	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, sizeof(faces) / sizeof(faces[0]), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;

}