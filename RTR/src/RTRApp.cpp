#define STB_IMAGE_IMPLEMENTATION
#include "RTRApp.h"



void RTRApp::SayHello() {
	std::cout << "HELLO WORLD!" << std::endl;
}

int RTRApp::Init() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "RTR Error: Failed to initialize the OpenGL context" << std::endl;
		return -1;
	}

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
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return 0;
}

void RTRApp::Run() {
	while (1) {
		DrawSquare();
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

void RTRApp::DrawSquare() {
	//Variables for error checking
	int success;				//Indicates success for failure
	char infoLog[512];			//Storage for the error message


	float vertices[] = {
		-1.0f, 1.0f, 0.0f, //Top Left
		1.0f, 1.0f, 0.0f, //Top Right
		1.0f, -1.0f, 0.0f, //Bottom Right
		-1.0, -1.0f, 0.0f //Bottom Left
	};

	float vertex_colors[]{
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f 
	};

	//Create buffer for the vertices
	unsigned int VertexBuffer;
	//Create 1 buffer and assign it the ID
	glGenBuffers(1, &VertexBuffer);

	//Specify that the buffer is for Vertices
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

	//Copy vertex data from array into buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBuffer), vertices, GL_STATIC_DRAW);

	const char* vertexShaderSource = "#version 400 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aCol;\n"
		"out vec3 vertexColour;\n"
		"void main()\n"
		"{\n"
		" vertexColour = aCol;\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\n";

	//Create space for shader and assign the ID
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check for errors after compiling shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	const char* fragmentShaderSource = "#version 400\n"
		"in vec3 vertexColour;\n"
		"out vec4 fragmentColour;\n"
		"void main() {\n"
		"	fragmentColour = vec4(vertexColour,1.0f);\n"
		"}\n";

	//Create space for fragment shader and assign the ID
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check for compilation errors
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//Create space for the shader program, attach the shaders, and link everything
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	glUseProgram(shaderProgram);

	//Delete shaders as they are already linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	//Create Vertex Array Object to allow control over attributes
	/*unsigned int VertexArray = 0;
	glGenVertexArrays(1, &VertexArray);
	glBindVertexArray(VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glVertexAttribLPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);*/

}