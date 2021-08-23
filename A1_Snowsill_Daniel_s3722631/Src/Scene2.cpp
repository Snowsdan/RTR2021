#include "Scene2.h"



void Scene2::InitialiseScene() {
	GenerateNewSponge();

}

void Scene2::DrawSponge() {
	for (Cube* currentCube : *spongeList) {
	
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, currentCube->position);
		sceneTwoShader->setMat4("model", modelMat);
		DrawCube(currentCube);

	}
}
void Scene2::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) {

	sceneTwoShader->Use();
	sceneTwoShader->setMat4("view", cameraMatrix);
	sceneTwoShader->setMat4("projection", projectionMatrix);
	
	for (Light* light : *lightList) {
		std::string lightTypeString = "lights[" + std::to_string(lightCounter) + "].type";
		//std::cout << lightTypeString << std::endl;
		std::string lightAmbientString = "lights[" + std::to_string(lightCounter) + "].ambient";
		std::string lightDiffuseString = "lights[" + std::to_string(lightCounter) + "].diffuse";
		std::string lightSpecString = "lights[" + std::to_string(lightCounter) + "].specular";

		sceneTwoShader->setInt(lightTypeString.c_str(), light->lightType);
		sceneTwoShader->setVec3(lightAmbientString.c_str(), light->ambient.x, light->ambient.y, light->ambient.z);
		sceneTwoShader->setVec3(lightDiffuseString.c_str(), light->diffuse.x, light->diffuse.y, light->diffuse.z);
		sceneTwoShader->setVec3(lightSpecString.c_str(), light->specular.x, light->specular.y, light->specular.z);

		if (light->lightType == point) {
			std::string lightPosString = "lights[" + std::to_string(lightCounter) + "].position";
			std::string lightConstString = "lights[" + std::to_string(lightCounter) + "].constant";
			std::string lightLinString = "lights[" + std::to_string(lightCounter) + "].linear";
			std::string lightQuadString = "lights[" + std::to_string(lightCounter) + "].quadratic";

			sceneTwoShader->setVec3(lightPosString.c_str(), light->lightPos.x, light->lightPos.y, light->lightPos.z);
			sceneTwoShader->setFloat(lightConstString.c_str(), light->constant );
			sceneTwoShader->setFloat(lightLinString.c_str(), light->linear);
			sceneTwoShader->setFloat(lightQuadString.c_str(), light->quadratic);
		}
		else {
			std::string lightDirString = "lights[" + std::to_string(lightCounter) + "].direction";
			sceneTwoShader->setVec3(lightDirString.c_str(), light->direction.x, light->direction.y, light->direction.z);
		}
		lightCounter++;
	}

	lightCounter = 0;
	//sceneTwoShader->setVec3("light.direction", cameraDirection.x, cameraDirection.y, cameraDirection.z);
	sceneTwoShader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	sceneTwoShader->setVec3("material.ambient", 1.0, 0.5, 0.31);
	sceneTwoShader->setVec3("material.diffuse", 1.0, 0.5, 0.31);
	sceneTwoShader->setVec3("material.specular", 0.5, 0.5, 0.5);
	sceneTwoShader->setFloat("material.shininess", 32.0);

	


	DrawSponge();

	for (Light* light : *lightList) {
		light->DrawLight(cameraMatrix, projectionMatrix);
	}

}

void Scene2::DrawCube(Cube* cube) {
	
	
	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int vertexArrayObject = 0;
	unsigned int colourBuffer = 0;
	unsigned int faceElementBuffer = 0;

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube->vertices), cube->vertices, GL_STATIC_DRAW);

	//Generate vertex array object
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Specify attribute locations
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Specify what data is for colour
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Specify what data is for normals
	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube->faces), cube->faces, GL_STATIC_DRAW);

	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, sizeof(cube->faces) / sizeof(cube->faces[0]), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;

}