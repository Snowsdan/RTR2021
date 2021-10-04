#pragma once
#pragma once
#include "SceneBase.h"


class Scene6 : public SceneBase {
public:
	Scene6() {
		sceneSixShader = new RTRShader("src/Shaders/Scene6VertexShader.vert", "src/Shaders/Scene6FragmentShader.frag");
		sceneName = "Scene 2";
		std::cout << "START SCENE 2" << std::endl;
	}
	void InitialiseScene();
	void DeactivateScene();
	void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);
	void CalcTime();
	RTRShader* sceneSixShader;


private:
	int lightCounter = 0;


};