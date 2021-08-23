#pragma once
#include "RTRShader.h"

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Each cube will consist of 12 triangles (6 cube faces have 2 triangles each)
#define NUM_CUBE_FACES 12
#define NUM_CUBE_VERTICES 72

struct Cube {

	float size, radius;
	unsigned long vertexArraySize, faceArraySize;
    float vertices[216];
	unsigned int faces[36];

	glm::vec3 position;
	std::list<Cube> childCubes;

    Cube(float newSize, glm::vec3 posVec) {
        size = newSize;
        position = posVec;
		float radius = size / 2;
		float newVertices[] = {
			//Front Face
			//     points              colours				normals
			 -radius, radius, radius,    1.0f, 0.0f, 0.0f,  0.0, 0.0, 1.0,//Top Left - 0
			  radius, radius, radius,    1.0f,0.0f, 0.0f,	0.0, 0.0, 1.0,//Top Right - 1
			 radius, -radius, radius,    1.0f, 0.0f, 0.0f,	0.0, 0.0, 1.0,//Bottom Right - 2
			-radius, -radius, radius,    1.0f, 0.0f, 0.0f,	0.0, 0.0, 1.0,//Bottom Left - 3

			//Left Face
			//     points              colours
			-radius, radius, -radius,    0.0f, 1.0f, 0.0f,	-1.0, 0.0, 0.0,//Top Left - 4
			 -radius, radius, radius,    0.0f, 1.0f, 0.0f,	-1.0, 0.0, 0.0,//Top Right - 5
			-radius, -radius, radius,    0.0f, 1.0f, 0.0f,	-1.0, 0.0, 0.0,//Bottom Right - 6
		   -radius, -radius, -radius,    0.0f, 1.0f, 0.0f,	-1.0, 0.0, 0.0,//Bottom Left - 7

			//Back Face
			//     points              colours
			-radius, radius, -radius,    1.0f, 0.0f, 0.0f,	0.0, 0.0, -1.0,//Top Left - 8
			 radius, radius, -radius,    1.0f, 0.0f, 0.0f,	0.0, 0.0, -1.0,//Top Right - 9
			radius, -radius, -radius,    1.0f, 0.0f, 0.0f,	0.0, 0.0, -1.0,//Bottom Right - 10
		   -radius, -radius, -radius,    1.0f, 0.0f, 0.0f,	0.0, 0.0, -1.0,//Bottom Left - 11

			//Right Face
			//     points              colours
			  radius, radius, radius,    0.0f, 1.0f, 0.0f,  1.0, 0.0, 0.0,//Top Left - 12
			 radius, radius, -radius,    0.0f, 1.0f, 0.0f,  1.0, 0.0, 0.0,//Top Right - 13
			 radius, -radius,-radius,    0.0f, 1.0f, 0.0f,	1.0, 0.0, 0.0,//Bottom Right - 14
			 radius, -radius, radius,    0.0f, 1.0f, 0.0f,	1.0, 0.0, 0.0,//Bottom Left - 15

			//Top Face
			//     points              colours
			-radius, radius, -radius,    0.0f, 0.0f, 1.0f,	0.0, 1.0, 0.0,//Top Left - 16
			 radius, radius, -radius,    0.0f, 0.0f, 1.0f,	0.0, 1.0, 0.0,//Top Right - 17
			  radius, radius, radius,    0.0f, 0.0f, 1.0f,	0.0, 1.0, 0.0,//Bottom Right - 18
			 -radius, radius, radius,    0.0f, 0.0f, 1.0f,	0.0, 1.0, 0.0,//Bottom Left - 19

			//Bottom Face
			//     points              colours
			radius, -radius, -radius,    0.0f, 0.0f, 1.0f,	0.0, -1.0, 0.0,//Top Left - 20
		   -radius, -radius, -radius,    0.0f, 0.0f, 1.0f,	0.0, -1.0, 0.0,//Top Right - 21
			-radius, -radius, radius,    0.0f, 0.0f, 1.0f,	0.0, -1.0, 0.0,//Bottom Right - 22
			 radius, -radius, radius,    0.0f, 0.0f, 1.0f,	0.0, -1.0, 0.0,//Bottom Left - 23
		};		

		unsigned int newFaces[] = {
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
		vertexArraySize = sizeof(newVertices);
		faceArraySize = sizeof(newFaces);

		
		memcpy(faces, newFaces, sizeof(faces));
        memcpy(vertices, newVertices, sizeof(vertices));
		//std::cout << "CUBE SIZE: " << vertices[0] << std::endl;

     
    }

    

	

};

class SceneBase {
	public:
        virtual void InitialiseScene() = 0;
		std::list<Cube*>* GenCubes(Cube* currentCube);
		void IncreaseSponge();
        void DecreaseSponge();
        void GenerateNewSponge();
        const char* GetSceneName();
        int GetSpongeLevel();
        int GetNumVertices();
        int GetNumFaces();
        virtual void DrawSponge() = 0;
        virtual void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) = 0;
        virtual void DrawCube(Cube* cube) = 0;
        
        

        std::list<Cube*>* spongeList = new std::list<Cube*>;

    protected:
        const char* sceneName;
        int spongeLevel = 1;
        int numVertices = 0;
        int numFaces = 0;
};
