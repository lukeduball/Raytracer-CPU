#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Ray;

struct IntersectionData
{
	uint32_t index;
};

class Object
{
public:
	virtual bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData) = 0;

	virtual void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords) = 0;

	glm::vec3 getAlbedo();

protected:
	Object(glm::vec3 pos);
	glm::vec3 position;
	glm::vec3 albedo;

private:
};