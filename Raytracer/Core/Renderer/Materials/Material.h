#pragma once

#include <glm/vec3.hpp>

class Material
{
public:
	enum class Type { DIFFUSE, REFLECT, REFLECT_AND_REFRACT };

	Material(Type type, glm::vec3 color);

	Type getMaterialType();
	glm::vec3 getAlbedo();

private:
	Type type;
	glm::vec3 albedo;
};