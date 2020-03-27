#pragma once

#include <glm/vec3.hpp>

class Material
{
public:
	enum class Type { DIFFUSE, REFLECT, REFLECT_AND_REFRACT };

	Type getMaterialType() const;
	glm::vec3 getAlbedo() const;
	int32_t getTextureID() const;

protected:
	Material(Type t, glm::vec3 color);
	Material(Type t, int32_t texID);

private:
	Type type;
	glm::vec3 albedo;
	int32_t textureID;
};