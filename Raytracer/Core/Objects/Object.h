#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

class Ray;

class Object
{
public:
	virtual bool intersect(const Ray & ray, float & parameter) = 0;

	virtual glm::vec3 getNormalData(glm::vec3 &intersectionPoint) = 0;
	virtual glm::vec2 getTextureCoordData(glm::vec3 &intersectionPoint, glm::vec3 &normal) = 0;

	glm::vec3 getAlbedo();

protected:
	Object(glm::vec3 pos);
	glm::vec3 position;
	glm::vec3 albedo;

private:
};