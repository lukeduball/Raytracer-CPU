#include "Material.h"

Material::Material(Type type, glm::vec3 color) : type(type), albedo(color)
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
