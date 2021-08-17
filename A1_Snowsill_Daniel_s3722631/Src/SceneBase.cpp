#include "SceneBase.h"

std::list<Cube*>* SceneBase::GenCubes(Cube* currentCube) {
	std::list<Cube*>* newCubes = new std::list<Cube*>;
	for (float x = -1; x <= 1; x += 1) {
		for (float y = -1; y <= 1; y += 1) {
			for (float z = -1; z <= 1; z += 1) {
				int sum = abs(x) + abs(y) + abs(z);
				if (sum > 1) {
					float size = currentCube->size;
					float newSize = size / 3;

					glm::mat4 modelMat = glm::mat4(1.0f);
					glm::vec3 posVector = glm::vec3(
						currentCube->position.x + x * newSize,
						currentCube->position.y + y * newSize,
						currentCube->position.z + z * newSize);
					modelMat = glm::translate(modelMat, posVector);

					Cube* newCube = new Cube(newSize, posVector);
					//std::cout << newCube->size << std::endl;

					newCubes->push_front(newCube);;
				}
			}
		}
	}
	std::cout << "CUBES SIZE" << newCubes->size() << std::endl;
	return newCubes;
}
void SceneBase::IncreaseSponge() {
	std::list<Cube*>* newCubes = new std::list<Cube*>;

	for (Cube* currentCube : *spongeList) {

		newCubes->splice(newCubes->end(), *GenCubes(currentCube));
	}
	std::cout << "NEW CUBES SIZE: " << newCubes->size() << std::endl;
	spongeList = newCubes;
}