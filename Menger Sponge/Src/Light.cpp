#include "Light.h"


Light::Light(LightType type, glm::vec3 position, glm::vec3 dir, glm::vec3 ambientVec,
	glm::vec3 diffuseVec, glm::vec3 specularVec,  float constantVal, float linearVal, float quadraticVal ) {
	lightType = type;
	lightPos = position;
	direction = dir;
	ambient = ambientVec;
	diffuse = diffuseVec;
	specular = specularVec;

	constant = constantVal;
	linear = linearVal;
	quadratic = quadraticVal;

}

void Light::DrawLight(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix) {
	lightShader->Use();
	//shader->setVec3("lightColour", 1.0f, 1.0f, 1.0f);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));



	lightShader->setMat4("model", model);
	lightShader->setMat4("view", cameraMatrix);
	lightShader->setMat4("projection", projectionMatrix);
	DrawLightModel();
}
void Light::DrawLightModel() {
	float radius = 0.5;
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
		 -radius, radius, radius,    //Top Left - 0
		  radius, radius, radius,     //Top Right - 1
		 radius, -radius, radius,    //Bottom Right - 2
		-radius, -radius, radius,     //Bottom Left - 3

		//Left Face
		//     points              colours
		-radius, radius, -radius,    //Top Left - 4
		 -radius, radius, radius,     //Top Right - 5
		-radius, -radius, radius,    //Bottom Right - 6
	   -radius, -radius, -radius,     //Bottom Left - 7

		//Back Face
		//     points              colours
		-radius, radius, -radius,      //Top Left - 8
		 radius, radius, -radius,     //Top Right - 9
		radius, -radius, -radius,    //Bottom Right - 10
	   -radius, -radius, -radius,     //Bottom Left - 11

		//Right Face
		//     points              colours
		  radius, radius, radius,      //Top Left - 12
		 radius, radius, -radius,     //Top Right - 13
		 radius, -radius,-radius,    //Bottom Right - 14
		 radius, -radius, radius,    //Bottom Left - 15

		//Top Face
		//     points              colours
		-radius, radius, -radius,     //Top Left - 16
		 radius, radius, -radius,     //Top Right - 17
		  radius, radius, radius,    //Bottom Right - 18
		 -radius, radius, radius,    //Bottom Left - 19

		//Bottom Face
		//     points              colours
		radius, -radius, -radius,     //Top Left - 20
	   -radius, -radius, -radius,    //Top Right - 21
		-radius, -radius, radius,    //Bottom Right - 22
		 radius, -radius, radius,    //Bottom Left - 23
	};
	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int lightVAO = 0;
	unsigned int colourBuffer = 0;
	unsigned int faceElementBuffer = 0;

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Generate vertex array object
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	//Specify attribute locations
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);
	//std::cout << "RENDER FRAME" << std::endl;
	//Draw the shape
	glBindVertexArray(lightVAO);
	glDrawElements(GL_TRIANGLES, sizeof(faces) / sizeof(faces[0]), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	lightVAO = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;
}