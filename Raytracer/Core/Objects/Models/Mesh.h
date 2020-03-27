#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <assimp/scene.h>

class Material;

struct Face
{
	uint32_t indices[3];
	Material * material;

};

class Mesh
{
public:
	Mesh();
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
};