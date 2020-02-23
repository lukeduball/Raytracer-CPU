#include "Sphere.h"

#include <glm/geometric.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(glm::vec3 pos, float rad, glm::vec3 color) : Object(pos), radius(rad), radiusSquared(rad * rad)
{
	this->albedo = color;
}

void Sphere::getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords)
{
	normal = glm::normalize(intersectionPoint - this->position);

	textureCoords.x = (1 + atan2(normal.z, normal.x) / (float)M_PI) * 0.5f;
	textureCoords.y = acosf(normal.y) / (float)M_PI;
}

bool Sphere::intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData)
{
	//Find the distance squared between the ray origin and center
	glm::vec3 rayOriginToSphereCenter = this->position - ray.getOrigin();
	float distanceSquared = glm::dot(rayOriginToSphereCenter, rayOriginToSphereCenter);
	bool outsideSphere = distanceSquared >= this->radiusSquared;

	float closestApproachParameter = glm::dot(rayOriginToSphereCenter, ray.getDirectionVector());
	if (outsideSphere && closestApproachParameter < 0)
	{
		return false;
	}

	float halfChordDistanceSquared = this->radiusSquared - distanceSquared + closestApproachParameter * closestApproachParameter;
	if (outsideSphere && halfChordDistanceSquared < 0)
	{
		return false;
	}

	if (outsideSphere)
	{
		parameter = closestApproachParameter - sqrt(halfChordDistanceSquared);
	}
	else
	{
		parameter = closestApproachParameter + sqrt(halfChordDistanceSquared);
	}

	return true;
}
