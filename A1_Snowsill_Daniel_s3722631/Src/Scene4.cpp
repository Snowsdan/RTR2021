#include "Scene4.h"

void Scene4::InitialiseScene() {
	std::cout << "START SCENE 3" << std::endl;
	GenerateNewSponge(1.0 / 3.0);
	CreateNewMatrices();
	IncreaseLights();

	numVertices *= numSponges;
	numFaces *= numSponges;
}

void Scene4::DeactivateScene() {

	numActiveLights = 0;
	spongeLevel = 1;
	numVertices = 0;
	numFaces = 0;
	glUseProgram(0);
	glBindVertexArray(0);
}

void Scene4::DrawSponges() {

	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int vertexArrayObject = 0;
	unsigned int faceElementBuffer = 0;
	unsigned int modelMatricesBuffer = 0;

	//Send matrix data to buffer
	

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, spongeAttributes->vertices.size() * sizeof(float), &spongeAttributes->vertices[0], GL_STATIC_DRAW);
	
	
	//Generate vertex array object
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//Specify what data is for vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Specify what data is for normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//std::cout << "NUMBER OF MATS: " << modelMatrices->size() << std::endl;
	//Specify data for model matrices
	glBindVertexArray(vertexArrayObject);
	glGenBuffers(1, &modelMatricesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, modelMatricesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(matrices), matrices, GL_STATIC_DRAW);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, 1);

	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, 1);

		
	

	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, spongeAttributes->faces.size() * sizeof(unsigned int), &spongeAttributes->faces[0], GL_STATIC_DRAW);

	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElementsInstanced(GL_TRIANGLES, spongeAttributes->faces.size(), GL_UNSIGNED_INT, 0, 9);
	//glDrawArraysInstanced(GL_TRIANGLES, spongeAttributes->vertices[0], spongeAttributes->vertices.size(), 9);
	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;
}

void Scene4::UpdateModelMats() {

	for (int i = 0; i < 9; i++) {
		matrices[i] = glm::rotate(matrices[i], glm::radians(1.0f), axisRotations[i]);
	}

}

void Scene4::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) {

	sceneFourShader->Use();
	sceneFourShader->setMat4("view", cameraMatrix);
	sceneFourShader->setMat4("projection", projectionMatrix);
	sceneFourShader->setInt("numActiveLights", numActiveLights);
	sceneFourShader->setBool("isLighting", isLighting);


	for (Light* light : *lightList) {
		std::string lightTypeString = "lights[" + std::to_string(lightCounter) + "].type";
		//std::cout << lightTypeString << std::endl;
		std::string lightAmbientString = "lights[" + std::to_string(lightCounter) + "].ambient";
		std::string lightDiffuseString = "lights[" + std::to_string(lightCounter) + "].diffuse";
		std::string lightSpecString = "lights[" + std::to_string(lightCounter) + "].specular";

		sceneFourShader->setInt(lightTypeString.c_str(), light->lightType);
		sceneFourShader->setVec3(lightAmbientString.c_str(), light->ambient.x, light->ambient.y, light->ambient.z);
		sceneFourShader->setVec3(lightDiffuseString.c_str(), light->diffuse.x, light->diffuse.y, light->diffuse.z);
		sceneFourShader->setVec3(lightSpecString.c_str(), light->specular.x, light->specular.y, light->specular.z);

		if (light->lightType == point) {
			std::string lightPosString = "lights[" + std::to_string(lightCounter) + "].position";
			std::string lightConstString = "lights[" + std::to_string(lightCounter) + "].constant";
			std::string lightLinString = "lights[" + std::to_string(lightCounter) + "].linear";
			std::string lightQuadString = "lights[" + std::to_string(lightCounter) + "].quadratic";

			sceneFourShader->setVec3(lightPosString.c_str(), light->lightPos.x, light->lightPos.y, light->lightPos.z);
			sceneFourShader->setFloat(lightConstString.c_str(), light->constant);
			sceneFourShader->setFloat(lightLinString.c_str(), light->linear);
			sceneFourShader->setFloat(lightQuadString.c_str(), light->quadratic);
		}
		else {
			std::string lightDirString = "lights[" + std::to_string(lightCounter) + "].direction";
			sceneFourShader->setVec3(lightDirString.c_str(), cameraDirection.x, cameraDirection.y, cameraDirection.z);
			//std::cout << "CAMERA DIRECTION: " << cameraDirection.x << " " << cameraDirection.y << " " << cameraDirection.z << std::endl;

		}
		lightCounter++;
	}
	lightCounter = 0;

	sceneFourShader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	//Set x axis faces to red
	sceneFourShader->setVec3("xMaterial.ambient", 0.3, 0.0, 0.0);
	sceneFourShader->setVec3("xMaterial.diffuse", 0.3, 0.0, 0.0);
	sceneFourShader->setVec3("xMaterial.specular", 1.0, 1.0, 1.0);
	sceneFourShader->setFloat("xMaterial.shininess", 128.0);

	//Set y axis faces to green
	sceneFourShader->setVec3("yMaterial.ambient", 0.0, 0.3, 0.0);
	sceneFourShader->setVec3("yMaterial.diffuse", 0.0, 0.3, 0.0);
	sceneFourShader->setVec3("yMaterial.specular", 1.0, 1.0, 1.0);
	sceneFourShader->setFloat("yMaterial.shininess", 128.0);

	//set z axis faces to blue
	sceneFourShader->setVec3("zMaterial.ambient", 0.0, 0.0, 0.3);
	sceneFourShader->setVec3("zMaterial.diffuse", 0.0, 0.0, 0.3);
	sceneFourShader->setVec3("zMaterial.specular", 1.0, 1.0, 1.0);
	sceneFourShader->setFloat("zMaterial.shininess", 128.0);

	//sceneFourShader->setMat4("model", modelMats->at(0).model)
	UpdateModelMats();
	DrawSponges();

	//Debug method for drawing where the lights are
	/*for (Light* light : *lightList) {
		light->DrawLight(cameraMatrix, projectionMatrix);
	}*/

}



void Scene4::CreateNewMatrices() {
	srand((unsigned)time(NULL));
	int index = 0;
	for (float x = -1; x <= 1; x++) {
		for (float y = -1; y <= 1; y++) {
			MatrixInfo matrix;
			glm::mat4 modelMat = glm::mat4(1.0f);

			//Create a sponge at level 1
			float xPos = x / 3.0 * 2.0;
			float yPos = y / 3.0 * 2.0;

			modelMat = glm::translate(modelMat, glm::vec3(xPos, yPos, 0.0));

			float xRotation = (float)rand() / RAND_MAX;
			float yRotation = (float)rand() / RAND_MAX;
			float zRotation = (float)rand() / RAND_MAX;

			matrix.rotationAxis = glm::vec3(xRotation, yRotation, zRotation);
			matrix.model = modelMat;

			modelMatrices->push_back(modelMat);
			matrices[index] = modelMat;
			axisRotations[index] = glm::vec3(xRotation, yRotation, zRotation);
			index++;
		}

	}
	std::cout << "NUMBER OF SPONGES: " << modelMats->size() << std::endl;
}

//This is different to the SceneBase version as it increases the number
//of vertices and faces 9 times instead of the usual once
void Scene4::IncreaseSponge() {
	std::list<Cube*>* newCubes = new std::list<Cube*>;
	spongeAttributes->vertices.clear();
	spongeAttributes->faces.clear();
	for (Cube* currentCube : *spongeAttributes->cubeList) {
		newCubes->splice(newCubes->end(), *GenCubes(currentCube, spongeAttributes));
	}
	numVertices += NUM_CUBE_VERTICES * numSponges;
	numFaces += NUM_CUBE_FACES * numSponges;
	spongeLevel++;

	spongeAttributes->cubeList = newCubes;
}

void Scene4::DecreaseSponge() {
	if (spongeLevel > 1) {
		spongeAttributes->cubeList->clear();
		numFaces = 0;
		numVertices = 0;
		GenerateNewSponge(1.0 / 3.0);

		int newSpongeLevel = spongeLevel - 1;
		spongeLevel = 1;


		//std::cout << "New Sponge LEVEL: " << newSpongeLevel << std::endl;

		for (int i = 1; i < newSpongeLevel; i++) {
			std::cout << "ADD LEVEL" << std::endl;

			IncreaseSponge();
		}
	}

}
