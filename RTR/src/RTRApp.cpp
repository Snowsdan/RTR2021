#define STB_IMAGE_IMPLEMENTATION
#include "RTRApp.h"


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


	return 0;


}

void RTRApp::Run() {
	//while (!quitApp) {
		//CheckInput();
		DrawSquare();

	//}
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


	float vertexPoints[] = {
		-0.5f, 0.5f, 0.0f, //Top Left
		0.5f, 0.5f, 0.0f, //Top Right
		0.5f, -0.5f, 0.0f, //Bottom Right
		-0.5f, -0.5f, 0.0f //Bottom Left
	};

	float vertexColours[] = {
		0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f

	};

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

	//Send vertex colour data to buffer
	/*glGenBuffers(1, &colourBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);*/

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); 
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faces), faces, GL_STATIC_DRAW);

	

#pragma region Shaders
	const char* vertexShaderSource = "#version 400\n"
		"layout (location = 0) in vec3 vertexPoints;\n"
		"layout (location = 1) in vec3 vertexCol;\n"
		"out vec3 vertexColour;\n"
		"void main()\n"
		"{\n"
		" vertexColour = vertexCol;\n"
		" gl_Position = vec4(vertexPoints.x, vertexPoints.y, vertexPoints.z, 1.0);\n"
		"}\0";

	//Create space for shader and assign the ID
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	//check for errors after compiling shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	const char* fragmentShaderSource = "#version 400\n"
		"in vec3 vertexColour;\n"
		"out vec4 fragmentColour;\n"
		"void main() {\n"
		"	fragmentColour = vec4(vertexColour,1.0f);\n"
		"}\0";

	//Create space for fragment shader and assign the ID
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	//Check for compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
		//return -1;
	}

	//Create space for the shader program, attach the shaders, and link everything
	unsigned int shaderProgram = 0;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
		//return -2;
	}

	//Delete shaders as they are already linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion

	while (!quitApp) {
		CheckInput();
		//glClearColor(0.5, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);

		glBindVertexArray(vertexArrayObject);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//Clean the current buffer
		glBindVertexArray(0);

		SDL_GL_SwapWindow(m_SDLWindow);
	}
	

	
	
	//std::cout << "SHAPE DRAWN" << std::endl;

	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	//glDeleteBuffers(1, &colourBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	glDeleteProgram(shaderProgram);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	//colourBuffer = 0;
	faceElementBuffer = 0;
	shaderProgram = 0;

}

