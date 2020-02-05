#include "Sphere.h"

#include <glm/geometric.hpp>

Sphere::Sphere(glm::vec3 pos, float rad) : Object(pos), radius(rad) {}

IntersectionResult Sphere::intersect(Ray ray)
{
	IntersectionResult intersectionResult = {};

	//Find the distance squared between the ray origin and center
	glm::vec3 rayOriginToSphereCenter = this->position - ray.getOrigin();
	float distanceSquared = glm::dot(rayOriginToSphereCenter, rayOriginToSphereCenter);
	bool outsideSphere = distanceSquared >= this->radius*this->radius;

	float closestApproachParameter = glm::dot(rayOriginToSphereCenter, ray.getDirectionVector());
	if (outsideSphere && closestApproachParameter < 0)
	{
		intersectionResult.doesIntersect = false;
		return intersectionResult;
	}

	float halfChordDistanceSquared = this->radius*this->radius - distanceSquared + closestApproachParameter * closestApproachParameter;
	if (outsideSphere && halfChordDistanceSquared < 0)
	{
		intersectionResult.doesIntersect = false;
		return intersectionResult;
	}

	float lineParameter;
	if (outsideSphere)
	{
		lineParameter = closestApproachParameter - sqrt(halfChordDistanceSquared);
	}
	else
	{
		lineParameter = closestApproachParameter + sqrt(halfChordDistanceSquared);
	}

	intersectionResult.doesIntersect = true;
	//Use the parametric equation of the ray to find the intersection point
	intersectionResult.intersectionPoint = ray.getOrigin() + (ray.getDirectionVector() * lineParameter);
	//Calculate the normal by subtracting the spheres center and dividing by the spheres radius
	intersectionResult.intersectionNormal = (intersectionResult.intersectionPoint - this->position) / this->radius;
	
	return intersectionResult;
}
