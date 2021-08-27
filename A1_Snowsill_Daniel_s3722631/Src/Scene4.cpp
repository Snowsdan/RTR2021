#include "Scene4.h"

void Scene4::InitialiseScene() {
	std::cout << "START SCENE 3" << std::endl;
	CreateNewSponges();



	IncreaseLights();
}

void Scene4::DeactivateScene() {
	sponges->clear();
	numActiveLights = 0;
	spongeLevel = 1;
	numVertices = 0;
	numFaces = 0;
	glUseProgram(0);
	glBindVertexArray(0);
}

void Scene4::DrawSponges() {

	for (Scene4Sponge* currentSponge : *sponges) {
		currentSponge->rotation = currentSponge->rotation + 1.0f;
		float rotation = currentSponge->rotation;

		glm::mat4 model = glm::mat4(1.0);

		model = glm::translate(model, currentSponge->origin);
		model = glm::rotate(model, glm::radians(currentSponge->rotation), currentSponge->rotationAxis);


		sceneFourShader->setMat4("model", model);
		DrawSponge(currentSponge->sponge);

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

	DrawSponges();

	//Debug method for drawing where the lights are
	/*for (Light* light : *lightList) {
		light->DrawLight(cameraMatrix, projectionMatrix);
	}*/

}



void Scene4::IncreaseSponge() {

	for (Scene4Sponge* currentSponge : *sponges) {
		std::list<Cube*>* newCubes = new std::list<Cube*>;
		currentSponge->sponge->vertices.clear();
		currentSponge->sponge->vertices.clear();
		for (Cube* currentCube : *currentSponge->sponge->cubeList) {

			newCubes->splice(newCubes->end(), *GenCubes(currentCube, currentSponge->sponge));
		}

		currentSponge->sponge->cubeList = newCubes;

	}
	spongeLevel++;
}

void Scene4::DecreaseSponge() {
	if (spongeLevel > 1) {
		numFaces = 0;
		numVertices = 0;

		for (Scene4Sponge* currentSponge : *sponges) {
			currentSponge->sponge->cubeList->clear();
			currentSponge->sponge->vertices.clear();
			currentSponge->sponge->faces.clear();
		}
		sponges->clear();
		CreateNewSponges();

		int newSpongeLevel = spongeLevel - 1;
		spongeLevel = 1;

		for (int i = 1; i < newSpongeLevel; i++)
			IncreaseSponge();

	}
}

void Scene4::CreateNewSponges() {
	srand((unsigned)time(NULL));

	for (float x = -1; x <= 1; x++) {
		for (float y = -1; y <= 1; y++) {
			//std::list<Cube*>* sponge = new std::list<Cube*>;
			Scene4Sponge* spongeInfo = new Scene4Sponge;

			//Create a sponge at level 1
			float xPos = x / 3.0 * 2.0;
			float yPos = y / 3.0 * 2.0;

			spongeInfo->origin = glm::vec3(xPos, yPos, 0.0);

			/*td::cout << "xPos: " << xPos << std::endl;
			std::cout << "yPos: " << yPos << std::endl << std::endl;*/

			//Multiply by 1.5 to spread them out
			Cube* baseCube = new Cube(1.0 / 3.0, glm::vec3(0, 0, 0));
			float xRotation = (float)rand() / RAND_MAX;
			float yRotation = (float)rand() / RAND_MAX;
			float zRotation = (float)rand() / RAND_MAX;

			/*std::cout << "xROTATION: " << xRotation << std::endl;
			std::cout << "Y ROTATION: " << yRotation << std::endl << std::endl;*/
			//std::cout << "SPLICE CUBES "  << std::endl;

			spongeInfo->rotationAxis = glm::vec3(xRotation, yRotation, zRotation);
			spongeInfo->sponge->cubeList->splice(spongeInfo->sponge->cubeList->end(),
				*GenCubes(baseCube, spongeInfo->sponge));
			//std::cout << "DONE SPLICING CUBES " << std::endl;

			//Add the new sponge to the list
			sponges->push_back(spongeInfo);

			numFaces += NUM_CUBE_FACES;
			numVertices += NUM_CUBE_VERTICES;
		}

	}
	std::cout << "NUMBER OF SPONGES: " << sponges->size() << std::endl;
}

void Scene4::DrawSponge(SpongeAtt* attributes) {

	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int vertexArrayObject = 0;
	unsigned int faceElementBuffer = 0;
	unsigned int modelMatricesBuffer = 0;

	//Send matrix data to buffer
	glGenBuffers(1, &modelMatricesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, modelMatricesBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * modelMats->size(), &modelMats[0], GL_STATIC_DRAW);

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, attributes->vertices.size() * sizeof(float), &attributes->vertices[0], GL_STATIC_DRAW);

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

	//Specify data for model matrices
	glBindBuffer(GL_ARRAY_BUFFER, modelMatricesBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, attributes->faces.size() * sizeof(unsigned int), &attributes->faces[0], GL_STATIC_DRAW);

	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, attributes->faces.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;
}
