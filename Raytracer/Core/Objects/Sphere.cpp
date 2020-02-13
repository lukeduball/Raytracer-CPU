#include "Sphere.h"

#include <glm/geometric.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(glm::vec3 pos, float rad, glm::vec3 color) : Object(pos), radius(rad), radiusSquared(rad * rad)
{
	this->albedo = color;
}

glm::vec3 Sphere::getNormalData(glm::vec3 & intersectionPoint)
{
	return glm::normalize(intersectionPoint - this->position);
}

glm::vec2 Sphere::getTextureCoordData(glm::vec3 & intersectionPoint, glm::vec3 & normal)
{
	glm::vec2 texCoords;
	texCoords.x = (1 + atan2(normal.z, normal.x) / (float)M_PI) * 0.5f;
	texCoords.y = acosf(normal.y) / (float)M_PI;
	return texCoords;
}

bool Sphere::intersect(const Ray & ray, float & parameter)
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
}
