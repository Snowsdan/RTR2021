#pragma once
#include "SceneBase.h"
#include "Light.h"

class Scene2 : public SceneBase {
public:
	Scene2() {
		sceneTwoShader = new RTRShader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
		sceneName = "Scene 2";
		lightList = new std::list<Light*>;
		lightList->push_back(new Light(
			point,
			glm::vec3(-0.1f, 1.5f, 2.0f),
			glm::vec3(0.0),
			glm::vec3(0.2,0.2,0.2),
			glm::vec3(1.0,1.0,1.0),
			glm::vec3(1.0,1.0,1.0),
			1.0, 0.7,0.032));

		std::cout << "START SCENE 2" << std::endl;
	}
	void InitialiseScene();
	void DrawSponge();
	void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);
	void DrawCube(Cube* cube);

	RTRShader* sceneTwoShader;
	std::list<Light*>* lightList;

private: 
	int lightCounter = 0;


};