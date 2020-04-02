#include "Object.h"

Material * Object::getMaterial()
{
	return this->material;
}

Object::Object(glm::vec3 pos, Material* material) : position(pos), material(material) {}
