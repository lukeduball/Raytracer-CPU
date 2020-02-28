#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include <assimp/scene.h>

class Mesh
{
public:
	Mesh();
	std::vector<uint32_t> indices;
	std::vector<glm::vec3> vertices;
};