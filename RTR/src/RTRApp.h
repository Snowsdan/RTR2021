#pragma once
#ifndef RTRApp_H
#define RTRApp_H

class RTRApp {
public:
	RTRApp() {};
	void SayHello();
	int Init();
	void Run();
	void Done();
protected:
	SDL_Window* m_SDLWindow;
	SDL_Renderer* m_SDLRenderer;
	SDL_GLContext m_GLContext;

};

#endif