#pragma once

#include "Object.h"

class Triangle: public Object
{
public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 color);

	bool intersect(const Ray & ray, float & parameter);

	glm::vec3 getNormalData(glm::vec3 &intersectionPoint);
	glm::vec2 getTextureCoordData(glm::vec3 &intersectionPoint, glm::vec3 &normal);

	static bool intersectTriangle(const Ray & ray, const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, float & parameter);

private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
};