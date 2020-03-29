#include "Entity.h"

#include "Models/Model.h"
#include "Models/Mesh.h"
#include "../Renderer/Materials/Material.h"
#include "Triangle.h"
#include "../Math/MathFunctions.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 pos, float s, Model * m, Material * material) : Object(pos, material), scale(s), model(m), pitchRotation(0.0f), yawRotation(0.0f), rollRotation(0.0f)
{
	this->transformedVerticesList.resize(this->model->getMeshList().size());

	this->transformModelVertices();
}

void Entity::setRotation(float pitch, float yaw, float roll)
{
	this->pitchRotation = pitch;
	this->yawRotation = yaw;
	this->rollRotation = roll;
	this->transformModelVertices();
}

bool Entity::intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData)
{
	for (uint32_t j = 0; j < this->model->getMeshList().size(); j++)
	{
		Mesh * mesh = this->model->getMeshList()[j];
		size_t numTriangles = mesh->faces.size();
		for (uint32_t i = 0; i < numTriangles; i++)
		{
			Face face = mesh->faces[i];
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

void Entity::getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material & material)
{
	//Get the current mesh intersected from the index provided by the intersection data
	Mesh * mesh = this->model->getMeshList()[intersectionData.meshIndex];
	//Get the vertex data from the index provided by the intersection data
	Face face = mesh->faces[intersectionData.faceIndex];
	glm::vec3 vertex1 = this->transformedVerticesList[intersectionData.meshIndex][face.indices[0]];
	glm::vec3 vertex2 = this->transformedVerticesList[intersectionData.meshIndex][face.indices[1]];
	glm::vec3 vertex3 = this->transformedVerticesList[intersectionData.meshIndex][face.indices[2]];
	//Calculate the normal by taking the cross product of the difference of the vertices
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));

	if (mesh->textureCoords.size() > 0)
	{
		textureCoords = calculateUVCoordinatesAtIntersection(intersectionPoint, face, intersectionData.meshIndex);
	}
	else
	{
		textureCoords = glm::vec2(0, 0);
	}

	//If the underlaying Entity has a material, override all other mesh specific materials with this material
	if (this->material)
	{
		material = this->getMaterial();
	}
	else
	{
		material = *face.material;
	}
}

glm::vec2 Entity::calculateUVCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face & face, const uint32_t & meshIndex)
{
	Mesh * mesh = this->model->getMeshList()[meshIndex];

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
	glm::vec2 v1UVCoords = mesh->textureCoords[face.indices[0]];
	glm::vec2 v2UVCoords = mesh->textureCoords[face.indices[1]];
	glm::vec2 v3UVCoords = mesh->textureCoords[face.indices[2]];

	//Interpolate each UV coordinate from the 3 vertices using the area ratios calculated from the subtriangles
	return v1UVCoords * area1Ratio + v2UVCoords * area2Ratio + v3UVCoords * area3Ratio;
}

void Entity::transformModelVertices()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, position);
	transformMatrix = glm::rotate(transformMatrix, glm::radians(pitchRotation), glm::vec3(1, 0, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(yawRotation), glm::vec3(0, 1, 0));
	transformMatrix = glm::rotate(transformMatrix, glm::radians(rollRotation), glm::vec3(0, 0, 1));
	transformMatrix = glm::scale(transformMatrix, glm::vec3(scale));
	for (uint32_t j = 0; j < this->model->getMeshList().size(); j++)
	{
		Mesh * mesh = this->model->getMeshList()[j];
		this->transformedVerticesList[j].resize(mesh->vertices.size());
		for (uint32_t i = 0; i < mesh->vertices.size(); i++)
		{
			//Transform the vertices by using the transform matrix multiplied by the meshes vertex data
			this->transformedVerticesList[j][i] = transformMatrix * glm::vec4(mesh->vertices[i], 1.0f);
		}
	}
}
