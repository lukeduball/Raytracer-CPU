#include "RefractiveMaterial.h"

RefractiveMaterial::RefractiveMaterial(float ior, bool smooth) : Material(Type::REFLECT_AND_REFRACT, glm::vec3(0.0f), smooth), indexOfRefraction(ior)
{
}

float RefractiveMaterial::getIndexOfRefraction()
{
	return indexOfRefraction;
}
