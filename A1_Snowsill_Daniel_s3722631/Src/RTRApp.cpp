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
	scene2 = new Scene2();
	SDL_CaptureMouse(SDL_TRUE);
	SDL_WarpMouseInWindow(sdlWindow, 400, 300);
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

	currentScene = scene2;
	scene2->InitialiseScene();

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
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
		100.0f);


	currentScene->RenderScene(camera->GetViewMatrix(), projection);

	glm::value_ptr(projection);
	
	SDL_GL_SwapWindow(sdlWindow);
}


void RTRApp::DrawCube(float size) {
	float radius = size / 2;
	unsigned int faces[] = {
		//Front face indices
		0, 2,1,
		0, 3, 2,
		//Left face indices
		4, 6, 5,
		4, 7, 6,
		//Back face indices
		9, 10, 8,
		8, 10, 11,
		//Right face indices
		12, 14, 13,
		12, 15, 14,
		////Top face indices
		16, 18, 17,
		16, 19, 18,
		////Bottom face indices
		20, 22, 21,
		20, 23, 22
	};

	float vertices[] = {
		//Front Face
		//     points              colours
		 -radius, radius, radius,    1.0f, 1.0f, 0.0f, //Top Left - 0
		  radius, radius, radius,    0.0f,1.0f, 0.0f, //Top Right - 1
		 radius, -radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 2
		-radius, -radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 3

		//Left Face
		//     points              colours
		 -radius, radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 4
		-radius, radius, radius,    0.0f,1.0f, 0.0f, //Top Right - 5
	   -radius, -radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 6
		-radius, -radius, -radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 7

		//Back Face
		//     points              colours
		-radius, radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 8
		 radius, radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 9
		radius, -radius, -radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 10
	   -radius, -radius, -radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 11

		//Right Face
		//     points              colours
		radius, radius, radius,    1.0f, 1.0f, 0.0f,  //Top Left - 12
	   radius, radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 13
	   radius, -radius,-radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 14
	   radius, -radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 15

		//Top Face
		//     points              colours
		-radius, radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 16
		 radius, radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 17
		  radius, radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 18
		 -radius, radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 19

		//Bottom Face
		//     points              colours
		radius, -radius, -radius,    1.0f, 1.0f, 0.0f,  //Top Left - 20
	   -radius, -radius, -radius,    0.0f,1.0f, 0.0f, //Top Right - 21
		-radius, -radius, radius,    1.0f, 0.0f, 0.0f,//Bottom Right - 22
		 radius, -radius, radius,    0.0f, 0.0f, 1.0f, //Bottom Left - 23
	};


	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int vertexArrayObject = 0;
	unsigned int colourBuffer = 0;
	unsigned int faceElementBuffer = 0;

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Generate vertex array object
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Specify attribute locations
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, sizeof(faces) / sizeof(faces[0]), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;

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
				isLighting = !isLighting;
				//TODO: Add a enable/disable lighting methdo to each scene
				break;
			case SDLK_z:
				isDepthTesting = !isDepthTesting;
				isDepthTesting ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
				break;
			case SDLK_c:
				isBackCulling = !isBackCulling;
				isBackCulling ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
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
	sprintf_s(isLightingBuffer, sizeof(isLightingBuffer), "Lighting: %s", isLighting ? "enabled" : "disabled");
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





