#pragma once

#include <glm/vec3.hpp>

class Ray;

class AABB
{
public:
	AABB(float miX, float miY, float miZ, float maX, float maY, float maZ);

	bool intersect(const Ray & ray);

	glm::vec3 getMinAsPoint();
	glm::vec3 getMaxAsPoint();

private:
	float minX, minY, minZ, maxX, maxY, maxZ;
};