#pragma once
#include "RTRShader.h"
#include "Light.h"

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <list>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Each cube will consist of 12 triangles (6 cube faces have 2 triangles each)
#define NUM_CUBE_FACES 12
#define NUM_CUBE_VERTICES 72
#define MAX_LIGHTS 10

struct Cube {

    float size;


	glm::vec3 position;


    Cube(float newSize, glm::vec3 posVec) {
        size = newSize;
        position = posVec;

    }
};

struct SpongeAtt {
	std::vector<float> vertices;
	std::vector<unsigned int> faces;
    std::list<Cube*>* cubeList = new std::list<Cube*>;
};

class SceneBase {
	public:
        virtual void InitialiseScene() = 0;
		virtual void DeactivateScene() = 0;
		virtual std::list<Cube*>* GenCubes(Cube* currentCube, SpongeAtt* attributes);
		virtual void IncreaseSponge();
        virtual void DecreaseSponge();
        void GenerateNewSponge();
		void IncreaseLights();
		void DecreaseLights();

        const char* GetSceneName();
        int GetSpongeLevel();
        int GetNumVertices();
        int GetNumFaces();
        virtual void RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix, glm::vec3 cameraPos, glm::vec3 cameraDirection) = 0;
        virtual void DrawSponge(SpongeAtt* attributes);
        
        

        //std::list<Cube*>* spongeList = new std::list<Cube*>;
		SpongeAtt* spongeAttributes = new SpongeAtt;

		bool isLighting = true;
		int numActiveLights = 0;

    protected:
		std::list<Light*>* lightList = new std::list<Light*>;

        const char* sceneName;
        int spongeLevel = 1;
        int numVertices = 0;
        int numFaces = 0;
};
