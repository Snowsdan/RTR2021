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
#include "Scene2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>




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
    void calcFPS(float timeDelta);

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
    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    SDL_GLContext glContext;

    RTRCamera* camera;


    SceneBase* currentScene;
    SceneBase* scene2;

    bool isLighting = true;
    bool isDepthTesting = true;
    bool isBackCulling = true;
    bool isFullMode = true;
    bool isFirstMove = true;

    int FPS = 0;
    int framesCounter = 0;
    //Timer is in milliseconds
    float frameCalcTimer = 0;
    std::list<Cube*>* spongeList = new std::list<Cube*>;
};

//Following callback method was taken from https://learnopengl.com/In-Practice/Debugging
inline void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

