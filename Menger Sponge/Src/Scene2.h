#pragma once
#include "SceneBase.h"


class Scene2 : public SceneBase {
public:
	Scene2() {
		sceneTwoShader = new RTRShader("src/Shaders/Scene2VertexShader.vert", "src/Shaders/Scene2FragmentShader.frag");
		sceneName = "Scene 2";
		std::cout << "START SCENE 2" << std::endl;
	}
	void InitialiseScene();
	void DeactivateScene();
	void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);

	RTRShader* sceneTwoShader;
	

private: 
	int lightCounter = 0;


};