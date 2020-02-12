#include "Object.h"

glm::vec3 Object::getAlbedo()
{
	return albedo;
}

Object::Object(glm::vec3 pos) : position(pos) {}
