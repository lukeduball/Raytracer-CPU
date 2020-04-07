#include "AABB.h"

#include "../Renderer/Ray.h"

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
