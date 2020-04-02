#include "PhongMaterial.h"

PhongMaterial::PhongMaterial(glm::vec3 color, float d, float s, float p) : Material(Type::PHONG, color), diffuseComponent(d), specularComponent(s), powerComponent(p)
{
}

PhongMaterial::PhongMaterial(int32_t texID, float d, float s, float p) : Material(Type::PHONG, texID), diffuseComponent(d), specularComponent(s), powerComponent(p)
{
}

float PhongMaterial::getPowerComponent()
{
	return this->powerComponent;
}

float PhongMaterial::getSpecularComponent()
{
	return this->specularComponent;
}

float PhongMaterial::getDiffuseComponent()
{
	return this->diffuseComponent;
}
