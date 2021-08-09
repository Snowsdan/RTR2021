#define STB_IMAGE_IMPLEMENTATION
#include "RTRApp.h"
//#include "RTRShader.h"




int RTRApp::Init() {
	m_SDLWindow = nullptr;
	m_SDLRenderer = nullptr;
	m_GLContext = nullptr;
	quitApp = false;
	

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "RTR Error: Failed to initialize the OpenGL context" << std::endl;
		return -1;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16); 
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	m_SDLWindow = SDL_CreateWindow(
		"Real Time Rendering",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_OPENGL);

	m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_SDLRenderer == nullptr) {
		std::cerr << "RTR Error: SDL2 Renderer couldn't be created. Error: " << SDL_GetError() << std::endl;
		return -2;
	}

	m_GLContext = SDL_GL_CreateContext(m_SDLWindow);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cerr << "RTR Error: Failed to initialize the OpenGL Context" << std::endl;
		return -3;
	}

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	std::cout << "RTR:MESSAGE: OpenGL version " << GLVersion.major << "." << GLVersion.minor << " initialised." << std::endl;

	shader = new RTRShader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
	camera = new RTRCamera();

	SDL_CaptureMouse(SDL_TRUE);
	SDL_WarpMouseInWindow(m_SDLWindow, 400, 300);
	return 0;


}

void RTRApp::Run() {

	while (!quitApp) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentTime = SDL_GetTicks();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		
		CheckInput();

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f)
		};

		shader->setMat4("view", camera->GetViewMatrix());

		//Make multiple cubes and give them different translations
		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
			glm::mat4 modelMat = glm::mat4(1.0f);
			modelMat = glm::translate(modelMat, cubePositions[i]);
			//modelMat = glm::rotate(modelMat, ((float)SDL_GetTicks() / 2000) * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0));
			shader->setMat4("model", modelMat);

			DrawCube();

		}


		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,
			100.0f);
		shader->setMat4("projection", projection);

		shader->Use();
		SDL_GL_SwapWindow(m_SDLWindow);
	}
}

void RTRApp::Done() {
	SDL_GL_DeleteContext(m_GLContext);
	SDL_DestroyRenderer(m_SDLRenderer);
	SDL_DestroyWindow(m_SDLWindow);
	SDL_Quit();
	m_SDLWindow = nullptr;
	m_SDLRenderer = nullptr;
	m_GLContext = nullptr;
}

void RTRApp::CheckInput(){

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

void RTRApp::DrawCube() {

	//Variables for error checking
	int success;				//Indicates success or failure
	char infoLog[512];			//Storage for the error message

	unsigned int faces[] = {
		//Front face indices
		0, 2,1,  	
		0, 3, 2,
		//Left face indices
		4, 6, 5,
		4, 7, 6,
		//Back face indices
		8, 10, 9,
		8, 11, 10,
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
		 -0.5f, 0.5f, 0.5f,    1.0f, 1.0f, 0.0f, //Top Left - 0
		  0.5f, 0.5f, 0.5f,    0.0f,1.0f, 0.0f, //Top Right - 1
		 0.5f, -0.5f, 0.5f,    1.0f, 0.0f, 0.0f,//Bottom Right - 2
		-0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f, //Bottom Left - 3

		//Left Face
		//     points              colours
		 -0.5f, 0.5f, -0.5f,    1.0f, 1.0f, 0.0f,  //Top Left - 4
		-0.5f, 0.5f, 0.5f,    0.0f,1.0f, 0.0f, //Top Right - 5
	   -0.5f, -0.5f, 0.5f,    1.0f, 0.0f, 0.0f,//Bottom Right - 6
		-0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f, //Bottom Left - 7

		//Back Face
		//     points              colours
		-0.5f, 0.5f, -0.5f,    1.0f, 1.0f, 0.0f,  //Top Left - 8
	     0.5f, 0.5f, -0.5f,    0.0f,1.0f, 0.0f, //Top Right - 9
	    0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,//Bottom Right - 10
	   -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f, //Bottom Left - 11

		//Right Face
		//     points              colours
		0.5f, 0.5f, 0.5f,    1.0f, 1.0f, 0.0f,  //Top Left - 12
	   0.5f, 0.5f, -0.5f,    0.0f,1.0f, 0.0f, //Top Right - 13
	   0.5f, -0.5f,-0.5f,    1.0f, 0.0f, 0.0f,//Bottom Right - 14
	   0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f, //Bottom Left - 15

		//Top Face
		//     points              colours
		-0.5f, 0.5f, -0.5f,    1.0f, 1.0f, 0.0f,  //Top Left - 16
		 0.5f, 0.5f, -0.5f,    0.0f,1.0f, 0.0f, //Top Right - 17
		  0.5f, 0.5f, 0.5f,    1.0f, 0.0f, 0.0f,//Bottom Right - 18
		 -0.5f, 0.5f, 0.5f,    0.0f, 0.0f, 1.0f, //Bottom Left - 19

		//Bottom Face
		//     points              colours
		0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f,  //Top Left - 20
	   -0.5f, -0.5f, -0.5f,    0.0f,1.0f, 0.0f, //Top Right - 21
		-0.5f, -0.5f, 0.5f,    1.0f, 0.0f, 0.0f,//Bottom Right - 22
		 0.5f, -0.5f, 0.5f,    0.0f, 0.0f, 1.0f, //Bottom Left - 23
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



	

	
	




