#pragma once
#ifndef RTRApp_H
#define RTRApp_H
#define SDL_MAIN_HANDLED
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <sdl/SDL.h>
#include <iostream>
#include <glad/glad.h>

class RTRApp {
public:
	RTRApp() {
		m_SDLWindow = nullptr;
		m_SDLRenderer = nullptr;
		m_GLContext = nullptr;
	};
	int Init();
	void Run();
	void Done();
	int DrawSquare();
protected:
	SDL_Window* m_SDLWindow;
	SDL_Renderer* m_SDLRenderer;
	SDL_GLContext m_GLContext;

};

#endif