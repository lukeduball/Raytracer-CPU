#include "Material.h"

//Set the textureID to a negative number to indicate that there is no texture associated with this material
Material::Material(Type t, glm::vec3 color) : type(t), textureID(-1), albedo(color)
{
}

Material::Material(Type t, int32_t texID) : type(t), textureID(texID), albedo(glm::vec3(1.0f, 1.0f, 1.0f))
{
}

Material::Material() : type(Type::DIFFUSE)
{
}

Material::Type Material::getMaterialType() const
{
	return this->type;
}

glm::vec3 Material::getAlbedo() const
{
	return this->albedo;
}

int32_t Material::getTextureID() const
{
	return this->textureID;
}
