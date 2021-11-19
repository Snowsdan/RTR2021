#pragma once
#include "SceneBase.h"
#include <vector>
struct Scene3Sponge {

	float rotation = 0.0;
	glm::vec3 rotationAxis;
	
	//used for the model matrix for each sponge
	glm::vec3 origin;
	SpongeAtt* sponge = new SpongeAtt();


};

class Scene3 : public SceneBase {
public:
	Scene3() {
		sceneThreeShader = new RTRShader("src/Shaders/Scene3VertexShader.vert", "src/Shaders/Scene3FragmentShader.frag");
		sceneName = "Scene 3";
	}
	void InitialiseScene();
	void DeactivateScene();
	void DrawSponges();
	void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);
	void IncreaseSponge() override;
	void DecreaseSponge() override;
	
	RTRShader* sceneThreeShader;
	
	//Make a new pointer to a vector filled with sponges
	std::vector<Scene3Sponge*>* sponges = new std::vector<Scene3Sponge*>;


private:
	void CreateNewSponges();
	//std::list<Cube*>* GenCubes(Cube* currentCube, float initialAngle, glm::vec3 rotationAxis);
	int lightCounter = 0;


};