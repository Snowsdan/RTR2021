//-----------------------------------------------------------------------------
// MainApp.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include "MainApp.h"
#include "RTRShader.h"

int MainApp::Init()
{
    if (int err = RTRApp::Init() != 0) {
        return err;
    }

    // Setup projection matrix and viewport transform. 
    // These won't need to change as we're not worrying about screen size changes for this assignment
    m_ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    // Create two shaders
    // You might want to maintain a vector of all shaders you will use in your assignment
    m_DefaultShader = new RTRShader();
    if (m_DefaultShader->Load("Src/RTRDefault.vert", "Src/RTRDefault.frag", "Src/RTRDefault.geom") != 0) {
        return -1;
    }

    m_PlasmaShader = new RTRShader();
    if (m_PlasmaShader->Load("Src/RTRPlasma.vert", "Src/RTRPlasma.frag") != 0) {
        return -1;
    }

    // Create and initialise camera
    m_Camera = new RTRCamera(glm::vec3(0.0, 12.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    // Create and initialise lighting model
    m_LightingModel = new RTRLightingModel();
    
    // Add directional light to follow camera view direction
    m_LightingModel->AddLight({
        .Type = RTRLightType::DirectionalLight,
        .Ambient = glm::vec3(0.2, 0.2, 0.2),
        .Diffuse = glm::vec3(1.0, 1.0, 1.0),
        .Specular = glm::vec3(0.5, 0.5, 0.5),
        .Direction = glm::vec3(1.0, 0.0, 0.0)
        });
    // Add x-axis red spot lights (palce all at x+3.0 so they are positioned around shaded cube)
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(1.0, 0.0, 0.0),
        .Diffuse = glm::vec3(1.0, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(2.0 + 3.0, 0.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
    });
    


    gameLevel = new GameLevel(m_ProjectionMatrix);

    // Create and initialise the debug console/overlay
    m_Console = new Console();
    m_Console->Init();

    return 0;
}

void MainApp::Done()
{
    m_Console->End(); delete m_Console;
    delete m_PlasmaShader;
    delete m_DefaultShader;
    RTRApp::Done();
}

bool MainApp::Tick()
{
    m_QuitApp = false;
    CheckInput();
    UpdateState(m_TimeDelta);
    RenderFrame();
    return m_QuitApp;
}

// TODO: Add MOUSE support for looking around.
// TODO: Might move all this as "core" functionality in RTRApp class.
void MainApp::CheckInput()
{
    SDL_Event e;
    if (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: m_QuitApp = true; break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: m_QuitApp = true; break;
                    case SDLK_w: m_MovingForward = true; break;
                    case SDLK_s: m_MovingBackward = true; break;
                    case SDLK_a: m_StrafingLeft = true; break;
                    case SDLK_d: m_StrafingRight = true; break;
                    case SDLK_LEFT: m_TurningLeft = true; break;
                    case SDLK_RIGHT: m_TurningRight = true; break;
                    case SDLK_UP: m_TiltingUp = true; break;
                    case SDLK_DOWN: m_TiltingDown = true; break;
                    case SDLK_l: gameLevel->LaunchBall(); break;
                    case SDLK_k: gameLevel->debug = !gameLevel->debug;
                }
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    case SDLK_w: m_MovingForward = false; break;
                    case SDLK_s: m_MovingBackward = false; break;
                    case SDLK_a: m_StrafingLeft = false; break;
                    case SDLK_d: m_StrafingRight = false; break;
                    case SDLK_LEFT: m_TurningLeft = false; break;
                    case SDLK_RIGHT: m_TurningRight = false; break;
                    case SDLK_UP: m_TiltingUp = false; break;
                    case SDLK_DOWN: m_TiltingDown = false; break;
                }
                break;
        
        }
    }
}

void MainApp::UpdateState(unsigned int td_milli)
{
    // Update camera position based on keybard state checked earlier and saved
    if (m_MovingForward == true) m_Camera->MoveForward(td_milli);
    if (m_MovingBackward == true) m_Camera->MoveBackward(td_milli);
    if (m_TurningLeft == true) m_Camera->TurnLeft(td_milli);
    if (m_TurningRight == true) m_Camera->TurnRight(td_milli);
    if (m_StrafingLeft == true) m_Camera->StrafeLeft(td_milli);
    if (m_StrafingRight == true) m_Camera->StrafeRight(td_milli);
    if (m_TiltingUp == true) m_Camera->TiltUp(td_milli);
    if (m_TiltingDown == true) m_Camera->TiltDown(td_milli);

    // Update directional camera to align with camera forward direction
    m_LightingModel->GetLight(0)->Direction = m_Camera->m_Front;

    // Setup Model and View matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ViewMatrix = m_Camera->GetViewMatrix();

}

void MainApp::RenderFrame()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gameLevel->RenderScene((float)m_CurTime, (float)m_TimeDelta, m_ViewMatrix, m_Camera);

    // Print out all debug info
    m_Console->Render("DEBUG", m_FPS,
        m_Camera->m_Position.x, m_Camera->m_Position.y, m_Camera->m_Position.z,
        m_Camera->m_Yaw, m_Camera->m_Pitch);
    // Swap buffers
    SDL_GL_SwapWindow(m_SDLWindow);
}

//-----------------------------------------------------------------------------
static const char* MAIN_WINDOW_TITLE = "RMIT COSC1226: Real-Time Rendering";
int main(int argc, char** argv)
{
    MainApp* app = new MainApp(MAIN_WINDOW_TITLE, false, 1200, 800);
    if (int err = app->Init() != 0) {
        std::cout << "RTR:ERROR: Failed to initialise application. Please see error messages for details..." << std::endl;
        return -1;
    };
    app->Run();
    app->Done();
    return 0;
}
