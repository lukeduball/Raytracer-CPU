#pragma once

#include <glm/vec3.hpp>

class Material
{
public:
	enum class Type { PHONG, REFLECT, REFLECT_AND_REFRACT };

	Material();

	Type getMaterialType() const;
	glm::vec3 getAlbedo() const;
	int32_t getTextureID() const;
	bool isSmoothShading() const;

protected:
	Material(Type t, glm::vec3 color, bool smooth);
	Material(Type t, int32_t texID, bool smooth);

private:
	Type type;
	glm::vec3 albedo;
	int32_t textureID;
	bool smoothShading;
};