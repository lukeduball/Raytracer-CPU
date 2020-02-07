#include <iostream>
#include <fstream>
#include <vector>
#include <glm/vec3.hpp>

#include "../Objects/Sphere.h"
#include "../Renderer/Ray.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;

#include <limits>
const float infinity = std::numeric_limits<float>::max();

bool trace(const Ray & ray, std::vector<Object*> & objectList, float &nearestHitParameter, Object &objectHit)
{
	nearestHitParameter = infinity;
	for (Object * object : objectList)
	{
		float rayParameter = infinity;
		if(object->intersect(ray, rayParameter))
	}
}

int main()
{
	Ray ray = Ray(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	Sphere sphere = Sphere(glm::vec3(0.0f, 0.0f, 0.0f), 2);
	IntersectionResult result = sphere.intersect(ray);
	std::cout << result.doesIntersect << " (X:" << result.intersectionPoint.x << " Y:" << result.intersectionPoint.y << " Z:" << result.intersectionPoint.z << ")" << std::endl;

	std::vector<glm::vec3> framebuffer;

	std::ofstream ofs("./out.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
	for (uint32_t i = 0; i < WIDTH * HEIGHT; i++)
	{
		float color = i / (float)(WIDTH * HEIGHT);
		char r = (char)(255 * color);
		char g = (char)(255 * color);
		char b = (char)(255 * color);
		ofs << r << g << b;
	}

	ofs.close();
	return 0;
}