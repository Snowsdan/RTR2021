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

	return 0;


}

void RTRApp::Run() {
	//RTRShader* shader = new RTRShader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
	while (!quitApp) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CheckInput();

		shader->Use();
		DrawSquare();
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
	const Uint8* keys;
	SDL_PumpEvents();
	if (keys = SDL_GetKeyboardState(nullptr)) {
		if (keys[SDL_SCANCODE_ESCAPE]) {
			quitApp = true;
		}
	}
}

void RTRApp::DrawSquare() {

	//Variables for error checking
	int success;				//Indicates success or failure
	char infoLog[512];			//Storage for the error message

	unsigned int faces[] = {
		0, 2,1,  	//First Triangle
		0, 3, 2

	};

	float vertices[] = {
		//     points              colours
		 -0.5f, 0.5f, 0.0f,    1.0f, 1.0f, 0.0f,  //Top Left
		  0.5f, 0.5f, 0.0f,    0.0f,1.0f, 0.0f, //Top Right
		 0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,//Bottom Right
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, //Bottom Left
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

	//Setting Uniforms MUST happen after glUseProgram()
	//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	glm::mat4 transformMat = glm::mat4(1.0f);
	//transformMat = glm::rotate(transformMat, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	transformMat = glm::scale(transformMat, glm::vec3(0.5, 0.5, 0.5));
	shader->setMat4("transformMat", transformMat);


	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;

}

