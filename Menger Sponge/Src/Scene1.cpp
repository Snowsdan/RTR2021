#include "Scene1.h"

void Scene1::InitialiseScene() {
	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GenerateNewSponge(1.0);
	
}

void Scene1::DeactivateScene() {
	while (!spongeAttributes->cubeList->empty()) {
		delete spongeAttributes->cubeList->front();
		spongeAttributes->cubeList->pop_front();
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_NORMALIZE);

	spongeAttributes->cubeList->clear();
	numActiveLights = 0;
	spongeLevel = 1;
	numVertices = 0;
	numFaces = 0;
}

void  Scene1::RenderScene(glm::mat4 cameraMatrix, glm::mat4 projectionMatrix,
	glm::vec3 cameraPos, glm::vec3 cameraDirection) {


	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&projectionMatrix[0][0]);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glPushMatrix();
	glLoadMatrixf(&cameraMatrix[0][0]);
	float len = 2.0;

	glm::mat4 view;
	view = glm::lookAt(glm::vec3(-0.05, 0.5, -0.05), glm::vec3(0.5, 0.3, 0.0), glm::vec3(0.0,1.0,0.0));

	if (isLighting) {
		
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		//Disable all lights before deciding which ones to activate
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT3);
		glDisable(GL_LIGHT4);
		glDisable(GL_LIGHT5);
		glDisable(GL_LIGHT6);
		glDisable(GL_LIGHT7);

		//Enable lights depending on how many active lights
		switch (numActiveLights) {
		case 8:
			glEnable(GL_LIGHT7);
		case 7:
			glEnable(GL_LIGHT6);
		case 6:
			glEnable(GL_LIGHT5);
		case 5:
			glEnable(GL_LIGHT4);
		case 4:
			glEnable(GL_LIGHT3);
		case 3:
			glEnable(GL_LIGHT2);
		case 2:
			glEnable(GL_LIGHT1);
		case 1:
			glEnable(GL_LIGHT0);
		}
		//update directional light 
		float cameraDir[3] = { cameraDirection.x, cameraDirection.y, cameraDirection.z };

		//Update directional light direction
		glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, cameraDir);
	}
	else {
		glDisable(GL_LIGHTING);
	}

	DrawVertices();
	
	//std::cout << "END DRAW" << std::endl;
	glPopMatrix();

}

void Scene1::DrawVertices() {
	int faceLimit = 0;

	if (spongeLevel == 0)
		faceLimit = spongeAttributes->faces.size();
	else 
		faceLimit = spongeAttributes->faces.size() - 6;

	glBegin(GL_TRIANGLES);
	for (int faceIndex = 0; faceIndex < faceLimit; faceIndex += 6) {
		//The normal vec3 correspond to the rgb value 
		float r = spongeAttributes->vertices[spongeAttributes->faces[faceIndex] * 6 + 3];
		float g = spongeAttributes->vertices[spongeAttributes->faces[faceIndex] * 6 + 4];
		float b = spongeAttributes->vertices[spongeAttributes->faces[faceIndex] * 6 + 5];

		float currentMaterial[] = { abs(r), abs(g), abs(b), 1.0 };

		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, currentMaterial);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5f);
		//The faces vector stores the order of attributes (3 floats for position, 3 for normal)
		//Multiplying the index by 6 gives us the starting attribute for each square face
		glColor3f(abs(r), abs(g), abs(b));
		glVertex3f(spongeAttributes->vertices[spongeAttributes->faces[faceIndex] * 6],//0
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex] * 6) + 1],//
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex] * 6) + 2]); //

		glVertex3f(spongeAttributes->vertices[spongeAttributes->faces[faceIndex + 1] * 6], //2
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 1] * 6) + 1],	//
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 1] * 6) + 2]); //

		glVertex3f(spongeAttributes->vertices[spongeAttributes->faces[faceIndex + 2] * 6], //1
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 2] * 6) + 1],
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 2] * 6) + 2]);

		glVertex3f(spongeAttributes->vertices[spongeAttributes->faces[faceIndex + 3] * 6],//0
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 3] * 6) + 1],//
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 3] * 6) + 2]); //

		glVertex3f(spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 4] * 6)], // 3
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 4] * 6) + 1],	//
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 4] * 6) + 2]); //

		glVertex3f(spongeAttributes->vertices[spongeAttributes->faces[faceIndex + 1] * 6], //2
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 5] * 6) + 1],	//
			spongeAttributes->vertices[(spongeAttributes->faces[faceIndex + 5] * 6) + 2]); //

	}

	glEnd();
	
}

void Scene1::IncreaseLights() {
	if (lightList->size() < 8) {
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

			float lightAmbient[] = { 0.1f, 0.1f, 0.1f,1.0 };
			float lightDiffuse[] = { 1.0,1.0,1.0,1.0 };
			float lightSpecular[] = { 0.2,0.2,0.2,1 };

			
			glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

			//glLightfv(GL_LIGHT0, GL_POSITION, 0);
		}
		else {
			std::cout << "POINT" << std::endl;
			//if there is already a directional light, add point light
			srand(time(NULL));
			GLenum newLightName;
			switch (lightList->size()) {
				case 1:
					newLightName = GL_LIGHT1;
					break;
				case 2:
					newLightName = GL_LIGHT2;
					break;
				case 3:
					newLightName = GL_LIGHT3;
					break;
				case 4:
					newLightName = GL_LIGHT4;
					break;
				case 5:
					newLightName = GL_LIGHT5;
					break;
				case 6:
					newLightName = GL_LIGHT6;
					break;
				case 7:
					newLightName = GL_LIGHT7;
					break;
			}

			//Set random position
			float randomX = (float)(rand() % 6 - 3);
			float randomY = (float)(rand() % 6 - 3);
			float randomZ = (float)(rand() % 6 - 3);
			float lightPosition[] = {
				randomX,
				randomY,
				randomZ
			};
			//Set random light colours
			float lightAmbient[] = { 
				(float)rand() / RAND_MAX, 
				(float)rand() / RAND_MAX, 
				(float)rand() / RAND_MAX,
				1.0 };

			float lightDiffuse[] = {
				(float)rand() / RAND_MAX, 
				(float)rand() / RAND_MAX, 
				(float)rand() / RAND_MAX,
				1.0 };

			float lightSpecular[] = { 0.2,0.2,0.2,1 };

			Light* newLight = new Light(
				point,
				glm::vec3(lightPosition[0] , lightPosition[1], lightPosition[2]),
				glm::vec3(0.0),
				glm::vec3(lightAmbient[0], lightAmbient[1], lightAmbient[2]),
				glm::vec3(lightDiffuse[0], lightDiffuse[1],lightDiffuse[2]),
				glm::vec3(1.0, 1.0, 1.0),
				0.0, 0.0, 0.0);

			lightList->push_back(newLight);

			//Set light parameters
			glLightfv(newLightName, GL_AMBIENT, lightAmbient);
			glLightfv(newLightName, GL_DIFFUSE, lightDiffuse);
			glLightfv(newLightName, GL_SPECULAR, lightSpecular);

			glLightfv(newLightName, GL_POSITION, lightPosition);
		}
		numActiveLights++;
	}
	std::cout << "LIGHT LIST SIZE: " << lightList->size() << std::endl;
}



