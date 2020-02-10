#include "Object.h"

glm::vec3 Object::getColor()
{
	return color;
}

Object::Object(glm::vec3 pos) : position(pos) {}
