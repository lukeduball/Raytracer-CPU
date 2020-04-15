#include "AABB.h"

#include "../Renderer/Ray.h"

#include <algorithm>

AABB::AABB(float miX, float miY, float miZ, float maX, float maY, float maZ) : AABB(glm::vec3( (miX+maX) / 2.0f, (miY+maY) / 2.0f, (miZ + maZ) / 2.0f), glm::vec3((maX - miX) / 2.0f, (maY - miY) / 2.0f, (maZ - miZ) / 2.0f))
{
}

AABB::AABB(glm::vec3 c, glm::vec3 hDist) : center(c), halfDistances(hDist)
{
}

bool AABB::intersect(const Ray & ray)
{
	glm::vec3 min = getMinAsPoint();
	glm::vec3 max = getMaxAsPoint();

	float tmin = (min.x - ray.getOrigin().x) / ray.getDirectionVector().x;
	float tmax = (max.x - ray.getOrigin().x) / ray.getDirectionVector().x;

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	float tymin = (min.y - ray.getOrigin().y) / ray.getDirectionVector().y;
	float tymax = (max.y - ray.getOrigin().y) / ray.getDirectionVector().y;

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

	float tzmin = (min.z - ray.getOrigin().z) / ray.getDirectionVector().z;
	float tzmax = (max.z - ray.getOrigin().z) / ray.getDirectionVector().z;

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
	return center - halfDistances;
}

glm::vec3 AABB::getMaxAsPoint()
{
	return center + halfDistances;
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
