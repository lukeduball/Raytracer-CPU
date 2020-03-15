#include "RefractiveMaterial.h"

RefractiveMaterial::RefractiveMaterial(float ior) : Material(Type::REFLECT_AND_REFRACT, glm::vec3(0.0f)), indexOfRefraction(ior)
{
}

float RefractiveMaterial::getIndexOfRefraction()
{
	return indexOfRefraction;
}
