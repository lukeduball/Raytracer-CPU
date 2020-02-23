#pragma once

#include <vector>
#include "../Object.h"

class Mesh;

class Model : public Object
{
public:
	Model(glm::vec3 pos, float s, glm::vec3 color, Mesh * mesh);

	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords);

private:
	float scale;
	std::vector<glm::vec3> transformedVertices;
	Mesh * mesh;

	void transformModelVertices();

};