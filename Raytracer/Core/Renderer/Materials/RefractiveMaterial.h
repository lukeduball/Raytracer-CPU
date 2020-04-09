#pragma once

#include "Material.h"

class RefractiveMaterial : public Material
{
public:
	RefractiveMaterial(float ior, bool smooth = false);

	float getIndexOfRefraction();

private:
	float indexOfRefraction;
};