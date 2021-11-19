#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

class Collider;
class BoundingBox; 
class BoundingSphere;

enum Direction {
	UP,
	LEFT,
	DOWN,
	RIGHT
};

struct CollisionInfo {
	CollisionInfo(Direction newDirection, glm::vec2 diff) {
		direction = newDirection;
		differenceVec = diff;
	}
	Direction direction;
	glm::vec2 differenceVec;
};

class Collider {
public:

	glm::vec3 center;
	
	virtual CollisionInfo* checkCollision(Collider* collider) = 0;
	virtual CollisionInfo* checkCollision(BoundingBox* collisionBox) = 0;
	virtual CollisionInfo* checkCollision(BoundingSphere* collisionSphere) = 0;

	virtual void Translate(glm::vec3 translation) = 0;
	virtual void Scale(glm::vec3 scale) = 0;


	CollisionInfo* resolveCollision(BoundingBox* box, BoundingSphere* sphere);
	CollisionInfo* resolveCollision(BoundingBox* boxA, BoundingBox* boxB);
	CollisionInfo* resolveCollision(BoundingSphere* sphereA, BoundingSphere* sphereB);
	Direction FindNewDirection(glm::vec2 point);

	virtual void DrawVolume() = 0;

};

class BoundingBox : public Collider {
public:
	BoundingBox(float minX, float maxX, float minZ, float maxZ, glm::vec3 center);

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;

	float sizeX;
	float sizeZ;

	CollisionInfo* checkCollision(Collider* collider);
	CollisionInfo* checkCollision(BoundingBox* collisionBox);
	CollisionInfo* checkCollision(BoundingSphere* collisionSphere);
	void Translate(glm::vec3 translation);
	void Scale(glm::vec3 scale);

	void DrawVolume();

};

class BoundingSphere : public Collider{
public:
	BoundingSphere(float radius, glm::vec3 center);

	float radius;

	CollisionInfo* checkCollision(Collider* collider);
	CollisionInfo* checkCollision(BoundingBox* collisionBox);
	CollisionInfo* checkCollision(BoundingSphere* collisionSphere);
	void Translate(glm::vec3 translation);
	void Scale(glm::vec3 scale);

	void DrawVolume();
};





