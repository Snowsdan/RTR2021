#include "Collider.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>


#pragma region Bounding Box
BoundingBox::BoundingBox(float minX, float maxX, float minZ, float maxZ, glm::vec3 center)
{
	this->minX = minX;
	this->maxX = maxX;
	this->minZ = minZ;
	this->maxZ = maxZ;

	this->center = center;

	sizeX = maxX - minX;
	sizeZ = maxZ - minZ;
}

CollisionInfo* BoundingBox::checkCollision(Collider* collider) {

	return collider->checkCollision(this);

}

CollisionInfo* BoundingBox::checkCollision(BoundingBox* collisionBox)
{
	return resolveCollision(this, collisionBox);
}

CollisionInfo* BoundingBox::checkCollision(BoundingSphere* collisionSphere)
{
	return resolveCollision(this, collisionSphere);
}

void BoundingBox::Translate(glm::vec3 translation) {
	minX += translation.x;
    maxX += translation.x;
    minY += translation.y;
    maxY += translation.y;
    minZ += translation.z;
    maxZ += translation.z;

	center.x += translation.x;
	center.y += translation.y;
	center.z += translation.z;
}

void BoundingBox::Scale(glm::vec3 scale) {
	minX *= scale.x;
	maxX *= scale.x;
	minY *= scale.y;
	maxY *= scale.y;
	minZ *= scale.z;
	maxZ *= scale.z;

	sizeX = maxX - minX;
	sizeZ = maxZ - minZ;
}

void BoundingBox::DrawVolume() {
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 0.0);

	glm::vec3 miniX = glm::vec3(center.x + minX, center.y, center.z + minZ);

	glVertex3f(center.x + sizeX / 2.0, center.y + 0.251, center.z + sizeZ / 2.0);
	glVertex3f(center.x - sizeX / 2.0, center.y + 0.251, center.z + sizeZ / 2.0);
	glVertex3f(center.x - sizeX / 2.0, center.y + 0.251, center.z - sizeZ / 2.0);
	glVertex3f(center.x + sizeX / 2.0, center.y + 0.251, center.z - sizeZ / 2.0);
	glVertex3f(center.x + sizeX / 2.0, center.y + 0.251, center.z + sizeZ / 2.0);
	glEnd();
}
#pragma endregion

#pragma region Bounding Sphere

BoundingSphere::BoundingSphere(float radius, glm::vec3 center) {

	this->radius = radius;
	this->center = center;
}

CollisionInfo* BoundingSphere::checkCollision(Collider* collider) {

	return collider->checkCollision(this);

}

CollisionInfo* BoundingSphere::checkCollision(BoundingBox* collisionBox)
{
	return resolveCollision(collisionBox, this);
}

CollisionInfo* BoundingSphere::checkCollision(BoundingSphere* collisionSphere)
{
	return resolveCollision(this, collisionSphere);
}

void BoundingSphere::Translate(glm::vec3 translation) {
	center.x += translation.x;
	center.y += translation.y;
	center.z += translation.z;
}

void BoundingSphere::Scale(glm::vec3 Scale) {

}

void BoundingSphere::DrawVolume() {
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 0.0);
	int numParts = 20;

	for (int i = 0; i < numParts; i++) {
		float angle = 2.0f * M_PI * (float)i / float(numParts);
			
		//Add 0.001 to avoid depth fighting
		float x = (radius + 0.001) * cosf(angle);   

		float z = (radius + 0.001) * sinf(angle);             
		glVertex3f(center.x + x, center.y , center.z + z);
	}
	
	glEnd();
}
#pragma endregion


#pragma region Collision Resolution
CollisionInfo* Collider::resolveCollision(BoundingBox* box, BoundingSphere* sphere)
{
	//std::cout << "BOX SPHERE COLLISION" << std::endl;
	glm::vec2 sphereCenter = glm::vec2(box->center.x, box->center.z);

	glm::vec2 halfExtents = glm::vec2(box->sizeX / 2.0f, box->sizeZ / 2.0f);

	// get difference vector between both centers
	float centerDifferenceX = sphere->center.x - box->center.x;
	float centerDifferenceZ = sphere->center.z - box->center.z;
	glm::vec2 difference = glm::vec2(centerDifferenceX, centerDifferenceZ);

	glm::vec2 clamped = glm::clamp(difference, -halfExtents, halfExtents);

	// add clamped value to box and get the value closest point of box to the circle
	glm::vec2 closestPoint = sphereCenter + clamped;

	// vector between center circle and closest point from the box collider
	difference = closestPoint - glm::vec2(sphere->center.x, sphere->center.z);

	//return collision info if there is a collision
	if (glm::length(difference) < sphere->radius) {
		return new CollisionInfo(FindNewDirection(difference), difference);
	}
	else {
		return nullptr;
	}


	
}

CollisionInfo* Collider::resolveCollision(BoundingBox* first, BoundingBox* second)
{
	if ((first->maxX >= second->minX && first->minX <= second->maxX)
		&& (first->maxZ >= second->minZ && first->minZ <= second->maxZ)) {
		//TODO: Find actual collision data
		return new CollisionInfo(UP, glm::vec2(1.0));
	} 
	return nullptr;
}

CollisionInfo* Collider::resolveCollision(BoundingSphere* first, BoundingSphere* second)
{
	float distance = sqrt((first->center.x - second->center.x) * (first->center.x - second->center.x) +
		(first->center.z - second->center.z) * (first->center.z - second->center.z));

	if (distance < (first->radius + second->radius)) {
		glm::vec2 centerDiff = glm::vec2(first->center.x - second->center.x, first->center.z - second->center.z);
		return new CollisionInfo(FindNewDirection(centerDiff), centerDiff);
	}
	return nullptr;
}

//Used to find which direction we need to make a ball go once it has collided with something
Direction Collider::FindNewDirection(glm::vec2 point)
{
	glm::vec2 directionVectors[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	float max = 0.0f;

	unsigned int best_match = -1;

	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(point), directionVectors[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}



#pragma endregion 

