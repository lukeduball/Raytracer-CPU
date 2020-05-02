#include "AABB.h"

#include "../Renderer/Ray.h"
#include "../Math/MathFunctions.h"

#include <algorithm>

AABB::AABB(float miX, float miY, float miZ, float maX, float maY, float maZ) : AABB(glm::vec3( (miX+maX) / 2.0f, (miY+maY) / 2.0f, (miZ + maZ) / 2.0f), glm::vec3((maX - miX) / 2.0f, (maY - miY) / 2.0f, (maZ - miZ) / 2.0f))
{
}

AABB::AABB(glm::vec3 c, glm::vec3 hDist) : center(c), halfDistances(hDist)
{
}

bool AABB::intersect(const Ray & ray, float & t)
{
	glm::vec3 min = getMinAsPoint();
	glm::vec3 max = getMaxAsPoint();

	float tmin = (min.x - ray.getOrigin().x) / ray.getDirectionVector().x;
	float tmax = (max.x - ray.getOrigin().x) / ray.getDirectionVector().x;

	if (tmin > tmax)
	{
		std::swap(tmin, tmax);
	}

	float tymin = (min.y - ray.getOrigin().y) / ray.getDirectionVector().y;
	float tymax = (max.y - ray.getOrigin().y) / ray.getDirectionVector().y;

	if (tymin > tymax)
	{
		std::swap(tymin, tymax);
	}

	if ((tmin > tymax) || (tymin > tmax))
	{
		return false;
	}

	if (tymin > tmin)
	{
		tmin = tymin;
	}

	if (tymax < tmax)
	{
		tmax = tymax;
	}

	float tzmin = (min.z - ray.getOrigin().z) / ray.getDirectionVector().z;
	float tzmax = (max.z - ray.getOrigin().z) / ray.getDirectionVector().z;

	if (tzmin > tzmax)
	{
		std::swap(tzmin, tzmax);
	}

	if ((tmin > tzmax) || (tzmin > tmax))
	{
		return false;
	}

	if (tzmin > tmin)
	{
		tmin = tzmin;
	}

	if (tzmax < tmax)
	{
		tmax = tzmax;
	}
		
	if (tmin > 0.0f)
	{
		t = tmin;
	}
	else
	{
		t = tmax;
	}

	//Intersection is behind the ray 
	if (t < 0.0f)
	{
		return false;
	}

	return true;
}

bool AABB::isTriangleOverlapping(const glm::vec3 & vertex1, const glm::vec3 & vertex2, const glm::vec3 & vertex3)
{
	//Translate boudning box to the origin and perform that transformation on the triangle as well
	glm::vec3 transformedVertex0 = vertex1 - this->center;
	glm::vec3 transformedVertex1 = vertex2 - this->center;
	glm::vec3 transformedVertex2 = vertex3 - this->center;

	//Calculate the edges of the triangles
	glm::vec3 edge10 = transformedVertex1 - transformedVertex0;
	glm::vec3 edge21 = transformedVertex2 - transformedVertex1;
	glm::vec3 edge02 = transformedVertex0 - transformedVertex2;

	//Test if the box intersects the plane of the triangle
	glm::vec3 triangleNormal = glm::normalize(glm::cross(edge10, edge21));
	float planeConstant = -glm::dot(transformedVertex1, triangleNormal);

	if (!doesPlaneIntersect(triangleNormal, planeConstant))
	{
		return false;
	}

	//Test the minimal AABB around the triangle projected onto each normal of the cube (1,0,0), (0, 1,0) and (0,0,1)
	for (uint32_t i = 0; i < 3; i++)
	{
		float min = std::min(transformedVertex0[i], std::min(transformedVertex1[i], transformedVertex2[i]));
		float max = std::max(transformedVertex0[i], std::max(transformedVertex1[i], transformedVertex2[i]));
		if (min > this->halfDistances[i] || max < -this->halfDistances[i])
		{
			return false;
		}
	}

	//Perform tests with edge21 which project the triangle onto and axis and the box onto the axis
	glm::vec3 edge10abs = glm::abs(edge10);
	//X axis
	if (!axisTest(edge10.z, edge10.y, transformedVertex0.y, transformedVertex0.z, transformedVertex2.y, transformedVertex2.z, edge10abs.z, halfDistances.y, edge10abs.y, halfDistances.z)) return false;
	//Y axis
	if (!axisTest(edge10.x, edge10.z, transformedVertex0.z, transformedVertex0.x, transformedVertex2.z, transformedVertex2.x, edge10abs.z, halfDistances.x, edge10abs.x, halfDistances.z)) return false;
	//Z axis
	if (!axisTest(edge10.y, edge10.x, transformedVertex1.x, transformedVertex1.y, transformedVertex2.x, transformedVertex2.y, edge10abs.y, halfDistances.x, edge10abs.x, halfDistances.y)) return false;

	//Perform tests with edge32 which project the triangle onto and axis and the box onto the axis
	glm::vec3 edge21abs = glm::abs(edge21);
	//X axis
	if (!axisTest(edge21.z, edge21.y, transformedVertex0.y, transformedVertex0.z, transformedVertex2.y, transformedVertex2.z, edge21abs.z, halfDistances.y, edge21abs.y, halfDistances.z)) return false;
	//Y axis
	if (!axisTest(edge21.x, edge21.z, transformedVertex0.z, transformedVertex0.x, transformedVertex2.z, transformedVertex2.x, edge21abs.z, halfDistances.x, edge21abs.x, halfDistances.z)) return false;
	//Z axis
	if (!axisTest(edge21.y, edge21.x, transformedVertex0.x, transformedVertex0.y, transformedVertex1.x, transformedVertex1.y, edge21abs.y, halfDistances.x, edge21abs.x, halfDistances.y)) return false;
	
	//Perform tests with edge13 which project the triangle onto and axis and the box onto the axis
	glm::vec3 edge02abs = glm::abs(edge02);
	//X axis
	if (!axisTest(edge02.z, edge02.y, transformedVertex0.y, transformedVertex0.z, transformedVertex1.y, transformedVertex1.z, edge02abs.z, halfDistances.y, edge02abs.y, halfDistances.z)) return false;
	//Y axis
	if (!axisTest(edge02.x, edge02.z, transformedVertex0.z, transformedVertex0.x, transformedVertex1.z, transformedVertex1.x, edge02abs.z, halfDistances.x, edge02abs.x, halfDistances.z)) return false;
	//Z axis
	if (!axisTest(edge02.y, edge02.x, transformedVertex1.x, transformedVertex1.y, transformedVertex2.x, transformedVertex2.y, edge02abs.y, halfDistances.x, edge02abs.x, halfDistances.y)) return false;

	return true;
}

bool AABB::doesPlaneIntersect(glm::vec3 planeNormal, float planeConstant)
{
	float extent = glm::dot(this->halfDistances, glm::abs(planeNormal));
	//This is just the plane constant because the box was transformed to be at (0,0,0) and the dot of (0,0,0) with anything is zero
	float signedDistance = planeConstant;//glm::dot(this->center, planeNormal) + planeConstant;

	if (signedDistance - extent > 0) { return false; }
	if (signedDistance + extent < 0) { return false; }
	return true;
}

glm::vec3 AABB::getMinAsPoint()
{
	return center - halfDistances;
}

glm::vec3 AABB::getMaxAsPoint()
{
	return center + halfDistances;
}

glm::vec3 AABB::getCenter() const
{
	return center;
}

glm::vec3 AABB::getHalfDistances() const
{
	return halfDistances;
}

AABB * AABB::calculateBoundingBox(const std::vector<glm::vec3> & pointList)
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

bool AABB::axisTest(float edge1, float edge2, float v00, float v01, float v10, float v11, float absEdge0, float box0, float absEdge1, float box1)
{
	//Projection of the triangle onto the axis
	float max = edge1 * v00 - edge2 * v01;
	float min = edge1 * v10 - edge2 * v11;
	if (max < min)
	{
		std::swap(max, min);
	}

	//Projection of the box onto the axis
	float radius = absEdge0 * box0 + absEdge1 * box1;

	//If the min point of the projected triangle is greator than the radius or max point is less than the radius of the projected box, they do not overlap in that axis
	if (min > radius || max < -radius)
	{
		return false;
	}
	return true;
}

