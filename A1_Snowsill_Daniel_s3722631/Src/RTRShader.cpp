#include "RTRShader.h"

RTRShader::RTRShader(const char* vertexPath, const char* fragmentPath) {
	//strings to store shader code
	std::string vertexShaderString;
	std::string fragmentShaderString;

	//input file streams used to read the source files
	std::ifstream vertShaderFileStream;
	std::ifstream fragShaderFileStream;

	//streams that read file streams as strings
	std::stringstream vertStringStream;
	std::stringstream fragStringStream;

	//Enable exceptions for file streams
	vertShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try{
		vertShaderFileStream.open(vertexPath);
		//Read the buffer form the file streams into the string streams
		vertStringStream << vertShaderFileStream.rdbuf();
		vertShaderFileStream.close();
		vertexShaderString = vertStringStream.str();
	}
	catch(std::ifstream::failure e){
		std::cout << "ERROR: VERTEX SHADER FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	try {
		fragShaderFileStream.open(fragmentPath);
		//Read the buffer form the file streams into the string streams
		fragStringStream << fragShaderFileStream.rdbuf();
		fragShaderFileStream.close();
		fragmentShaderString = fragStringStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR: FRAGMENT SHADER FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	//convert from string to char pointer as OpenGL uses char pointers and not strings
	const char* vertexCode = vertexShaderString.c_str();
	const char* fragCode = fragmentShaderString.c_str();

#pragma region ShaderCompiling
	//Variables for error checking
	int success;				//Indicates success or failure
	char infoLog[512];			//Storage for the error message


	//Vertex Shader
	unsigned int vertexShader = 0;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);

	//Check for errors after compiling shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//Create space for fragment shader and assign the ID
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragCode, NULL);
	glCompileShader(fragmentShader);

	//Check for compilation errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//Start creating and linking shaders into a program
	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
		
	}
	//Delete shaders as they are already linked and no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion
}

void RTRShader::Use() {
	glUseProgram(ID);
}

void RTRShader::setBool(const char* name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void RTRShader::setInt(const char* name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name), value);

}
void RTRShader::setFloat(const char* name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void RTRShader::setVec3(const char* name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}
void RTRShader::setMat4(const char* name, const glm::mat4& mat)const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]);

}

