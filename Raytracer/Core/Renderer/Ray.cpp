#include "Ray.h"

#include "glm/geometric.hpp"

Ray::Ray(glm::vec3 o, glm::vec3 d) : origin(o), direction(glm::normalize(d))
{
}

glm::vec3 Ray::getOrigin() const
{
	return this->origin;
}

glm::vec3 Ray::getDirectionVector() const
{
	return this->direction;
}
