#pragma once

#include "RTRShader.h"
#include <vector>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>



class SkyBox {
public: 
	SkyBox();
	void Render(RTRShader* shader);
private:

	void InitMesh();
	void InitTexture();

	int numVertices;
	int numFaces;


	unsigned int m_VertexBuffer = 0;
	unsigned int m_VertexArray = 0;
	unsigned int m_FaceElementBuffer = 0;
	unsigned int textureID = 0;
	int width, height, nrChannels;
	unsigned char* data;

	std::vector<float> vertices;
	std::vector<unsigned int> faces;

	glm::mat4 model = glm::mat4(1.0);

};