#include "Scene3.h"

void Scene3::InitialiseScene() {
	std::cout << "START SCENE 3" << std::endl;
	CreateNewSponges();

	

	IncreaseLights();
}

void Scene3::DeactivateScene() {
	sponges->clear();
	numActiveLights = 0;
	spongeLevel = 1;
	numVertices = 0;
	numFaces = 0;
	glUseProgram(0);
	glBindVertexArray(0);
}

void Scene3::DrawSponges() {

	for(Scene3Sponge* currentSponge : *sponges){
		currentSponge->rotation = currentSponge->rotation + 1.0f;
		float rotation = currentSponge->rotation;
		
		glm::mat4 model = glm::mat4(1.0);

		model = glm::translate(model, currentSponge->origin);
		model = glm::rotate(model, glm::radians(currentSponge->rotation), currentSponge->rotationAxis);


		sceneThreeShader->setMat4("model", model);
		DrawSponge(currentSponge->sponge);

	}

}

void Scene3::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) {

	sceneThreeShader->Use();
	sceneThreeShader->setMat4("view", cameraMatrix);
	sceneThreeShader->setMat4("projection", projectionMatrix);
	sceneThreeShader->setInt("numActiveLights", numActiveLights);
	sceneThreeShader->setBool("isLighting", isLighting);


	for (Light* light : *lightList) {
		std::string lightTypeString = "lights[" + std::to_string(lightCounter) + "].type";
		//std::cout << lightTypeString << std::endl;
		std::string lightAmbientString = "lights[" + std::to_string(lightCounter) + "].ambient";
		std::string lightDiffuseString = "lights[" + std::to_string(lightCounter) + "].diffuse";
		std::string lightSpecString = "lights[" + std::to_string(lightCounter) + "].specular";

		sceneThreeShader->setInt(lightTypeString.c_str(), light->lightType);
		sceneThreeShader->setVec3(lightAmbientString.c_str(), light->ambient.x, light->ambient.y, light->ambient.z);
		sceneThreeShader->setVec3(lightDiffuseString.c_str(), light->diffuse.x, light->diffuse.y, light->diffuse.z);
		sceneThreeShader->setVec3(lightSpecString.c_str(), light->specular.x, light->specular.y, light->specular.z);

		if (light->lightType == point) {
			std::string lightPosString = "lights[" + std::to_string(lightCounter) + "].position";
			std::string lightConstString = "lights[" + std::to_string(lightCounter) + "].constant";
			std::string lightLinString = "lights[" + std::to_string(lightCounter) + "].linear";
			std::string lightQuadString = "lights[" + std::to_string(lightCounter) + "].quadratic";

			sceneThreeShader->setVec3(lightPosString.c_str(), light->lightPos.x, light->lightPos.y, light->lightPos.z);
			sceneThreeShader->setFloat(lightConstString.c_str(), light->constant);
			sceneThreeShader->setFloat(lightLinString.c_str(), light->linear);
			sceneThreeShader->setFloat(lightQuadString.c_str(), light->quadratic);
		}
		else {
			std::string lightDirString = "lights[" + std::to_string(lightCounter) + "].direction";
			sceneThreeShader->setVec3(lightDirString.c_str(), cameraDirection.x, cameraDirection.y, cameraDirection.z);
			//std::cout << "CAMERA DIRECTION: " << cameraDirection.x << " " << cameraDirection.y << " " << cameraDirection.z << std::endl;

		}
		lightCounter++;
	}
	lightCounter = 0;

	sceneThreeShader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	//Set x axis faces to red
	sceneThreeShader->setVec3("xMaterial.ambient", 0.3, 0.0, 0.0);
	sceneThreeShader->setVec3("xMaterial.diffuse", 0.3, 0.0, 0.0);
	sceneThreeShader->setVec3("xMaterial.specular", 1.0, 1.0, 1.0);
	sceneThreeShader->setFloat("xMaterial.shininess", 128.0);

	//Set y axis faces to green
	sceneThreeShader->setVec3("yMaterial.ambient", 0.0, 0.3, 0.0);
	sceneThreeShader->setVec3("yMaterial.diffuse", 0.0, 0.3, 0.0);
	sceneThreeShader->setVec3("yMaterial.specular", 1.0, 1.0, 1.0);
	sceneThreeShader->setFloat("yMaterial.shininess", 128.0);

	//set z axis faces to blue
	sceneThreeShader->setVec3("zMaterial.ambient", 0.0, 0.0, 0.3);
	sceneThreeShader->setVec3("zMaterial.diffuse", 0.0, 0.0, 0.3);
	sceneThreeShader->setVec3("zMaterial.specular", 1.0, 1.0, 1.0);
	sceneThreeShader->setFloat("zMaterial.shininess", 128.0);




	DrawSponges();
	/*Cube* cb = new Cube(1.0 / 3.0, glm::vec3(0, 0, 0));
	sceneThreeShader->setMat4("model", cb->modelMat);
	DrawCube(cb);*/
	//std::cout << "FIRST CUBE: " << sponges->front()->front()->position.x << std::endl;

	//Debug method for drawing where the lights are
	/*for (Light* light : *lightList) {
		light->DrawLight(cameraMatrix, projectionMatrix);
	}*/

}



void Scene3::IncreaseSponge() {

	for (Scene3Sponge* currentSponge : *sponges) {
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

void Scene3::DecreaseSponge() {
	if (spongeLevel > 1) {
		numFaces = 0;
		numVertices = 0;

		for (Scene3Sponge* currentSponge : *sponges) {
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

void Scene3::CreateNewSponges() {
	srand((unsigned)time(NULL));

	for (float x = -1; x <= 1; x++) {
		for (float y = -1; y <= 1; y++) {
			//std::list<Cube*>* sponge = new std::list<Cube*>;
			Scene3Sponge* spongeInfo = new Scene3Sponge;

			//Create a sponge at level 1
			float xPos = x / 3.0 * 2.0;
			float yPos = y / 3.0 * 2.0;

			spongeInfo->origin = glm::vec3(xPos, yPos, 0.0);

			/*td::cout << "xPos: " << xPos << std::endl;
			std::cout << "yPos: " << yPos << std::endl << std::endl;*/

			//Multiply by 1.5 to spread them out
			Cube* baseCube = new Cube(1.0 / 3.0, glm::vec3(0 , 0 , 0));
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

