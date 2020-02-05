#pragma once

#include <glm/vec3.hpp>

struct IntersectionResult
{
	bool doesIntersect = false;
	glm::vec3 intersectionPoint;
	glm::vec3 intersectionNormal;
};

class Ray
{
public:
	Ray(glm::vec3 o, glm::vec3 d);

	glm::vec3 getOrigin();
	glm::vec3 getDirectionVector();

private:
	glm::vec3 origin;
	glm::vec3 direction;
};