#include "Collider.h"
#include <iostream>

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

bool BoundingBox::checkCollision(Collider* collider) {

	return collider->checkCollision(this);

}

bool BoundingBox::checkCollision(BoundingBox* collisionBox)
{
	return resolveCollision(this, collisionBox);
}

bool BoundingBox::checkCollision(BoundingSphere* collisionSphere)
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
#pragma endregion

#pragma region Bounding Sphere

BoundingSphere::BoundingSphere(float radius, glm::vec3 center) {

	this->radius = radius;
	this->center = center;
}

bool BoundingSphere::checkCollision(Collider* collider) {

	return collider->checkCollision(this);

}

bool BoundingSphere::checkCollision(BoundingBox* collisionBox)
{
	return resolveCollision(collisionBox, this);
}

bool BoundingSphere::checkCollision(BoundingSphere* collisionSphere)
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
#pragma endregion


#pragma region Collision Resolution
bool Collider::resolveCollision(BoundingBox* box, BoundingSphere* sphere)
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
	glm::vec2 closest = sphereCenter + clamped;

	// vector between center circle and closest point from the box collider
	difference = closest - glm::vec2(sphere->center.x, sphere->center.z);

	return glm::length(difference) < sphere->radius;
	
}

bool Collider::resolveCollision(BoundingBox* first, BoundingBox* second)
{
	return (first->maxX >= second->minX && first->minX <= second->maxX)
		&& (first->maxZ >= second->minZ && first->minZ <= second->maxZ);
}

bool Collider::resolveCollision(BoundingSphere* first, BoundingSphere* second)
{
	float distance = sqrt((first->center.x - second->center.x) * (first->center.x - second->center.x) +
		(first->center.z - second->center.z) * (first->center.z - second->center.z));

	return distance < (first->radius + second->radius);
}

#pragma endregion 

