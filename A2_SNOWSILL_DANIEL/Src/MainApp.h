//-----------------------------------------------------------------------------
// MainApp.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "Console.h"

class MainApp : public RTRApp
{
public:
    MainApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768)
        : RTRApp(title, fullscreen, width, height) 
    { }
    int Init();
    void Done();
    bool Tick();

private:
    bool m_QuitApp{ false };
    RTRShader* m_DefaultShader{ nullptr };
    RTRShader* m_PlasmaShader{ nullptr };
    RTRCamera* m_Camera{ nullptr };
    RTRLightingModel* m_LightingModel{ nullptr };
    RTRObject* m_Cube{ nullptr };
    RTRObject* m_PlasmaCube{ nullptr };
    Console* m_Console{ nullptr };

    bool m_MovingForward{ false };
    bool m_MovingBackward{ false };
    bool m_TurningLeft{ false };
    bool m_TurningRight{ false };
    bool m_StrafingLeft{ false };
    bool m_StrafingRight{ false };
    bool m_TiltingUp{ false };
    bool m_TiltingDown{ false };

    glm::mat4 m_ModelMatrix{ 1.0 };
    glm::mat4 m_ViewMatrix{ 1.0 };
    glm::mat4 m_ProjectionMatrix{ 1.0 };

    void CheckInput();
    void UpdateState(unsigned int td_milli);
    void RenderFrame();
};
