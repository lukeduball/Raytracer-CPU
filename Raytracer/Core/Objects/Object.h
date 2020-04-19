#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Ray;
class Material;
struct Face;

struct IntersectionData
{
	uint32_t meshIndex;
	Face * face;
};

class Object
{
public:
	//This intersection test it meant to be a rough but fast test
	virtual bool possibleIntersection(const Ray & ray, float & parameter) = 0;
	//This intersection test gives a definitive intersection of the object
	virtual bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData) = 0;

	virtual void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material *& material) = 0;
	
	Material * getMaterial();

protected:
	Object(glm::vec3 pos, Material* material);
	glm::vec3 position;
	Material * material;

private:
};