#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "../Objects/Object.h"

class Camera;
class Light;
class Ray;

class Renderer
{
public:
	static const int MAX_RAY_DEPTH;

	Renderer(uint32_t w, uint32_t h);
	void render(Camera &camera, std::vector<Object*> &objectList, std::vector<Light*> &lightList);

	std::vector<glm::vec3> & getFramebuffer();

private:
	std::vector<glm::vec3> framebuffer;
	uint32_t width;
	uint32_t height;

	glm::vec3 getColorFromRaycast(const Ray & ray, std::vector<Object*> & objectList, std::vector<Light*> & lightList, const uint32_t & depth = 0);
	bool trace(const Ray & ray, std::vector<Object*> & objectList, float &nearestHitParameter, Object *& objectHit, float upperBound, IntersectionData & intersectionData);
	glm::vec3 getReflectionVector(const glm::vec3 incidentDirection, const glm::vec3 normal);
};