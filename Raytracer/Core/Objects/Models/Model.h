#pragma once

#include <vector>
#include "../Object.h"
#include "Mesh.h"

#include <assimp/scene.h> 

class Model : public Object
{
public:
	Model(glm::vec3 pos, float s, Mesh * mesh, Material * material);
	Model(glm::vec3 pos, float s, std::string path, Material * material);

	void setRotation(float pitch, float yaw, float roll);

	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords);

private:
	float scale;
	float yawRotation;
	float pitchRotation;
	float rollRotation;
	std::vector<glm::vec3> transformedVertices;
	std::vector<Mesh> meshList;
	Mesh * mesh;

	void processNode(aiNode * node, const aiScene * scene);
	Mesh processMesh(aiMesh *mesh, const aiScene * scene);

	void transformModelVertices();

};