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

Model::Model(glm::vec3 pos, float s, Mesh * m, Material * material) : Object(pos, material), scale(s), pitchRotation(0.0f), yawRotation(0.0f), rollRotation(0.0f)
{
	this->meshList.push_back(*m);
	this->transformedVerticesList.resize(1);
	//Calculates the transformed vertices for this object's scale and position
	this->transformModelVertices();
}

Model::Model(glm::vec3 pos, float s, std::string path, Material * material) : Object(pos, material), scale(s), pitchRotation(0.0f), yawRotation(0.0f), rollRotation(0.0f)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "(ERROR) ASSIMP: " << importer.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
	this->transformedVerticesList.resize(this->meshList.size());

	this->transformModelVertices();
}

void Model::setRotation(float pitch, float yaw, float roll)
{
	this->pitchRotation = pitch;
	this->yawRotation = yaw;
	this->rollRotation = roll;
	this->transformModelVertices();
}


bool Model::intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData)
{
	for (uint32_t j = 0; j < this->meshList.size(); j++)
	{
		Mesh mesh = meshList[j];
		size_t numTriangles = mesh.faces.size();
		for (uint32_t i = 0; i < numTriangles; i++)
		{
			Face face = mesh.faces[i];
			uint32_t vertex1Index = face.indices[0];
			uint32_t vertex2Index = face.indices[1];
			uint32_t vertex3Index = face.indices[2];

			//Get the vertex data for the data at the given indices
			glm::vec3 & vertex1 = this->transformedVerticesList[j][vertex1Index];
			glm::vec3 & vertex2 = this->transformedVerticesList[j][vertex2Index];
			glm::vec3 & vertex3 = this->transformedVerticesList[j][vertex3Index];

			float rayParameter = MathFunctions::T_INFINITY;
			//Check the triangle for intersection, do not accept an intersection if the rayParameter is zero because the intersection is with the same face and check it is the nearest intersection
			if (Triangle::intersectTriangle(ray, vertex1, vertex2, vertex3, rayParameter) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < parameter)
			{
				parameter = rayParameter;
				//Capture the index number of the triangle for use in normal calculation later
				intersectionData.faceIndex = i;
				//Capture the index of the mesh for use in calculations later
				intersectionData.meshIndex = j;
			}
		}
	}
	//If the paramter is equal to Infinity, the ray did not intersect the model
	return parameter != MathFunctions::T_INFINITY;
}

void Model::getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords)
{
	//Get the current mesh intersected from the index provided by the intersection data
	Mesh mesh = this->meshList[intersectionData.meshIndex];
	//Get the vertex data from the index provided by the intersection data
	Face face = mesh.faces[intersectionData.faceIndex];
	glm::vec3 vertex1 = this->transformedVerticesList[intersectionData.meshIndex][face.indices[0]];
	glm::vec3 vertex2 = this->transformedVerticesList[intersectionData.meshIndex][face.indices[1]];
	glm::vec3 vertex3 = this->transformedVerticesList[intersectionData.meshIndex][face.indices[2]];
	//Calculate the normal by taking the cross product of the difference of the vertices
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));

	if (mesh.textureCoords.size() > 0)
	{
		textureCoords = calculateUVCoordinatesAtIntersection(intersectionPoint, face, intersectionData.meshIndex);
	}
	else
	{
		textureCoords = glm::vec2(0, 0);
	}
}

glm::vec2 Model::calculateUVCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face & face, const uint32_t & meshIndex)
{
	Mesh mesh = this->meshList[meshIndex];

	//Get the vertex positions for each vertex in the face
	glm::vec3 vertex1 = this->transformedVerticesList[meshIndex][face.indices[0]];
	glm::vec3 vertex2 = this->transformedVerticesList[meshIndex][face.indices[1]];
	glm::vec3 vertex3 = this->transformedVerticesList[meshIndex][face.indices[2]];

	//Calculate the vectors from each vertex to the intersection point
	glm::vec3 v1ToPointVector = vertex1 - intersectionPoint;
	glm::vec3 v2ToPointVector = vertex2 - intersectionPoint;
	glm::vec3 v3ToPointVector = vertex3 - intersectionPoint;

	//Calculates the area of the triangle
	float area = glm::length(glm::cross(vertex1 - vertex2, vertex1 - vertex3));
	//Calculate the ratio that each of the sub triangles areas take up
	float area1Ratio = glm::length(glm::cross(v2ToPointVector, v3ToPointVector)) / area;
	float area2Ratio = glm::length(glm::cross(v3ToPointVector, v1ToPointVector)) / area;
	float area3Ratio = glm::length(glm::cross(v1ToPointVector, v2ToPointVector)) / area;

	//Get the UV coordinates at each of the vertices
	glm::vec2 v1UVCoords = mesh.textureCoords[face.indices[0]];
	glm::vec2 v2UVCoords = mesh.textureCoords[face.indices[1]];
	glm::vec2 v3UVCoords = mesh.textureCoords[face.indices[2]];

	//Interpolate each UV coordinate from the 3 vertices using the area ratios calculated from the subtriangles
	return v1UVCoords * area1Ratio + v2UVCoords * area2Ratio + v3UVCoords * area3Ratio;
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

void Model::transformModelVertices()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, position);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(pitchRotation), glm::vec3(1, 0, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(yawRotation), glm::vec3(0, 1, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(rollRotation), glm::vec3(0, 0, 1));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(scale));
	for (uint32_t j = 0; j < meshList.size(); j++)
	{
		Mesh mesh = meshList[j];
		this->transformedVerticesList[j].resize(mesh.vertices.size());
		for (uint32_t i = 0; i < mesh.vertices.size(); i++)
		{
			//Transform the vertices by using the transform matrix multiplied by the meshes vertex data
			this->transformedVerticesList[j][i] = transformMatrix * glm::vec4(mesh.vertices[i], 1.0f);
		}
	}
}
