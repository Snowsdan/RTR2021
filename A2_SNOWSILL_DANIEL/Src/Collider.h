#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Collider {
public:
	Collider();

	glm::vec3 center;
	

};

class BoundingBox : public Collider {
public:
	BoundingBox();

	float minX, maxX;
	float minZ, maxZ;
};

class BoundingSphere : public Collider{
public:
	BoundingSphere();

	float radius;
};