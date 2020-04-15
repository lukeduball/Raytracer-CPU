#include "Ray.h"

#include "glm/geometric.hpp"

Ray::Ray(glm::vec3 o, glm::vec3 d, Type t) : origin(o), direction(glm::normalize(d)), type(t)
{
}

Ray::Type Ray::getRayType() const
{
	return type;
}

glm::vec3 Ray::getOrigin() const
{
	return this->origin;
}

glm::vec3 Ray::getDirectionVector() const
{
	return this->direction;
}

Ray Ray::convertToNewSpace(const Ray & ray, const glm::mat4 & matrix)
{
	glm::vec3 origin = matrix * glm::vec4(ray.getOrigin(), 1.0f);
	glm::vec3 direction = matrix * glm::vec4(ray.getDirectionVector(), 0.0f);
	direction = glm::normalize(direction);
	return Ray(origin, direction, ray.getRayType());
}
