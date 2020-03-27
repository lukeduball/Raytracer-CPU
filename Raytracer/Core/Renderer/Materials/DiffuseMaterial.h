#pragma once

#include "Material.h"

class DiffuseMaterial : public Material
{
public:
	DiffuseMaterial(glm::vec3 color);
	DiffuseMaterial(int32_t texID);

};