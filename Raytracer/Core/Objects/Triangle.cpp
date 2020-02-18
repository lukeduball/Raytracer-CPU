#include "Triangle.h"

#include "../Renderer/Ray.h"
#include "../Math/MathFunctions.h"

#include <glm/geometric.hpp>

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color) : Object((v1 + v2 + v3) / 3.0f), vertex1(v1), vertex2(v2), vertex3(v3)
{
	this->albedo = color;
}

//Implmentation of Moller-Trumbore Algorithm from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool Triangle::intersect(const Ray & ray, float & parameter)
{
	//Calculate the vectors for two sides of the triangle
	glm::vec3 sideV1V2 = this->vertex2 - this->vertex1;
	glm::vec3 sideV1V3 = this->vertex3 - this->vertex1;
	//Perform a triple product to calculate the determinant
	glm::vec3 directionSideCross = glm::cross(ray.getDirectionVector(), sideV1V3);
	float determinant = glm::dot(sideV1V2, directionSideCross);

	//When the determinant is 0, the ray and the triangle are parallel
	if (ARE_FLOATS_EQUAL(determinant, 0.0f))
	{
		return false;
	}

	float inverseDeterminant = 1 / determinant;

	//Gives a normalized coordinate on the triangle, if the coordinate is between 0 and 1 it lies within the triangle
	glm::vec3 rayParamaterVector = ray.getOrigin() - this->vertex1;
	float u = glm::dot(rayParamaterVector, directionSideCross) * inverseDeterminant;
	if (u < 0 || u > 1)
	{
		//The point lies outside the triangle
		return false;
	}

	glm::vec3 qvec = glm::cross(rayParamaterVector, sideV1V2);
	float v = glm::dot(ray.getDirectionVector(), qvec) * inverseDeterminant;
	if (v < 0 || u + v > 1)
	{
		//The point lies outside the triangle
		return false;
	}

	parameter = glm::dot(sideV1V3, qvec) * inverseDeterminant;

	return true;
}

glm::vec3 Triangle::getNormalData(glm::vec3 & intersectionPoint)
{
	return glm::normalize(glm::cross(this->vertex2 - this->vertex1, this->vertex3 - this->vertex1));
}

glm::vec2 Triangle::getTextureCoordData(glm::vec3 & intersectionPoint, glm::vec3 & normal)
{
	return glm::vec2();
}
