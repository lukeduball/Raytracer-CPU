#pragma once

#include "Object.h"
#include "../Renderer/Ray.h"

class Sphere : public Object
{
public:
	Sphere(glm::vec3 pos, float rad);

	IntersectionResult intersect(Ray ray);

private:
	float radius;
};