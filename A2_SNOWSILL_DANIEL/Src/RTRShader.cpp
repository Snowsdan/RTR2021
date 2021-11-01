//-----------------------------------------------------------------------------
// RTRShader.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "RTRShader.h"

static char *load_shader_file(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (fp == nullptr) return nullptr;
    fseek(fp, 0L, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0L, SEEK_SET);
    char *buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = 0;
    fclose(fp);
    return buf;
}

int RTRShader::Load(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
    //Strings that store the shader code
    const char* vertexCode = nullptr;
    const char* fragCode = nullptr;
    const char* geoCode = nullptr;

    //strings to store shader code
    std::string vertexShaderString;
    std::string fragmentShaderString;
    std::string geometryShaderString;

    //input file streams used to read the source files
    std::ifstream vertShaderFileStream;
    std::ifstream fragShaderFileStream;
    std::ifstream geoShaderFileStream;

    //streams that read file streams as strings
    std::stringstream vertStringStream;
    std::stringstream fragStringStream;
    std::stringstream geoStringStream;

    //Enable exceptions for file streams
    vertShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geoShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertShaderFileStream.open(vertexPath);
        //Read the buffer form the file streams into the string streams
        vertStringStream << vertShaderFileStream.rdbuf();
        vertShaderFileStream.close();
        vertexShaderString = vertStringStream.str();
    }
    catch (std::ifstream::failure e) {
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

    //Do what we did above but check for geometry shader first
    if (geometryPath != nullptr) {

        try {
            geoShaderFileStream.open(geometryPath);
            //Read the buffer form the file streams into the string streams
            geoStringStream << geoShaderFileStream.rdbuf();
            geoShaderFileStream.close();
            geometryShaderString = geoStringStream.str();
            geoCode = geometryShaderString.c_str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR: Geometry SHADER FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
       
    }

    //convert from string to char pointer as OpenGL uses char pointers and not strings
    vertexCode = vertexShaderString.c_str();
    fragCode = fragmentShaderString.c_str();


#pragma region ShaderCompiling
    //Variables for error checking
    int success;				//Indicates success or failure
    char infoLog[512];			//Storage for the error message
    unsigned int vertexShader = 0;
    unsigned int fragmentShader = 0;
    unsigned int geometryShader = 0;

    if (vertexCode != nullptr) {
        //Vertex Shader
        
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
    }
    

    if (fragCode != nullptr) {
        //Fragment Shader
        
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragCode, NULL);
        glCompileShader(fragmentShader);

        //Check for compilation errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            std::cout << "RTR:ERROR:RTR_Shader:Could not load FRAGMENT shader file - " << fragmentPath << std::endl;
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
        }
    }
    

    //Geometry Shader
    if (geoCode != nullptr) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &geoCode, NULL);
        glCompileShader(geometryShader);

        //Check for compilation errors
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::Geometry::COMPILATION_FAILED\n" <<
                infoLog << std::endl;
        }
    }
    //Start creating and linking shaders into a program
    m_Id = glCreateProgram();

    if(vertexShader != 0)   glAttachShader(m_Id, vertexShader);
    if(fragmentShader != 0) glAttachShader(m_Id, fragmentShader);
    if (geometryShader != 0) glAttachShader(m_Id, geometryShader);
    glLinkProgram(m_Id);


    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_Id, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER COMPILATION FAILED\n" <<
            infoLog << std::endl;

    }
    //Delete shaders as they are already linked and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);

    return 0;
#pragma endregion
  
}


