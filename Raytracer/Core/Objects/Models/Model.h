#pragma once

#include <vector>
#include "../Object.h"
#include "Mesh.h"

#include <assimp/scene.h> 

class Model
{
public:
	Model(Mesh * mesh);
	Model(std::string path);
	~Model();

	std::vector<Mesh*> & getMeshList();
	AABB * getModelBoundingBox();

private:
	float scale;
	float yawRotation;
	float pitchRotation;
	float rollRotation;
	std::vector<Mesh*> meshList;
	AABB* modelBoundingBox;

	void processNode(aiNode * node, const aiScene * scene);
	Mesh * processMesh(aiMesh *mesh, const aiScene * scene);

	void calculateModelBoundingBox();

};