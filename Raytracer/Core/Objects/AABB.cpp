#include "AABB.h"

#include "../Renderer/Ray.h"

#include <algorithm>

AABB::AABB(float miX, float miY, float miZ, float maX, float maY, float maZ) : minX(miX), minY(miY), minZ(miZ), maxX(maX), maxY(maY), maxZ(maZ)
{
}

bool AABB::intersect(const Ray & ray)
{
	float tmin = (minX - ray.getOrigin().x) / ray.getDirectionVector().x;
	float tmax = (maxX - ray.getOrigin().x) / ray.getDirectionVector().x;

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	float tymin = (minY - ray.getOrigin().y) / ray.getDirectionVector().y;
	float tymax = (maxY - ray.getOrigin().y) / ray.getDirectionVector().y;

	if (tymin > tymax)
	{
		std::swap(tymin, tymax);
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	float tzmin = (minZ - ray.getOrigin().z) / ray.getDirectionVector().z;
	float tzmax = (maxZ - ray.getOrigin().z) / ray.getDirectionVector().z;

	if (tzmin > tzmax)
	{
		std::swap(tzmin, tzmax);
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}
		
	return true;
}

glm::vec3 AABB::getMinAsPoint()
{
	return glm::vec3(minX, minY, minZ);
}

glm::vec3 AABB::getMaxAsPoint()
{
	return glm::vec3(maxX, maxY, maxZ);
}

AABB * AABB::calculateBoundingBox(const std::vector<glm::vec3> & pointList)
{
	glm::vec3 firstVert = pointList[0];
	float minX = firstVert.x, maxX = firstVert.x;
	float minY = firstVert.y, maxY = firstVert.y;
	float minZ = firstVert.z, maxZ = firstVert.z;

	for (uint32_t i = 1; i < pointList.size(); i++)
	{
		glm::vec3 vert = pointList[i];
		minX = std::min(minX, vert.x);
		maxX = std::max(maxX, vert.x);

		minY = std::min(minY, vert.y);
		maxY = std::max(maxY, vert.y);

		minZ = std::min(minZ, vert.z);
		maxZ = std::max(maxZ, vert.z);
	}

	return new AABB(minX, minY, minZ, maxX, maxY, maxZ);
}
