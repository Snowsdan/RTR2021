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
		void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection);
		void DrawVertices();
		void IncreaseLights() override;

	private:
		//colour arrays used for materials
		float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

};