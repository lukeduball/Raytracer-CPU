#pragma once

#include <vector>
#include <glm/vec3.hpp>

class Mesh
{
public:
	std::vector<uint32_t> indices;
	std::vector<glm::vec3> vertices;
};