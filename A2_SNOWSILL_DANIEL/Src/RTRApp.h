//-----------------------------------------------------------------------------
// RTRApp.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>

class RTRApp
{
public:
    RTRApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768)
        : m_MainWindowTitle{ title }
        , m_FullScreen{ fullscreen }
        , m_WindowWidth{ width }
        , m_WindowHeight{ height }
    { }
    virtual int Init();
    virtual void Done();
    void Run();
    virtual bool Tick() { return true; }

protected:
    std::string m_MainWindowTitle;
    bool m_FullScreen;
    int m_WindowWidth;
    int m_WindowHeight;
    int m_ScreenWidth{ 0 };
    int m_ScreenHeight{ 0 };
    SDL_Window* m_SDLWindow{ nullptr };
    SDL_Renderer* m_SDLRenderer{ nullptr };
    SDL_GLContext m_GLContext{ nullptr };
    unsigned int m_CurTime{ 0 };
    unsigned int m_TimeDelta{ 0 };
    int m_FPS{ 0 };
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