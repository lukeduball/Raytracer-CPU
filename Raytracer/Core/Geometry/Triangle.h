#pragma once

#include "../Objects/Object.h"

class Triangle: public Object
{
public:
	Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, Material * material);

	//This intersection test is meant to be a rough but fast intersection test to cull impossible intersections
	bool possibleIntersection(const Ray & ray, float & parameter);
	//This intersection test is a definitive intersection test to see if a ray intersects an object
	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material *& material);

	static bool intersectTriangle(const Ray & ray, const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3, float & parameter);

private:
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 vertex3;
};