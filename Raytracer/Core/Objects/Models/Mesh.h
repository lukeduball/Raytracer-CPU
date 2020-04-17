#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

#include <assimp/scene.h>

class Material;
class AABB;

template<typename T>
class Octree;

class OctreeNode;
class Ray;

struct Face
{
	uint32_t indices[3];
	Material * material;

};

class Mesh
{
public:
	Mesh();
	~Mesh();
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textureCoords;
	std::vector<glm::vec3> normals;
	Octree<uint32_t> * boundingOctree;

	void constructOctree();
	bool intersectMesh(const Ray & ray, float & parameter, Face *& intersectedFace);

private:
	void generateChildren(OctreeNode * node, const std::vector<uint32_t> & triContents, uint32_t depth);
};