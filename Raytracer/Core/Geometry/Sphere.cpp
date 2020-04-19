#include "Sphere.h"

#include <glm/geometric.hpp>

#include "../Renderer/Materials/Material.h"

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(glm::vec3 pos, float rad, Material * material) : Object(pos, material), radius(rad), radiusSquared(rad * rad)
{
}

void Sphere::getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material *& material)
{
	//Calculate the normal by subtracting the intersection point from the center of the sphere
	normal = glm::normalize(intersectionPoint - this->position);

	textureCoords.x = (1 + atan2(normal.z, normal.x) / (float)M_PI) * 0.5f;
	textureCoords.y = acosf(normal.y) / (float)M_PI;

	material = this->getMaterial();
}

bool Sphere::possibleIntersection(const Ray & ray, float & parameter)
{
	return intersectSphere(ray, parameter);
}

bool Sphere::intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData)
{
	return intersectSphere(ray, parameter);
}

bool Sphere::intersectSphere(const Ray & ray, float & parameter)
{
	//Find the distance squared between the ray origin and center
	glm::vec3 rayOriginToSphereCenter = this->position - ray.getOrigin();
	float distanceSquared = glm::dot(rayOriginToSphereCenter, rayOriginToSphereCenter);

	//Check if the ray origin is outside the sphere by checking if the squared distance is larger than the raidus squared
	bool outsideSphere = distanceSquared >= this->radiusSquared;

	//Find the the distance along the ray which is above the sphere center
	float closestApproachParameter = glm::dot(rayOriginToSphereCenter, ray.getDirectionVector());
	//If the point is outside the sphere, and the distance is negative, the ray is facing away from the sphere and does not intersect
	if (outsideSphere && closestApproachParameter < 0)
	{
		return false;
	}

	//Find the squared distance along the ray from the end of the closest approach parameter to the edge of the sphere
	//This distance is found by finding the distance from the sphere center to the end of the closestApproachParameter using the pythagorian theorem, distance = distanceSquared - closestApproachParameterSquared
	//This is substituted into another right triangle calculation where (radius squared - distance) which gives the equation below
	float halfChordDistanceSquared = this->radiusSquared - distanceSquared + closestApproachParameter * closestApproachParameter;
	if (outsideSphere && halfChordDistanceSquared < 0)
	{
		return false;
	}

	if (outsideSphere)
	{
		//calculate the ray parameter by taking the closestApproachParameter minus the halfChordDistance to get the intersection point on the sphere
		parameter = closestApproachParameter - sqrt(halfChordDistanceSquared);
	}
	else
	{
		//If the ray is inside the sphere, the halfChordDistance needs to be added
		parameter = closestApproachParameter + sqrt(halfChordDistanceSquared);
	}

	return true;
}
