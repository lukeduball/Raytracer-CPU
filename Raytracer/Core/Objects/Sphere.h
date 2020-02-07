#pragma once

#include "Object.h"
#include "../Renderer/Ray.h"

class Sphere : public Object
{
public:
	Sphere(glm::vec3 pos, float rad);

	bool intersect(const Ray & ray, float & parameter);
	IntersectionResult intersect(Ray ray);

private:
	float radius;
};