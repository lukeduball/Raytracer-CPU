#pragma once

#include "Object.h"

#include <vector>

#include <glm/vec3.hpp>

struct Face;
class Mesh;
class Material;
class Model;

class Entity : public Object
{
public:
	Entity(glm::vec3 pos, float s, Model * m, Material * material);

	void setRotation(float pitch, float yaw, float roll);

	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material & material);

private:
	Model * model;

	float scale;
	float yawRotation;
	float pitchRotation;
	float rollRotation;
	std::vector<std::vector<glm::vec3>> transformedVerticesList;

	glm::vec2 calculateUVCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face & face, const uint32_t & meshIndex);

	void transformModelVertices();
};