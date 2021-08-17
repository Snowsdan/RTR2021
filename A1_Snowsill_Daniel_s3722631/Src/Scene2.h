#pragma once
#include "SceneBase.h"
class Scene2 : public SceneBase {
	public: 
		void InitialiseScene();
		void DrawSponge();
		void RenderScene();
		void DrawCube(float size);

		RTRShader* shader;


};