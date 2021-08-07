#pragma once
#ifndef RTRApp_H
#define RTRApp_H
#define SDL_MAIN_HANDLED
#include "RTRShader.h"
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class RTRApp {
public:
	RTRApp() {
	};
	int Init();
	void Run();
	void Done();
	void DrawSquare();
	void CheckInput();
protected:
	SDL_Window* m_SDLWindow;
	SDL_Renderer* m_SDLRenderer;
	SDL_GLContext m_GLContext;
	RTRShader* shader;
	bool quitApp;
};

#endif