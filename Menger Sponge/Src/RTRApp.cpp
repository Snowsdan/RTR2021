//-----------------------------------------------------------------------------
// RTRApp.cpp
//-----------------------------------------------------------------------------
// Assignment 1 Starter Code
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION
#include "RTRApp.h"

#define GLT_IMPLEMENTATION
#include <gltext/gltext.h>

RTRApp::RTRApp(const char* title, bool fullscreen, int width, int height)
{
    m_MainWindowTitle = title;
    fullScreen = fullscreen;
    windowWidth = width;
    windowHeight = height;
    screenWidth = 1920;
	screenHeight = 720;
    sdlWindow = nullptr;
    sdlRenderer = nullptr;
    glContext = nullptr;
}

int RTRApp::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "RTR:ERROR: SDL2 video subsystem couldn't be initialized. Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (fullScreen == true) {
        sdlWindow = SDL_CreateWindow(
            m_MainWindowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    }
    else {
        sdlWindow = SDL_CreateWindow(
            m_MainWindowTitle.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            SDL_WINDOW_OPENGL);
    }

    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (sdlRenderer == nullptr) {
        std::cerr << "RTR:ERROR: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
        return -2;
    }

    glContext = SDL_GL_CreateContext(sdlWindow);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "RTR:ERROR: Failed to initialize the OpenGL context." << std::endl;
        return -3;
    }

    SDL_DisplayMode dm;
    if (SDL_GetCurrentDisplayMode(0, &dm) == 0) {
        screenWidth = dm.w;
        screenHeight = dm.h;
        if (fullScreen == true) {
            windowWidth = screenWidth;
            windowHeight = screenHeight;
        }
    }
    else {
        std::cerr << "RTR:WARNING: SDL coudn't retrieve current display mode details." << std::endl;
    }

    std::cout << "RTR:MESSAGE: OpenGL version " << GLVersion.major << "." << GLVersion.minor << " initialised." << std::endl;
    
	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	
	camera = new RTRCamera();
	scene1 = new Scene1();
	scene2 = new Scene2();
	scene3 = new Scene3();
	scene4 = new Scene4();
	scene6 = new Scene6();

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_CaptureMouse(SDL_TRUE);

	//Enable or disable vsync
	//SDL_GL_SetSwapInterval(0);

	//SDL_WarpMouseInWindow(sdlWindow, 400, 300);


	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	
	if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
		return EXIT_FAILURE;
	}
	
	return 0;
}



void RTRApp::Run()
{

	currentScene = scene1;
	currentScene->InitialiseScene();

    while (!quitApp) {
		//SDL_GetTicks returns milliseconds
		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		
		calcFPS(deltaTime);
		RenderFrame();
		
    }
}

void RTRApp::Done()
{
	gltTerminate();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
    sdlWindow = nullptr;
    sdlRenderer = nullptr;
    glContext = nullptr;
}

void RTRApp::RenderFrame()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);

	RenderOSD();
	CheckInput(); 
	
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f,
		100.0f);

	currentScene->RenderScene(camera->GetViewMatrix(), projection, camera->cameraPos, camera->direction);
	
	SDL_GL_SwapWindow(sdlWindow);
}



void RTRApp::CheckInput()
{
	
	SDL_Event event;
	//Returns 1(true) if there is an event in the queue
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				quitApp = true;
				break;
			case SDLK_w:
				camera->MoveCameraForward(deltaTime);
				break;
			case SDLK_a:
				camera->MoveCameraLeft(deltaTime);
				break;
			case SDLK_s:
				camera->MoveCameraBack(deltaTime);
				break;
			case SDLK_d:
				camera->MoveCameraRight(deltaTime);
				break;
			case SDLK_EQUALS:
				currentScene->IncreaseSponge();
				break;
			case SDLK_MINUS:
				currentScene->DecreaseSponge();
				break;
			case SDLK_h:
				isFullMode = !isFullMode;
				break;
			case SDLK_l:
				currentScene->isLighting = !currentScene->isLighting;
				//TODO: Add a enable/disable lighting method to each scene
				break;
			case SDLK_z:
				isDepthTesting = !isDepthTesting;
				isDepthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
				break;
			case SDLK_c:
				isBackCulling = !isBackCulling;
				isBackCulling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
				break;
			case SDLK_PERIOD:
				currentScene->IncreaseLights();
				break;
			case SDLK_COMMA:
				currentScene->DecreaseLights();
				break;
			case SDLK_1:
				if (currentScene != scene1) {
					currentScene->DeactivateScene();
					currentScene = scene1;
					camera->ResetCamera();
					currentScene->InitialiseScene();
				}
				
				break;
			case SDLK_2:
				if (currentScene != scene2) {
					currentScene->DeactivateScene();
					currentScene = scene2;
					camera->ResetCamera();
					currentScene->InitialiseScene();
				}
				break;
			case SDLK_3:
				if (currentScene != scene3) {
					currentScene->DeactivateScene();
					currentScene = scene3;
					camera->ResetCamera();
					currentScene->InitialiseScene();
				}
				break;
			case SDLK_4:
				if (currentScene != scene4) {
					currentScene->DeactivateScene();
					currentScene = scene4;
					camera->ResetCamera();
					currentScene->InitialiseScene();
				}
				break;
			case SDLK_6:
				if (currentScene != scene6) {
					currentScene->DeactivateScene();
					currentScene = scene6;
					camera->ResetCamera();
					currentScene->InitialiseScene();
				}
				break;
			} 
		case SDL_MOUSEMOTION:
			
			camera->RotateCamera();
			break;
		}
	}
}

// Render On-Screen Display
void RTRApp::RenderOSD()
{
	SDL_DisplayMode displayStats = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
	if (SDL_GetDisplayMode(0, 0, &displayStats) != 0) {

		std::cout << "ERROR: " << SDL_GetError() << std::endl;
	}
	

	//Create text for FPS
	char fpsBuffer[100];
	sprintf_s(fpsBuffer, sizeof(fpsBuffer), "FPS: %i", FPS);
	GLTtext* fpsText = gltCreateText();
	gltSetText(fpsText, fpsBuffer);

	//Create text for the scene name
	GLTtext* sceneNameText = gltCreateText();
	gltSetText(sceneNameText, currentScene->GetSceneName());

	//Create text for the resolution
	char displayInfo[100];
	sprintf_s(displayInfo,sizeof(displayInfo),"Resolution: %i x %i", displayStats.w, displayStats.h);
	GLTtext* displayText = gltCreateText();
	gltSetText(displayText, displayInfo);

	//Create text for the refresh rate
	char refreshInfo[100];
	sprintf_s(refreshInfo, sizeof(refreshInfo), "Refresh Rate: %i",displayStats.refresh_rate);
	GLTtext* refreshRateText = gltCreateText();
	gltSetText(refreshRateText, refreshInfo);

	//Create text for the sponge level
	char spongeLevel[100];
	sprintf_s(spongeLevel, sizeof(spongeLevel), "Sponge Level: %i", currentScene->GetSpongeLevel());
	GLTtext* currentLevelText = gltCreateText();
	gltSetText(currentLevelText, spongeLevel);

	//Create text for the vertex count
	char vertexCount[100];
	sprintf_s(vertexCount, sizeof(vertexCount), "Vertex Count: %i", currentScene->GetNumVertices());
	GLTtext* vertexText = gltCreateText();
	gltSetText(vertexText, vertexCount);

	//Create text for the amount of faces
	char faceCount[100];
	sprintf_s(faceCount, sizeof(faceCount), "Face Count: %i", currentScene->GetNumFaces());
	GLTtext* faceCountText = gltCreateText();
	gltSetText(faceCountText, faceCount);

	//Create text to show if lighting is enabled
	char isLightingBuffer[100];
	sprintf_s(isLightingBuffer, sizeof(isLightingBuffer), "Lighting: %s", currentScene->isLighting ? "enabled" : "disabled");
	GLTtext* lightingText = gltCreateText();
	gltSetText(lightingText, isLightingBuffer);

	//Create text to show if depth testing is enabled
	char isDepthBuffer[100];
	sprintf_s(isDepthBuffer, sizeof(isDepthBuffer), "Depth Testing: %s", isDepthTesting ? "enabled" : "disabled");
	GLTtext* depthTestText = gltCreateText();
	gltSetText(depthTestText, isDepthBuffer);

	//Create text to show if back culling is enabled
	char isCullingBuffer[200];
	sprintf_s(isCullingBuffer, sizeof(isCullingBuffer), "Back Face Culling: %s", isBackCulling ? "enabled" : "disabled");
	GLTtext* backCullingText = gltCreateText();
	gltSetText(backCullingText, isCullingBuffer);

	gltBeginDraw();
	gltColor(0.0f, 1.0f, 0.0f, 1.0f);
	gltDrawText2DAligned(fpsText, 1, 1, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(sceneNameText, 1, 20, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(displayText, 1, 40, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(refreshRateText, 1, 60, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(currentLevelText, 1, 80, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(vertexText, 1, 100, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(faceCountText, 1, 120, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(lightingText, 1, 140, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(depthTestText, 1, 160, 1.0, GLT_LEFT, GLT_TOP);
	gltDrawText2DAligned(backCullingText, 1, 180, 1.0, GLT_LEFT, GLT_TOP);
	gltEndDraw();

	gltDeleteText(fpsText);
	gltDeleteText(sceneNameText);
	gltDeleteText(displayText);
	gltDeleteText(refreshRateText);
	gltDeleteText(currentLevelText);
	gltDeleteText(vertexText);
	gltDeleteText(faceCountText);
	gltDeleteText(lightingText);
	gltDeleteText(depthTestText);
	gltDeleteText(backCullingText);
}

void RTRApp::calcFPS(float timeDelta) {
	framesCounter++;
	frameCalcTimer += (int)timeDelta;
	//std::cout << "NUMBER OF FRAMES: " << framesCounter << std::endl;
	if (framesCounter >= 100) {
		FPS = framesCounter / (frameCalcTimer / 1000);

		framesCounter = 0;
		frameCalcTimer = 0;
	}
}







