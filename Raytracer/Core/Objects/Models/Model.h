#pragma once

#include <vector>
#include "../Object.h"

class Mesh;

class Model : public Object
{
public:
	Model(glm::vec3 pos, float s, glm::vec3 color, Mesh * mesh);

	bool intersect(const Ray & ray, float & parameter);

	glm::vec3 getNormalData(glm::vec3 &intersectionPoint);
	glm::vec2 getTextureCoordData(glm::vec3 &intersectionPoint, glm::vec3 &normal);

private:
	float scale;
	std::vector<glm::vec3> transformedVertices;
	Mesh * mesh;

	void transformModelVertices();

};