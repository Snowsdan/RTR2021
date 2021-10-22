#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Collider;
class BoundingBox; 
class BoundingSphere;

class Collider {
public:

	glm::vec3 center;
	
	virtual bool checkCollision(Collider* collider) = 0;
	virtual bool checkCollision(BoundingBox* collisionBox) = 0;
	virtual bool checkCollision(BoundingSphere* collisionSphere) = 0;

	virtual void Translate(glm::vec3 translation) = 0;
	virtual void Scale(glm::vec3 scale) = 0;


	bool resolveCollision(BoundingBox* box, BoundingSphere* sphere);
	bool resolveCollision(BoundingBox* boxA, BoundingBox* boxB);
	bool resolveCollision(BoundingSphere* sphereA, BoundingSphere* sphereB);

};

class BoundingBox : public Collider {
public:
	BoundingBox(float minX, float maxX, float minZ, float maxZ, glm::vec3 center);

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;

	float sizeX;
	float sizeZ;

	bool checkCollision(Collider* collider);
	bool checkCollision(BoundingBox* collisionBox);
	bool checkCollision(BoundingSphere* collisionSphere);
	void Translate(glm::vec3 translation);
	void Scale(glm::vec3 scale);

};

class BoundingSphere : public Collider{
public:
	BoundingSphere(float radius, glm::vec3 center);

	float radius;

	bool checkCollision(Collider* collider);
	bool checkCollision(BoundingBox* collisionBox);
	bool checkCollision(BoundingSphere* collisionSphere);
	void Translate(glm::vec3 translation);
	void Scale(glm::vec3 scale);
};





