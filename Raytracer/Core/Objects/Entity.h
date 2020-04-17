#pragma once

#include "Object.h"

#include <vector>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

struct Face;
class Mesh;
class Material;
class Model;
class AABB;

class Entity : public Object
{
public:
	Entity(glm::vec3 pos, float s, Model * m, Material * material);

	void setRotation(float pitch, float yaw, float roll);

	bool intersect(const Ray & ray, float & parameter, IntersectionData & intersectionData);

	void getSurfaceData(const glm::vec3 & intersectionPoint, const IntersectionData & intersectionData, glm::vec3 & normal, glm::vec2 & textureCoords, Material *& material);

private:
	Model * model;

	float scale;
	float yawRotation;
	float pitchRotation;
	float rollRotation;
	glm::mat4 worldToLocalMatrix;
	glm::mat4 localToWorldMatrix;

	float convertLocalParameterToWorldParameter(const Ray & localRay, float localParameter, const Ray & worldRay);

	glm::vec3 getSmoothNormal(const glm::vec3 & intersectionPoint, const Face * face, const uint32_t & meshIndex);
	glm::vec3 getBarycentricCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face * face, const uint32_t & meshIndex);
	glm::vec2 calculateUVCoordinatesAtIntersection(const glm::vec3 & intersectionPoint, const Face * face, const uint32_t & meshIndex);

	void calculateTransformationMatrices();
	AABB * calculateBoundingBox(const std::vector<glm::vec3> & pointList);
};