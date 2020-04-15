#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

class Ray
{
public:
	enum class Type { PRIMARY, SHADOW };

	Ray(glm::vec3 o, glm::vec3 d, Type t = Type::PRIMARY);

	Type getRayType() const;
	glm::vec3 getOrigin() const;
	glm::vec3 getDirectionVector() const;

	static Ray convertToNewSpace(const Ray& ray, const glm::mat4& matrix);

private:
	Type type;
	glm::vec3 origin;
	glm::vec3 direction;
};