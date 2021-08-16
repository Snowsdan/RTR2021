//-----------------------------------------------------------------------------
// RTRApp.cpp
//-----------------------------------------------------------------------------
// Assignment 1 Starter Code
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------

#define STB_IMAGE_IMPLEMENTATION
#include "RTRApp.h"

RTRApp::RTRApp(const char* title, bool fullscreen, int width, int height)
{
    m_MainWindowTitle = title;
    fullScreen = fullscreen;
    windowWidth = width;
    windowHeight = height;
    screenWidth = 1920;
	screenHeight = 720;
    m_SDLWindow = nullptr;
    m_SDLRenderer = nullptr;
    m_GLContext = nullptr;
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
        m_SDLWindow = SDL_CreateWindow(
            m_MainWindowTitle.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN);
    }
    else {
        m_SDLWindow = SDL_CreateWindow(
            m_MainWindowTitle.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            SDL_WINDOW_OPENGL);
    }

    m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (m_SDLRenderer == nullptr) {
        std::cerr << "RTR:ERROR: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
        return -2;
    }

    m_GLContext = SDL_GL_CreateContext(m_SDLWindow);

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

	shader = new RTRShader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
	camera = new RTRCamera();

	SDL_CaptureMouse(SDL_TRUE);
	//SDL_WarpMouseInWindow(m_SDLWindow, 400, 300);

	gltInit();
	
	
	return 0;
}

void RTRApp::Run()
{
	Cube* BaseCube = new Cube(1.0, glm::vec3(0,0,0));
	spongeList->splice(spongeList->end(), *GenCubes(BaseCube));
	IncreaseSponge();

	std::cout << "SPONGE SIZE: " << spongeList->size() << std::endl;
    while (!quitApp) {
		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		RenderFrame();
    }
}

void RTRApp::Done()
{
	gltTerminate();

    SDL_GL_DeleteContext(m_GLContext);
    SDL_DestroyRenderer(m_SDLRenderer);
    SDL_DestroyWindow(m_SDLWindow);
    SDL_Quit();
    m_SDLWindow = nullptr;
    m_SDLRenderer = nullptr;
    m_GLContext = nullptr;
}

void RTRApp::RenderFrame()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, windowWidth, windowHeight);
	
	CheckInput();

	//Render Objects
	shader->setMat4("view", camera->GetViewMatrix());

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
		100.0f);
	shader->setMat4("projection", projection);
	shader->Use();

	DrawSponge();
	//RenderOSD();
	
	SDL_GL_SwapWindow(m_SDLWindow);
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
	GLTtext* hello_msg = gltCreateText();
	gltSetText(hello_msg, "Hello World!");

	gltBeginDraw();
	gltColor(0.0f, 1.0f, 0.0f, 1.0f);
	gltDrawText2D(hello_msg, 10, 10, 2.0);
	gltEndDraw();

	gltDeleteText(hello_msg);
}

std::list<Cube*>* RTRApp::GenCubes(Cube* currentCube) {
	std::list<Cube*>* newCubes = new std::list<Cube*>;
	for (float x = -1; x <= 1; x += 1) {
		for (float y = -1; y <= 1; y += 1) {
			for (float z = -1; z <= 1; z += 1) {
				int sum = abs(x) + abs(y) + abs(z);
				if (sum > 1) {
					float size = currentCube->size;
					float newSize = size / 3;

					glm::mat4 modelMat = glm::mat4(1.0f);
					glm::vec3 posVector = glm::vec3(
						currentCube->position.x + x * newSize,
						currentCube->position.y + y * newSize,
						currentCube->position.z + z * newSize);
					modelMat = glm::translate(modelMat, posVector);

					Cube* newCube = new Cube(newSize, posVector);
					//std::cout << newCube->size << std::endl;

					newCubes->push_front(newCube);;
				}
				/*if (sum > 1) {
					float size = 1;
					float newSize = size / 3;
					glm::mat4 modelMat = glm::mat4(1.0f);
					modelMat = glm::translate(modelMat, glm::vec3(x * newSize, y * newSize, z * newSize));
					shader->setMat4("model", modelMat);
					DrawCube(newSize);

					newCubes->push_back();
				}*/

			}
		}
	}
	std::cout << "CUBES SIZE" << newCubes->size() << std::endl;
	return newCubes;
}

void RTRApp::IncreaseSponge() {
	std::list<Cube*>* newCubes = new std::list<Cube*>;

	for (Cube* currentCube : *spongeList) {
		
		newCubes->splice(newCubes->end(),*GenCubes(currentCube));
	}
	std::cout << "NEW CUBES SIZE: " << newCubes->size() << std::endl;
	spongeList = newCubes;
	
}

void RTRApp::DrawSponge() {
	for (Cube* currentCube : *spongeList) {
		//std::cout << "RUN" << std::endl;

		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, currentCube->position);
		shader->setMat4("model", modelMat);
		DrawCube(currentCube->size);

	}
}

//void RTRApp::MergeLists(std::list<Cube*>* firstList, std::list<Cube*>* secondList) {



//}


