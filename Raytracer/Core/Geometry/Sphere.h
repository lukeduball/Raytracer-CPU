#pragma once

#include "../Objects/Object.h"
#include "../Renderer/Ray.h"

class Sphere : public Object
{
public:
	Sphere(glm::vec3 pos, float rad, Material * material);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material *& material);

	//This intersection test is meant to be a rough but fast intersection test to cull impossible intersections
	bool possibleIntersection(const Ray & ray, float & parameter);
	//This intersection test is a definitive intersection test to see if a ray intersects an object
	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

private:
	float radius;
	float radiusSquared;

	bool intersectSphere(const Ray & ray, float & parameter);
};