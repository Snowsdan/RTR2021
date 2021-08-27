#pragma once
#include "SceneBase.h"
#include <vector>
struct Scene4Sponge {

	float rotation = 0.0;
	glm::vec3 rotationAxis;

	//used for the model matrix for each sponge
	glm::vec3 origin;
	SpongeAtt* sponge = new SpongeAtt();


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
	void DrawSponge(SpongeAtt* attributes) override;
	void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);
	void IncreaseSponge() override;
	void DecreaseSponge() override;

	RTRShader* sceneFourShader;

	//Make a new pointer to a vector filled with sponges
	std::vector<Scene4Sponge*>* sponges = new std::vector<Scene4Sponge*>;
	std::vector<glm::mat4>* modelMats = new std::vector<glm::mat4>;

private:
	void CreateNewSponges();
	//std::list<Cube*>* GenCubes(Cube* currentCube, float initialAngle, glm::vec3 rotationAxis);
	int lightCounter = 0;


};