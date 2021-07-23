#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "RTRApp.h"
#include <sdl/SDL.h>


void RTRApp::SayHello() {
	std::cout << "HELLO WORLD!" << std::endl;
}

int RTRApp::Init() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "RTR Error: Failed to initialize the OpenGL context" << std::endl;
		return -1;
	}

	m_SDLWindow = SDL_CreateWindow(
		"Real Time Rendering",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL);

	m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_SDLRenderer == nullptr) {
		std::cerr << "RTR Error: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
		return -2;
	}

	m_GLContext = SDL_GL_CreateContext(m_SDLWindow);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "RTR Error: Failed to initialize the OpenGL Context" << std::endl;
		return -3;
	}
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return 0;
}

void Run() {
	while (1) {}
}

void Done() {
	
}