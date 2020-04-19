#include "Entity.h"

#include "Models/Model.h"
#include "Models/Mesh.h"
#include "../Renderer/Materials/Material.h"
#include "../Math/MathFunctions.h"
#include "../Geometry/AABB.h"
#include "../Renderer/Ray.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(glm::vec3 pos, float s, Model * m, Material * material) : Object(pos, material), scale(s), model(m), pitchRotation(0.0f), yawRotation(0.0f), rollRotation(0.0f)
{
	this->calculateTransformationMatrices();
}

void Entity::setRotation(float pitch, float yaw, float roll)
{
	this->pitchRotation = pitch;
	this->yawRotation = yaw;
	this->rollRotation = roll;
	this->calculateTransformationMatrices();
}

bool Entity::intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData)
{
	Ray localRay = Ray::convertToNewSpace(ray, this->worldToLocalMatrix);

	//Check to make sure the model bounding box exists
	if (model->getModelBoundingBox())
	{
		//Check if the ray intersects with the models bounding box to see if there could be an intersection
		float t = MathFunctions::T_INFINITY;
		if (model->getModelBoundingBox()->intersect(localRay, t))
		{
			return false;
		}
	}

	for (uint32_t j = 0; j < this->model->getMeshList().size(); j++)
	{
		Mesh * mesh = this->model->getMeshList()[j];


		//Check to make sure that the ray intersects with the mesh's bounding box
		float t = MathFunctions::T_INFINITY;
		Face * intersectedFace = nullptr;
		if (mesh->intersectMesh(localRay, t, intersectedFace) && t < parameter)
		{
			parameter = t;
			//Capture the index number of the triangle for use in normal calculation later
			intersectionData.face = intersectedFace;
			//Capture the index of the mesh for use in calculations later
			intersectionData.meshIndex = j;
		}
	}

	if (parameter != MathFunctions::T_INFINITY)
	{
		parameter = this->convertLocalParameterToWorldParameter(localRay, parameter, ray);
		return true;
	}

	//If the paramter is equal to Infinity, the ray did not intersect the model
	return false;
}

void Entity::getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material *& material)
{
	//Get the current mesh intersected from the index provided by the intersection data
	Mesh * mesh = this->model->getMeshList()[intersectionData.meshIndex];
	//Get the vertex data from the index provided by the intersection data
	Face * face = intersectionData.face;

	//Convert the intersection point to local coordinates so that per mesh data can be used
	glm::vec3 localIntersectionPoint = this->worldToLocalMatrix * glm::vec4(intersectionPoint, 1.0f);

	//If the underlaying Entity has a material, override all other mesh specific materials with this material
	if (this->getMaterial())
	{
		material = this->getMaterial();
	}
	else
	{
		material = face->material;
	}

	glm::vec3 localNormal;
	if (mesh->normals.size() > 0)
	{
		if (material->isSmoothShading())
		{
			localNormal = this->getSmoothNormal(localIntersectionPoint, face, intersectionData.meshIndex);
		}
		else
		{
			localNormal = (mesh->normals[face->indices[0]] + mesh->normals[face->indices[1]] + mesh->normals[face->indices[2]]) / 3.0f;
		}
	}
	else
	{
		glm::vec3 vertex1 = mesh->vertices[face->indices[0]];
		glm::vec3 vertex2 = mesh->vertices[face->indices[1]];
		glm::vec3 vertex3 = mesh->vertices[face->indices[2]];
		//Calculate the normal by taking the cross product of the difference of the vertices
		localNormal = glm::cross(vertex2 - vertex1, vertex3 - vertex1);
	}

	normal = glm::normalize(localToWorldMatrix * glm::vec4(localNormal, 0.0f));

	if (mesh->textureCoords.size() > 0)
	{
		textureCoords = calculateUVCoordinatesAtIntersection(localIntersectionPoint, face, intersectionData.meshIndex);
	}
	else
	{
		textureCoords = glm::vec2(0, 0);
	}
}

float Entity::convertLocalParameterToWorldParameter(const Ray & localRay, float localParameter, const Ray & worldRay)
{
	glm::vec3 localIntersection = localRay.getOrigin() + localRay.getDirectionVector() * localParameter;
	glm::vec3 globalIntersection = this->localToWorldMatrix * glm::vec4(localIntersection, 1.0f);
	return (globalIntersection - worldRay.getOrigin()).x / worldRay.getDirectionVector().x;
}

glm::vec3 Entity::getSmoothNormal(const glm::vec3 & intersectionPoint, const Face * face, const uint32_t & meshIndex)
{
	Mesh * mesh = this->model->getMeshList()[meshIndex];

	glm::vec3 barycentricCoords = getBarycentricCoordinatesAtIntersection(intersectionPoint, face, meshIndex);

	glm::vec3 normal1 = mesh->normals[face->indices[0]];
	glm::vec3 normal2 = mesh->normals[face->indices[1]];
	glm::vec3 normal3 = mesh->normals[face->indices[2]];
	return normal1 * barycentricCoords.x + normal2 * barycentricCoords.y + normal3 * barycentricCoords.z;
}

glm::vec3 Entity::getBarycentricCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face * face, const uint32_t & meshIndex)
{
	Mesh * mesh = this->model->getMeshList()[meshIndex];

	//Get the vertex positions for each vertex in the face
	glm::vec3 vertex1 = mesh->vertices[face->indices[0]];
	glm::vec3 vertex2 = mesh->vertices[face->indices[1]];
	glm::vec3 vertex3 = mesh->vertices[face->indices[2]];

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

	return glm::vec3(area1Ratio, area2Ratio, area3Ratio);
}

glm::vec2 Entity::calculateUVCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face * face, const uint32_t & meshIndex)
{
	Mesh * mesh = this->model->getMeshList()[meshIndex];

	glm::vec3 barycentricCoords = getBarycentricCoordinatesAtIntersection(intersectionPoint, face, meshIndex);

	//Get the UV coordinates at each of the vertices
	glm::vec2 v1UVCoords = mesh->textureCoords[face->indices[0]];
	glm::vec2 v2UVCoords = mesh->textureCoords[face->indices[1]];
	glm::vec2 v3UVCoords = mesh->textureCoords[face->indices[2]];

	//Interpolate each UV coordinate from the 3 vertices using the area ratios calculated from the subtriangles
	return v1UVCoords * barycentricCoords.x + v2UVCoords * barycentricCoords.y + v3UVCoords * barycentricCoords.z;
}

void Entity::calculateTransformationMatrices()
{
	//Create the local to world matrix by applying position transformation, rotational transformation, and scale transformation
	this->localToWorldMatrix = glm::mat4(1.0f);
	this->localToWorldMatrix = glm::translate(this->localToWorldMatrix, position);
	this->localToWorldMatrix = glm::rotate(this->localToWorldMatrix, glm::radians(pitchRotation), glm::vec3(1, 0, 0));
	this->localToWorldMatrix = glm::rotate(this->localToWorldMatrix, glm::radians(yawRotation), glm::vec3(0, 1, 0));
	this->localToWorldMatrix = glm::rotate(this->localToWorldMatrix, glm::radians(rollRotation), glm::vec3(0, 0, 1));
	this->localToWorldMatrix = glm::scale(this->localToWorldMatrix, glm::vec3(scale));

	//Create the world to local matrix by applying the inverse of the local to world matrix
	this->worldToLocalMatrix = glm::inverse(this->localToWorldMatrix);
}

AABB * Entity::calculateBoundingBox(const std::vector<glm::vec3> & pointList)
{
	glm::vec3 firstVert = pointList[0];
	float minX = firstVert.x, maxX = firstVert.x;
	float minY = firstVert.y, maxY = firstVert.y;
	float minZ = firstVert.z, maxZ = firstVert.z;

	for (uint32_t i = 1; i < pointList.size(); i++)
	{
		glm::vec3 vert = pointList[i];
		minX = std::min(minX, vert.x);
		maxX = std::max(maxX, vert.x);

		minY = std::min(minY, vert.y);
		maxY = std::max(maxY, vert.y);

		minZ = std::min(minZ, vert.z);
		maxZ = std::max(maxZ, vert.z);
	}

	return new AABB(minX, minY, minZ, maxX, maxY, maxZ);
}
