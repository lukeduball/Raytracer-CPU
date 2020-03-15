#include "Material.h"

Material::Material(Type t, glm::vec3 color) : type(t), albedo(color)
{
}

Material::Type Material::getMaterialType()
{
	return this->type;
}

glm::vec3 Material::getAlbedo()
{
	return this->albedo;
}
