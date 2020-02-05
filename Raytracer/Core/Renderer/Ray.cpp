#include "Ray.h"

#include "glm/geometric.hpp"

Ray::Ray(glm::vec3 o, glm::vec3 d) : origin(o), direction(glm::normalize(d))
{
}

glm::vec3 Ray::getOrigin()
{
	return this->origin;
}

glm::vec3 Ray::getDirectionVector()
{
	return this->direction;
}
