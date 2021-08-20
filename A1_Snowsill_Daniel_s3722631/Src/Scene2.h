#pragma once
#include "SceneBase.h"
class Scene2 : public SceneBase {
	public: 
		Scene2() {
			sceneTwoShader = new RTRShader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
			sceneName = "Scene 2";
			std::cout << "START SCENE 2" << std::endl;
		}
		void InitialiseScene();
		void DrawSponge();
		void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix);
		void DrawCube(float size);

		RTRShader* sceneTwoShader;


};