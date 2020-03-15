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
	enum class Type { PRIMARY, SHADOW };

	Ray(glm::vec3 o, glm::vec3 d, Type t = Type::PRIMARY);

	Type getRayType() const;
	glm::vec3 getOrigin() const;
	glm::vec3 getDirectionVector() const;

private:
	Type type;
	glm::vec3 origin;
	glm::vec3 direction;
};