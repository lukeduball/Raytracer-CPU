#pragma once

#include <glm/vec3.hpp>

class Material
{
public:
	enum class Type { DIFFUSE, REFLECT, REFLECT_AND_REFRACT };

	Type getMaterialType();
	glm::vec3 getAlbedo();

protected:
	Material(Type t, glm::vec3 color);

private:
	Type type;
	glm::vec3 albedo;
};