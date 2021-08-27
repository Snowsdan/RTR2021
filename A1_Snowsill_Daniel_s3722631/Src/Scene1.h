#pragma once

#include "Scene1.h"
#include "SceneBase.h"

class Scene1 : public SceneBase {
	public:
		Scene1() {
			sceneName = "Scene 1";
		}
		void InitialiseScene();
		void DeactivateScene();
		void DrawSponge();
		void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);
		void DrawCube(Cube* cube);

	protected:


};