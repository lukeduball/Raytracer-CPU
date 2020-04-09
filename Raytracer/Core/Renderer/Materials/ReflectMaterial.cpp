#include "ReflectMaterial.h"

ReflectMaterial::ReflectMaterial(bool smooth) : Material(Type::REFLECT, glm::vec3(0.0f), smooth)
{
}
