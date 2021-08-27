#pragma once
#include "SceneBase.h"
#include <vector>
struct MatrixInfo {

	float rotation = 0.0;
	glm::vec3 rotationAxis;
	glm::mat4 model;

};

class Scene4 : public SceneBase {
public:
	Scene4() {
		sceneFourShader = new RTRShader("src/Shaders/Scene4VertexShader.vert", "src/Shaders/Scene4FragmentShader.frag");
		sceneName = "Scene 4";
	}
	void InitialiseScene();
	void DeactivateScene();
	void DrawSponges();
	void IncreaseSponge() override;
	void DecreaseSponge() override;
	void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);

	RTRShader* sceneFourShader;

	//Make a new pointer to a vector filled with sponges
	std::vector<MatrixInfo>* modelMats = new std::vector<MatrixInfo>;
	std::vector<glm::mat4>* modelMatrices = new std::vector<glm::mat4>;

	glm::mat4 matrices[9];
	glm::vec3 axisRotations[9];


private:
	const int numSponges = 9;
	void UpdateModelMats();
	void CreateNewMatrices();
	int lightCounter = 0;


};