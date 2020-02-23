#include "Model.h"

#include "../../Renderer/Ray.h"
#include "../Triangle.h"
#include "../../Math/MathFunctions.h"
#include "Mesh.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(glm::vec3 pos, float s, glm::vec3 color, Mesh * m) : Object(pos), mesh(m), scale(s)
{
	this->albedo = color;
	this->transformedVertices.resize(mesh->vertices.size());
	//Calculates the transformed vertices for this object's scale and position
	this->transformModelVertices();
}

bool Model::intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData)
{
	float nearestParameter = MathFunctions::T_INFINITY;

	//Calculate the number of triangles by taking the indices and dividing by 3
	size_t numTriangles = mesh->indices.size() / 3;
	for (uint32_t i = 0; i < numTriangles; i++)
	{
		//Calculate the index by multiplying the current triangle by 3
		uint32_t j = i * 3;
		uint32_t vertex1Index = this->mesh->indices[j];
		uint32_t vertex2Index = this->mesh->indices[j + 1];
		uint32_t vertex3Index = this->mesh->indices[j + 2];

		//Get the vertex data for the data at the given indices
		glm::vec3 & vertex1 = this->transformedVertices[vertex1Index];
		glm::vec3 & vertex2 = this->transformedVertices[vertex2Index];
		glm::vec3 & vertex3 = this->transformedVertices[vertex3Index];

		float rayParameter = MathFunctions::T_INFINITY;
		//Check the triangle for intersection, do not accept an intersection if the rayParameter is zero because the intersection is with the same face and check it is the nearest intersection
		if (Triangle::intersectTriangle(ray, vertex1, vertex2, vertex3, rayParameter) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < nearestParameter)
		{
			parameter = rayParameter;
			//Capture the index number of the triangle for use in normal calculation later
			intersectionData.index = j;
		}
	}
	//If the paramter is equal to Infinity, the ray did not intersect the model
	return parameter != MathFunctions::T_INFINITY;
}

void Model::getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords)
{
	//Get the vertex data from the index provided by the intersection data
	glm::vec3 vertex1 = this->transformedVertices[this->mesh->indices[intersectionData.index]];
	glm::vec3 vertex2 = this->transformedVertices[this->mesh->indices[intersectionData.index + 1]];
	glm::vec3 vertex3 = this->transformedVertices[this->mesh->indices[intersectionData.index + 2]];
	//Calculate the normal by taking the cross product of the difference of the vertices
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex1));

	textureCoords = glm::vec2();
}

void Model::transformModelVertices()
{
	glm::mat4 transformMatrix = glm::mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, position);
	transformMatrix = glm::scale(transformMatrix, glm::vec3(scale));
	for (uint32_t i = 0; i < mesh->vertices.size(); i++)
	{
		//Transform the vertices by using the transform matrix multiplied by the meshes vertex data
		this->transformedVertices[i] = transformMatrix * glm::vec4(mesh->vertices[i], 1.0f);
	}
}
