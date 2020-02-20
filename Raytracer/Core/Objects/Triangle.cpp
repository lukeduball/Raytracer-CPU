#include "Triangle.h"

#include "../Renderer/Ray.h"
#include "../Math/MathFunctions.h"

#include <glm/geometric.hpp>

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color) : Object((v1 + v2 + v3) / 3.0f), vertex1(v1), vertex2(v2), vertex3(v3)
{
	this->albedo = color;
}

bool Triangle::intersect(const Ray & ray, float & parameter)
{
	return intersectTriangle(ray, this->vertex1, this->vertex2, this->vertex3, parameter);
}

glm::vec3 Triangle::getNormalData(glm::vec3 & intersectionPoint)
{
	return glm::normalize(glm::cross(this->vertex2 - this->vertex1, this->vertex3 - this->vertex1));
}

glm::vec2 Triangle::getTextureCoordData(glm::vec3 & intersectionPoint, glm::vec3 & normal)
{
	return glm::vec2();
}

//Implmentation of Moller-Trumbore Algorithm from https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
bool Triangle::intersectTriangle(const Ray & ray, const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, float & parameter)
{
	//Calculate the vectors for two sides of the triangle
	glm::vec3 sideV1V2 = vertex2 - vertex1;
	glm::vec3 sideV1V3 = vertex3 - vertex1;
	//Perform a triple product to calculate the determinant
	glm::vec3 directionEdgeV1V3Cross = glm::cross(ray.getDirectionVector(), sideV1V3);
	float determinant = glm::dot(sideV1V2, directionEdgeV1V3Cross);

	//When the determinant is 0, the ray and the triangle are parallel
	if (ARE_FLOATS_EQUAL(determinant, 0.0f))
	{
		return false;
	}

	float inverseDeterminant = 1.0f / determinant;

	//Find normalized u coordinate of triangle by performing triple scalar product with Ray Direction Vector, V1V3Edge, and Origin-V1
	glm::vec3 originV1Vector = ray.getOrigin() - vertex1;
	float u = glm::dot(originV1Vector, directionEdgeV1V3Cross) * inverseDeterminant;
	if (u < 0 || u > 1)
	{
		//The point lies outside the triangle
		return false;
	}

	//Find normalized v coordiante of the triangle by performing triple scalr product with Origin-V1, V1V2Edge, and Ray Direction Vector
	glm::vec3 originEdgeV1V2Cross = glm::cross(originV1Vector, sideV1V2);
	float v = glm::dot(ray.getDirectionVector(), originEdgeV1V2Cross) * inverseDeterminant;
	//if u+v is greater than 1, the point lies outside of the triangle
	if (v < 0 || u + v > 1)
	{
		//The point lies outside the triangle
		return false;
	}

	//Find the ray to intersection point distance parameter by performing triple product of Origin-V1, EdgeV1V2, and EdgeV1V3
	parameter = glm::dot(sideV1V3, originEdgeV1V2Cross) * inverseDeterminant;

	//If the paramater value for the ray is less than zero, the intersection is facing the wrong direction
	if (parameter < 0)
	{
		return false;
	}

	return true;
}
