#include "Scene2.h"



void Scene2::InitialiseScene() {
	GenerateNewSponge(1.0);
	IncreaseLights();
	std::cout << "FACES SIZE: " << spongeAttributes->faces.size() << std::endl;
}

void Scene2::DeactivateScene() {
	while (!spongeAttributes->cubeList->empty()) {
		delete spongeAttributes->cubeList->front();
		spongeAttributes->cubeList->pop_front();
	}

	spongeAttributes->cubeList->clear();
	numActiveLights = 0;
	spongeLevel = 1;
	numVertices = 0;
	numFaces = 0;
	glUseProgram(0);
	glBindVertexArray(0);
}

void Scene2::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) {
	sceneTwoShader->Use();
	sceneTwoShader->setMat4("view", cameraMatrix);
	sceneTwoShader->setMat4("projection", projectionMatrix);
	sceneTwoShader->setInt("numActiveLights", numActiveLights);
	sceneTwoShader->setBool("isLighting", isLighting);
	sceneTwoShader->setMat4("model", glm::mat4(1.0));

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
			sceneTwoShader->setVec3(lightDirString.c_str(), cameraDirection.x, cameraDirection.y, cameraDirection.z);
			//std::cout << "CAMERA DIRECTION: " << cameraDirection.x << " " << cameraDirection.y << " " << cameraDirection.z << std::endl;

		}
		lightCounter++;
	}
	lightCounter = 0;

	sceneTwoShader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	//Set x axis faces to red
	sceneTwoShader->setVec3("xMaterial.ambient", 0.3, 0.0, 0.0);
	sceneTwoShader->setVec3("xMaterial.diffuse", 0.3, 0.0, 0.0);
	sceneTwoShader->setVec3("xMaterial.specular", 1.0, 1.0, 1.0);
	sceneTwoShader->setFloat("xMaterial.shininess", 128.0);

	//Set y axis faces to green
	sceneTwoShader->setVec3("yMaterial.ambient", 0.0, 0.3, 0.0);
	sceneTwoShader->setVec3("yMaterial.diffuse", 0.0, 0.3, 0.0);
	sceneTwoShader->setVec3("yMaterial.specular", 1.0, 1.0,1.0);
	sceneTwoShader->setFloat("yMaterial.shininess", 128.0);

	//set z axis faces to blue
	sceneTwoShader->setVec3("zMaterial.ambient", 0.0, 0.0, 0.3);
	sceneTwoShader->setVec3("zMaterial.diffuse", 0.0, 0.0, 0.3);
	sceneTwoShader->setVec3("zMaterial.specular", 1.0, 1.0, 1.0);
	sceneTwoShader->setFloat("zMaterial.shininess", 128.0);

	


	DrawSponge(spongeAttributes);

	//Debug method for drawing where the lights are
	/*for (Light* light : *lightList) {
		light->DrawLight(cameraMatrix, projectionMatrix);
	}*/

}
