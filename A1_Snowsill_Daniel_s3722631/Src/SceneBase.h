#pragma once
#include "RTRShader.h"

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Cube {

    Cube(float size, glm::vec3 posVec) {
        this->size = size;
        position = posVec;
    }

    float size;
    //glm::mat4 positionMat;
    glm::vec3 position;
    std::list<Cube> childCubes;
};

class SceneBase {
	public:
        virtual void InitialiseScene() = 0;
		std::list<Cube*>* GenCubes(Cube* currentCube);
		void IncreaseSponge();
        void DecreaseSponge();
        void GenerateNewSponge();
        virtual void DrawSponge() = 0;
        virtual void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix) = 0;
        virtual void DrawCube(float size) = 0;
        
        

        std::list<Cube*>* spongeList = new std::list<Cube*>;
        int spongeLevel = 1;
        //RTRShader* currentShader = new RTRShader("src/Shaders/VertexShader.vert", "src/Shaders/FragmentShader.frag");
};
