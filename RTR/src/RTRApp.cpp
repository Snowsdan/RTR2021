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
	}



	//SDL_DisplayMode dm;
	//if(SDL_GetCurrentDisplayMode(0,&dm))

}

void Run() {

}

void Done() {

}