//-----------------------------------------------------------------------------
// RTRApp.h
//-----------------------------------------------------------------------------
// Assignment 1 Starter Code
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include "RTRShader.h"
#include "RTRCamera.h"
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>

#include "Scene2.h"


//enum Scene {
//    SceneOne,SceneTwo,SceneThree,SceneFour,SceneFive,SceneSix
//};


class RTRApp
{
public:
    RTRApp(const char* title, bool fullscreen, int, int height);
    virtual int Init();
    virtual void Done();
    void Run();
    virtual bool Tick(unsigned int td_milli) {
        std::cout << "RTRApp::Tick()" << std::endl;
        return true;
    }

    void DrawCube(float size);
    void CheckInput();
    void RenderFrame();
    void RenderOSD();

    std::list<Cube*>* GenCubes(Cube* currentCube);
    void IncreaseSponge();
    void DrawSponge();


protected:
    std::string m_MainWindowTitle;
    bool fullScreen;
    int windowWidth;
    int windowHeight;
    int screenWidth;
    int screenHeight;
    bool quitApp;
    float deltaTime = 0;
    float currentTime = 0;
    float lastTime = 0;    
    SDL_Window* m_SDLWindow;
    SDL_Renderer* m_SDLRenderer;
    SDL_GLContext m_GLContext;

    RTRCamera* camera;


    SceneBase* currentScene;
    SceneBase* scene2;


    std::list<Cube*>* spongeList = new std::list<Cube*>;
};



