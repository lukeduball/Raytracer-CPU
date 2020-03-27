#include "DiffuseMaterial.h"

DiffuseMaterial::DiffuseMaterial(glm::vec3 color) : Material(Type::DIFFUSE, color)
{
}

DiffuseMaterial::DiffuseMaterial(int32_t texID) : Material(Type::DIFFUSE, texID)
{
}
