#include "SceneBase.h"

std::list<Cube*>* SceneBase::GenCubes(Cube* currentCube, SpongeAtt* attributes) {
	std::list<Cube*>* newCubes = new std::list<Cube*>;
	SpongeAtt* newSponge = new SpongeAtt();
	for (float x = -1; x <= 1; x++) {
		for (float y = -1; y <= 1; y++) {
			for (float z = -1; z <= 1; z++) {
				int sum = abs(x) + abs(y) + abs(z);
				if (sum > 1) {
					float size = currentCube->size;
					float newSize = size / 3;
					float radius = newSize / 2;
					glm::vec3 posVec = glm::vec3(
						currentCube->position.x + x * newSize,
						currentCube->position.y + y * newSize,
						currentCube->position.z + z * newSize);
					Cube* newCube = new Cube(newSize, posVec);

					newCubes->push_front(newCube);
					float newVertices[] = {
						//Front Face
						//     points              											normals
						 -radius + posVec.x, radius + posVec.y, radius + posVec.z,       0.0, 0.0, 1.0,//Top Left - 0
						  radius + posVec.x, radius + posVec.y, radius + posVec.z,       0.0, 0.0, 1.0,//Top Right - 1
						 radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	0.0, 0.0, 1.0,//Bottom Right - 2
						-radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	0.0, 0.0, 1.0,//Bottom Left - 3

						//Left Face
						//     points              
						-radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	-1.0, 0.0, 0.0,//Top Left - 4
						 -radius + posVec.x, radius + posVec.y, radius + posVec.z,    	-1.0, 0.0, 0.0,//Top Right - 5
						-radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	-1.0, 0.0, 0.0,//Bottom Right - 6
					   -radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	-1.0, 0.0, 0.0,//Bottom Left - 7

						//Back Face
						//     points              
						radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Top Left - 8
						 -radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Top Right - 9
						-radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Bottom Right - 10
					   radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Bottom Left - 11

						//Right Face
						//     points             
						  radius + posVec.x, radius + posVec.y, radius + posVec.z,       1.0, 0.0, 0.0,//Top Left - 12
						 radius + posVec.x, radius + posVec.y, -radius + posVec.z,       1.0, 0.0, 0.0,//Top Right - 13
						 radius + posVec.x, -radius + posVec.y,-radius + posVec.z,    	1.0, 0.0, 0.0,//Bottom Right - 14
						 radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	1.0, 0.0, 0.0,//Bottom Left - 15

						//Top Face
						//     points             
						-radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 1.0, 0.0,//Top Left - 16
						 radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 1.0, 0.0,//Top Right - 17
						  radius + posVec.x, radius + posVec.y, radius + posVec.z,    	0.0, 1.0, 0.0,//Bottom Right - 18
						 -radius + posVec.x, radius + posVec.y, radius + posVec.z,    	0.0, 1.0, 0.0,//Bottom Left - 19

						//Bottom Face
						//     points              
						radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, -1.0, 0.0,//Top Left - 20
					   -radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, -1.0, 0.0,//Top Right - 21
						-radius + posVec.x, -radius + posVec.y, radius + posVec.z,   	0.0, -1.0, 0.0,//Bottom Right - 22
						 radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	0.0, -1.0, 0.0,//Bottom Left - 23
					};


					//Store new vertices
					for (int i = 0; i < sizeof(newVertices) / sizeof(newVertices[0]); i++) {
						attributes->vertices.push_back(newVertices[i]);
					}


					int startingIndex = attributes->faces.size();
					unsigned int faceIndex;

					if(attributes->faces.empty())
						faceIndex = 0;
					else
						faceIndex = attributes->faces.back() - 2;


					//36 elements in total, but the max value should be the starting value plus 23 if not 0
					for (int i = 0; i <= 6; i++) {
						attributes->faces.push_back(faceIndex);
						attributes->faces.push_back(faceIndex + 2);
						attributes->faces.push_back(faceIndex + 1);
						attributes->faces.push_back(faceIndex);
						attributes->faces.push_back(faceIndex + 3);
						attributes->faces.push_back(faceIndex + 2);
						faceIndex += 4;
					}

				}
			}
		}
	}

	//Update the counters for vertices and faces
	//Divide by two as half the array is taken up by normal info
	numVertices = spongeAttributes->vertices.size() / 2;

	//Divide by three as a face is made up of 3 elements
	numFaces = spongeAttributes->faces.size() / 3;
	return newCubes;
}

//Increase the level of subdivision in the sponge
void SceneBase::IncreaseSponge() {
	
	std::list<Cube*>* newCubes = new std::list<Cube*>;
	spongeAttributes->vertices.clear();
	spongeAttributes->faces.clear();
	for (Cube* currentCube : *spongeAttributes->cubeList) {
		newCubes->splice(newCubes->end(), *GenCubes(currentCube, spongeAttributes));
	}
	spongeLevel++;
	
	spongeAttributes->cubeList = newCubes;
}

void SceneBase::DecreaseSponge() {
	if (spongeLevel > 0) {
		spongeAttributes->cubeList->clear();
		numFaces = 0;
		numVertices = 0;
		GenerateNewSponge(1.0f);

		int newSpongeLevel = spongeLevel - 1;
		spongeLevel = 0;
		
		
		//std::cout << "New Sponge LEVEL: " << newSpongeLevel << std::endl;

		for (int i = 0; i < newSpongeLevel; i++) {
			std::cout << "ADD LEVEL" << std::endl;

			IncreaseSponge();
		}
	}

}

void SceneBase::GenerateNewSponge(float startingSize) {
	glm::vec3 posVec = glm::vec3(0.0);
	spongeAttributes->cubeList->push_back(new Cube(startingSize, posVec));
	
	float radius = startingSize / 2.0f;

	float newVertices[] = {
		//Front Face
		//     points              											normals
		 -radius + posVec.x, radius + posVec.y, radius + posVec.z,       0.0, 0.0, 1.0,//Top Left - 0
		  radius + posVec.x, radius + posVec.y, radius + posVec.z,       0.0, 0.0, 1.0,//Top Right - 1
		 radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	0.0, 0.0, 1.0,//Bottom Right - 2
		-radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	0.0, 0.0, 1.0,//Bottom Left - 3

		//Left Face
		//     points              
		-radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	-1.0, 0.0, 0.0,//Top Left - 4
		 -radius + posVec.x, radius + posVec.y, radius + posVec.z,    	-1.0, 0.0, 0.0,//Top Right - 5
		-radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	-1.0, 0.0, 0.0,//Bottom Right - 6
	   -radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	-1.0, 0.0, 0.0,//Bottom Left - 7

		//Back Face
		//     points              
		radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Top Left - 8
		 -radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Top Right - 9
		-radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Bottom Right - 10
	   radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, 0.0, -1.0,//Bottom Left - 11

		//Right Face
		//     points             
		  radius + posVec.x, radius + posVec.y, radius + posVec.z,       1.0, 0.0, 0.0,//Top Left - 12
		 radius + posVec.x, radius + posVec.y, -radius + posVec.z,       1.0, 0.0, 0.0,//Top Right - 13
		 radius + posVec.x, -radius + posVec.y,-radius + posVec.z,    	1.0, 0.0, 0.0,//Bottom Right - 14
		 radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	1.0, 0.0, 0.0,//Bottom Left - 15

		//Top Face
		//     points             
		-radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 1.0, 0.0,//Top Left - 16
		 radius + posVec.x, radius + posVec.y, -radius + posVec.z,    	0.0, 1.0, 0.0,//Top Right - 17
		  radius + posVec.x, radius + posVec.y, radius + posVec.z,    	0.0, 1.0, 0.0,//Bottom Right - 18
		 -radius + posVec.x, radius + posVec.y, radius + posVec.z,    	0.0, 1.0, 0.0,//Bottom Left - 19

		//Bottom Face
		//     points              
		radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, -1.0, 0.0,//Top Left - 20
	   -radius + posVec.x, -radius + posVec.y, -radius + posVec.z,    	0.0, -1.0, 0.0,//Top Right - 21
		-radius + posVec.x, -radius + posVec.y, radius + posVec.z,   	0.0, -1.0, 0.0,//Bottom Right - 22
		 radius + posVec.x, -radius + posVec.y, radius + posVec.z,    	0.0, -1.0, 0.0,//Bottom Left - 23
	};


	//Store new vertices
	for (int i = 0; i < sizeof(newVertices) / sizeof(newVertices[0]); i++) {
		spongeAttributes->vertices.push_back(newVertices[i]);
	}


	int startingIndex = spongeAttributes->faces.size();
	unsigned int faceIndex;

	if (spongeAttributes->faces.empty())
		faceIndex = 0;
	else
		faceIndex = spongeAttributes->faces.back() - 2;

	for (int i = 0; i < 6; i++) {

		spongeAttributes->faces.push_back(faceIndex);
		spongeAttributes->faces.push_back(faceIndex + 2);
		spongeAttributes->faces.push_back(faceIndex + 1);
		spongeAttributes->faces.push_back(faceIndex);
		spongeAttributes->faces.push_back(faceIndex + 3);
		spongeAttributes->faces.push_back(faceIndex + 2);
		faceIndex += 4;
	}

	std::cout << "FACE SIZE: " << spongeAttributes->faces.size() << std::endl;
	numFaces += NUM_CUBE_FACES;
	numVertices += NUM_CUBE_VERTICES;
	
}

void SceneBase::IncreaseLights() {
	if (lightList->size() < 10) {
		//if the list is empty, add a directional light
		if (lightList->empty()) {
			lightList->push_back(new Light(
				directional,
				glm::vec3(0.0),
				glm::vec3(-0.1f, 1.5f, 1.5f),
				glm::vec3(0.2f, 0.2f, 0.2f),
				glm::vec3(1.0, 1.0, 1.0),
				glm::vec3(0.3, 0.3, 0.3),
				0, 0, 0));
		}
		else {
			//if there is already a directional light, add point light
			srand(time(NULL));

			float randomX = (float)(rand() % 6 - 3);
			float randomY = (float)(rand() % 6 - 3);
			float randomZ = (float)(rand() % 6 - 3);
			Light* newLight = new Light(
				point,
				glm::vec3(randomX, randomY, randomZ),
				glm::vec3(0.0),
				glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX),
				glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX),
				glm::vec3(1.0, 1.0, 1.0),
				1.0, 0.7, 0.032);
			std::cout << "X Coord: " << (float)rand() / RAND_MAX << std::endl;
			lightList->push_back(newLight);
		}
		numActiveLights++;
	}
	std::cout << "LIGHT LIST SIZE: " << lightList->size() << std::endl;
}

void SceneBase::DrawSponge(SpongeAtt* attributes) {
	//Create buffer for the vertexPoints
	unsigned int vertexBuffer = 0;
	unsigned int vertexArrayObject = 0;
	unsigned int colourBuffer = 0;
	unsigned int faceElementBuffer = 0;

	//Send vertex point data to buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, attributes->vertices.size() * sizeof(float), &attributes->vertices[0], GL_STATIC_DRAW);

	//Generate vertex array object
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	//Specify attribute locations
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	//Specify what data is for vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Specify what data is for normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Declare Element Buffer Object that allows the GPU to read what vertices to use when drawing
	glGenBuffers(1, &faceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, faceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, attributes->faces.size() * sizeof(unsigned int), &attributes->faces[0], GL_STATIC_DRAW);

	//Draw the shape
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, attributes->faces.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	//Clean up
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &faceElementBuffer);
	vertexArrayObject = 0;
	vertexBuffer = 0;
	faceElementBuffer = 0;
}



void SceneBase::DecreaseLights() {
	if (numActiveLights > 0)
		numActiveLights--;
}


const char* SceneBase::GetSceneName() {
	return sceneName;
}

int SceneBase::GetSpongeLevel() {
	return spongeLevel;
}

int SceneBase::GetNumVertices() {
	return numVertices;
}

int SceneBase::GetNumFaces() {
	return numFaces;
}