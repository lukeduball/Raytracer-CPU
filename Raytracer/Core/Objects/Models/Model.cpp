#include "Model.h"

#include "../../Renderer/Ray.h"
#include "../Triangle.h"
#include "../../Math/MathFunctions.h"
#include "../../Renderer/Materials/Material.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

Model::Model(Mesh * m)
{
	this->meshList.push_back(*m);
}

Model::Model(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "(ERROR) ASSIMP: " << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

std::vector<Mesh>& Model::getMeshList()
{
	return this->meshList;
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	//Create a mesh object for all the meshes in this node
	for (uint32_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshList.push_back(processMesh(mesh, scene));
	}

	//Process the nodes for each of its children
	for (uint32_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	Mesh result;

	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		result.vertices.push_back(position);

		//Check if the mesh contains texture coordinates
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textureCoords;
			textureCoords.x = mesh->mTextureCoords[0][i].x;
			textureCoords.y = mesh->mTextureCoords[0][i].y;
			result.textureCoords.push_back(textureCoords);
		}
	}

	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		Face resultFace;
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			resultFace.indices[j] = face.mIndices[j];
		}

		//Load materials, normals, and texture coordinates

		result.faces.push_back(resultFace);
	}

	return result;
}
