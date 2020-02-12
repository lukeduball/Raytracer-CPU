#include "Sphere.h"

#include <glm/geometric.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

Sphere::Sphere(glm::vec3 pos, float rad, glm::vec3 color) : Object(pos), radius(rad) 
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
	bool outsideSphere = distanceSquared >= this->radius*this->radius;

	float closestApproachParameter = glm::dot(rayOriginToSphereCenter, ray.getDirectionVector());
	if (outsideSphere && closestApproachParameter < 0)
	{
		return false;
	}

	float halfChordDistanceSquared = this->radius*this->radius - distanceSquared + closestApproachParameter * closestApproachParameter;
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
