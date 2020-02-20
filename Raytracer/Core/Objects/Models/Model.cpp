#include "Model.h"

#include "../../Renderer/Ray.h"
#include "../Triangle.h"
#include "../../Math/MathFunctions.h"
#include "Mesh.h"

Model::Model(glm::vec3 pos, glm::vec3 color, Mesh * m) : Object(pos), mesh(m)
{
	this->albedo = color;
}

bool Model::intersect(const Ray & ray, float & parameter)
{
	this->transformedVertices = mesh->vertices;
	float nearestParameter = MathFunctions::T_INFINITY;

	size_t numTriangles = mesh->indices.size() / 3;
	for (uint32_t i = 0; i < numTriangles; i++)
	{
		uint32_t j = i * 3;
		uint32_t vertex1Index = this->mesh->indices[j];
		uint32_t vertex2Index = this->mesh->indices[j + 1];
		uint32_t vertex3Index = this->mesh->indices[j + 2];

		glm::vec3 & vertex1 = this->transformedVertices[vertex1Index];
		glm::vec3 & vertex2 = this->transformedVertices[vertex2Index];
		glm::vec3 & vertex3 = this->transformedVertices[vertex3Index];

		float rayParameter = MathFunctions::T_INFINITY;
		if (Triangle::intersectTriangle(ray, vertex1, vertex2, vertex3, rayParameter) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < nearestParameter)
		{
			parameter = rayParameter;
		}
	}
	return parameter != MathFunctions::T_INFINITY;
}

glm::vec3 Model::getNormalData(glm::vec3 & intersectionPoint)
{
	return glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec2 Model::getTextureCoordData(glm::vec3 & intersectionPoint, glm::vec3 & normal)
{
	return glm::vec2();
}
