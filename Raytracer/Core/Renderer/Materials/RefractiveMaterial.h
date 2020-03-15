#pragma once

#include "Material.h"

class RefractiveMaterial : public Material
{
public:
	RefractiveMaterial(float ior);

	float getIndexOfRefraction();

private:
	float indexOfRefraction;
};