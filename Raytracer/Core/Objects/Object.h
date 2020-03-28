#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Ray;
class Material;

struct IntersectionData
{
	uint32_t meshIndex;
	uint32_t faceIndex;
};

class Object
{
public:
	virtual bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData) = 0;

	virtual void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords) = 0;

	Material & getMaterial();

protected:
	Object(glm::vec3 pos, Material* material);
	glm::vec3 position;
	Material * material;

private:
};