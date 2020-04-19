#pragma once

#include <glm/vec3.hpp>

#include <vector>

class Ray;

class AABB
{
public:
	AABB(float miX, float miY, float miZ, float maX, float maY, float maZ);
	AABB(glm::vec3 c, glm::vec3 hDist);

	bool intersect(const Ray & ray, float & t);
	bool isTriangleOverlapping(const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3);
	bool doesPlaneIntersect(glm::vec3 planeNormal, float planeConstant);

	glm::vec3 getMinAsPoint();
	glm::vec3 getMaxAsPoint();
	glm::vec3 getCenter() const;
	glm::vec3 getHalfDistances() const;

	static AABB * calculateBoundingBox(const std::vector<glm::vec3> & pointsList);

private:
	glm::vec3 center;
	glm::vec3 halfDistances;

	bool axisTest(float edge1, float edge2, float v00, float v01, float v10, float v11, float absEdge0, float box0, float absEdge1, float box1);
};