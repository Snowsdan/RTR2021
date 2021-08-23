#pragma once
#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // include glad to get the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>
class RTRShader
{
public:
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader
	RTRShader(const char* vertexPath, const char* fragmentPath);


	void Use();

	// utility uniform functions
	void setBool(const char* name, bool value) const;
	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVec3(const char* name, float x, float y, float z)const;
	void setMat4(const char* name, const glm::mat4& mat)const;

	

};

#endif
