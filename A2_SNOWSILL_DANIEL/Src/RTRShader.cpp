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
            std::cout << "RTR:ERROR:RTR_Shader:Could not load geometry shader file - " << geometryPath << std::endl;
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
   /* int ret_code = 0;
    char *vert_src = nullptr, *frag_src = nullptr, *geom_src = nullptr;
    unsigned int vert_shader = 0, frag_shader = 0, geom_shader = 0;

    if (vertexPath != nullptr && (vert_src = load_shader_file(vertexPath)) == nullptr) {
        std::cout << "RTR:ERROR:RTR_Shader:Could not load vertex shader file - " << vertexPath << std::endl;
        ret_code = -1;
        goto ERR_RTR_SHADER_LOAD;
    }
    if (fragmentPath != nullptr && (frag_src = load_shader_file(fragmentPath)) == nullptr) {
        std::cout << "RTR:ERROR:RTR_Shader:Could not load fragment shader file - " << fragmentPath << std::endl;
        ret_code = -2;
        goto ERR_RTR_SHADER_LOAD;
    }
    if (geometryPath != nullptr && (geom_src = load_shader_file(geometryPath)) == nullptr) {
        std::cout << "RTR:ERROR:RTR_Shader:Could not load geometry shader file - " << geometryPath << std::endl;
        ret_code = -3;
        goto ERR_RTR_SHADER_LOAD;
    }

    int err_ok;
    char err_info[512];

    if (vert_src != nullptr) {
        vert_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vert_shader, 1, &vert_src, nullptr);
        glCompileShader(vert_shader);
        glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &err_ok);
        if (!err_ok) {
            glGetShaderInfoLog(vert_shader, 512, nullptr, err_info);
            std::cout << "RTR:ERROR: Vertex Shader failed to compile. Error: " << err_info << std::endl;
            ret_code = -4;
            goto ERR_RTR_SHADER_LOAD;
        }
    }
    if (frag_src != nullptr) {
        frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag_shader, 1, &frag_src, nullptr);
        glCompileShader(frag_shader);
        glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &err_ok);
        if (!err_ok) {
            glGetShaderInfoLog(frag_shader, 512, nullptr, err_info);
            std::cout << "RTR:ERROR: Fragment Shader failed to compile. Error: " << err_info << std::endl;
            ret_code = -5;
            goto ERR_RTR_SHADER_LOAD;
        }
    }
    if (geom_src != nullptr) {
        geom_shader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geom_shader, 1, &geom_src, nullptr);
        glCompileShader(geom_shader);
        glGetShaderiv(geom_shader, GL_COMPILE_STATUS, &err_ok);
        if (!err_ok) {
            glGetShaderInfoLog(geom_shader, 512, nullptr, err_info);
            std::cout << "RTR:ERROR: Geometry Shader failed to compile. Error: " << err_info << std::endl;
            ret_code = -6;
            goto ERR_RTR_SHADER_LOAD;
        }
    }

    m_Id = glCreateProgram();
    if (vert_shader != 0) glAttachShader(m_Id, vert_shader);
    if (frag_shader != 0) glAttachShader(m_Id, frag_shader);
    if (geom_shader != 0) glAttachShader(m_Id, geom_shader);
    glLinkProgram(m_Id);
    glGetProgramiv(m_Id, GL_LINK_STATUS, &err_ok);
    if (!err_ok) {
        glGetProgramInfoLog(m_Id, 512, NULL, err_info);
        std::cout << "RTR:ERROR: Shader Program failed to link. Error: " << err_info << std::endl;
        ret_code = -7;
        goto ERR_RTR_SHADER_LOAD;
    }

ERR_RTR_SHADER_LOAD:
    if (vert_shader != 0) glDeleteShader(vert_shader);
    if (frag_shader != 0) glDeleteShader(frag_shader);
    if (geom_shader != 0) glDeleteShader(geom_shader);
    if (vert_src != nullptr) delete vert_src;
    if (frag_src != nullptr) delete frag_src;
    if (geom_src != nullptr) delete geom_src;
    return ret_code;*/
}


