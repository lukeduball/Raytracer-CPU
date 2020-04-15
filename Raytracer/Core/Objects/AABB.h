#pragma once

#include <glm/vec3.hpp>

#include <vector>

class Ray;

class AABB
{
public:
	AABB(float miX, float miY, float miZ, float maX, float maY, float maZ);

	bool intersect(const Ray & ray);

	glm::vec3 getMinAsPoint();
	glm::vec3 getMaxAsPoint();

	static AABB * calculateBoundingBox(const std::vector<glm::vec3> & pointsList);

private:
	float minX, minY, minZ, maxX, maxY, maxZ;
};