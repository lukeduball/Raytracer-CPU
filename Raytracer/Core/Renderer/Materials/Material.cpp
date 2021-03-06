#include "Material.h"

//Set the textureID to a negative number to indicate that there is no texture associated with this material
Material::Material(Type t, glm::vec3 color, bool smooth) : type(t), textureID(-1), albedo(color), smoothShading(smooth)
{
}

Material::Material(Type t, int32_t texID, bool smooth) : type(t), textureID(texID), albedo(glm::vec3(1.0f, 1.0f, 1.0f)), smoothShading(smooth)
{
}

Material::Material() : type(Type::PHONG)
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

bool Material::isSmoothShading() const
{
	return smoothShading;
}
