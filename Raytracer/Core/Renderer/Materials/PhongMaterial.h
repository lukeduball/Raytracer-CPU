#pragma once

#include "Material.h"

class PhongMaterial : public Material
{
public:
	PhongMaterial(glm::vec3 color, float d, float s, float p);
	PhongMaterial(int32_t texID, float d, float s, float p);

	float getPowerComponent();
	float getSpecularComponent();
	float getDiffuseComponent();

private:
	float specularComponent;
	//Specular value is affected by powerComponent and a different value can change the look of the material
	float powerComponent;
	float diffuseComponent;
};