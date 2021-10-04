#include "Scene6.h"



void Scene6::InitialiseScene() {
	GenerateNewSponge(1.0);
	IncreaseLights();
	std::cout << "FACES SIZE: " << spongeAttributes->faces.size() << std::endl;
}

void Scene6::DeactivateScene() {
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

void Scene6::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) {
	sceneSixShader->Use();
	sceneSixShader->setMat4("view", cameraMatrix);
	sceneSixShader->setMat4("projection", projectionMatrix);
	sceneSixShader->setInt("numActiveLights", numActiveLights);
	sceneSixShader->setBool("isLighting", isLighting);
	sceneSixShader->setMat4("model", glm::mat4(1.0));

	for (Light* light : *lightList) {
		std::string lightTypeString = "lights[" + std::to_string(lightCounter) + "].type";
		//std::cout << lightTypeString << std::endl;
		std::string lightAmbientString = "lights[" + std::to_string(lightCounter) + "].ambient";
		std::string lightDiffuseString = "lights[" + std::to_string(lightCounter) + "].diffuse";
		std::string lightSpecString = "lights[" + std::to_string(lightCounter) + "].specular";

		sceneSixShader->setInt(lightTypeString.c_str(), light->lightType);
		sceneSixShader->setVec3(lightAmbientString.c_str(), light->ambient.x, light->ambient.y, light->ambient.z);
		sceneSixShader->setVec3(lightDiffuseString.c_str(), light->diffuse.x, light->diffuse.y, light->diffuse.z);
		sceneSixShader->setVec3(lightSpecString.c_str(), light->specular.x, light->specular.y, light->specular.z);

		if (light->lightType == point) {
			std::string lightPosString = "lights[" + std::to_string(lightCounter) + "].position";
			std::string lightConstString = "lights[" + std::to_string(lightCounter) + "].constant";
			std::string lightLinString = "lights[" + std::to_string(lightCounter) + "].linear";
			std::string lightQuadString = "lights[" + std::to_string(lightCounter) + "].quadratic";

			sceneSixShader->setVec3(lightPosString.c_str(), light->lightPos.x, light->lightPos.y, light->lightPos.z);
			sceneSixShader->setFloat(lightConstString.c_str(), light->constant);
			sceneSixShader->setFloat(lightLinString.c_str(), light->linear);
			sceneSixShader->setFloat(lightQuadString.c_str(), light->quadratic);
		}
		else {
			std::string lightDirString = "lights[" + std::to_string(lightCounter) + "].direction";
			sceneSixShader->setVec3(lightDirString.c_str(), cameraDirection.x, cameraDirection.y, cameraDirection.z);
			//std::cout << "CAMERA DIRECTION: " << cameraDirection.x << " " << cameraDirection.y << " " << cameraDirection.z << std::endl;

		}
		lightCounter++;
	}
	lightCounter = 0;

	sceneSixShader->setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);

	//Set x axis faces to red
	sceneSixShader->setVec3("xMaterial.ambient", 0.3, 0.0, 0.0);
	sceneSixShader->setVec3("xMaterial.diffuse", 0.3, 0.0, 0.0);
	sceneSixShader->setVec3("xMaterial.specular", 1.0, 1.0, 1.0);
	sceneSixShader->setFloat("xMaterial.shininess", 128.0);

	//Set y axis faces to green
	sceneSixShader->setVec3("yMaterial.ambient", 0.0, 0.3, 0.0);
	sceneSixShader->setVec3("yMaterial.diffuse", 0.0, 0.3, 0.0);
	sceneSixShader->setVec3("yMaterial.specular", 1.0, 1.0, 1.0);
	sceneSixShader->setFloat("yMaterial.shininess", 128.0);

	//set z axis faces to blue
	sceneSixShader->setVec3("zMaterial.ambient", 0.0, 0.0, 0.3);
	sceneSixShader->setVec3("zMaterial.diffuse", 0.0, 0.0, 0.3);
	sceneSixShader->setVec3("zMaterial.specular", 1.0, 1.0, 1.0);
	sceneSixShader->setFloat("zMaterial.shininess", 128.0);



	CalcTime();
	DrawSponge(spongeAttributes);

	//Debug method for drawing where the lights are
	/*for (Light* light : *lightList) {
		light->DrawLight(cameraMatrix, projectionMatrix);
	}*/

}

//Sets the time uniform in the vertex shader
void Scene6::CalcTime() {
	float finalTime = sin(time(NULL));
	std::cout << finalTime << std::endl;
	sceneSixShader->setFloat("time", finalTime);
}