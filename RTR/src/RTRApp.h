#pragma once
#ifndef RTRApp_H
#define RTRApp_H
#define SDL_MAIN_HANDLED
#include "RTRShader.h"
#include "RTRCamera.h"
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <iostream>
#include <glad/glad.h>



class RTRApp {
public:
	RTRApp() {
	};
	int Init();
	void Run();
	void Done();
	void DrawCube();
	void CheckInput();

	float deltaTime = 0;
	float currentTime = 0;
	float lastTime = 0;
	bool firstMouse = true;
protected:
	SDL_Window* m_SDLWindow;
	SDL_Renderer* m_SDLRenderer;
	SDL_GLContext m_GLContext;
	RTRShader* shader;
	RTRCamera* camera;
	bool quitApp;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
};

#endif