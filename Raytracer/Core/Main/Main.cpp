#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>

#include "../Objects/Sphere.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Image.h"
#include "../Renderer/Lights/DirectionalLight.h"
#include "../Renderer/Lights/PointLight.h"
#include "../Renderer/Renderer.h"

#define _USE_MATH_DEFINES
#include <math.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

void cleanup(std::vector<Object*> & objectList, std::vector<Light*> & lightList)
{
	for (uint32_t i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
	}

	for (uint32_t i = 0; i < lightList.size(); i++)
	{
		delete lightList[i];
	}
}

int main()
{
	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 0, 0, 45, (float)WIDTH / (float)HEIGHT);

	std::vector<Light*> lightList;
	lightList.push_back(new PointLight(glm::vec3(2, 2, -5), glm::vec3(1.0f, 1.0f, 1.0f), 50));
	lightList.push_back(new PointLight(glm::vec3(-2, 2, -5), glm::vec3(1.0f, 1.0f, 1.0f), 50));
	lightList.push_back(new PointLight(glm::vec3(2, -2, -5), glm::vec3(1.0f, 1.0f, 1.0f), 50));
	lightList.push_back(new PointLight(glm::vec3(-2, -2, -5), glm::vec3(1.0f, 1.0f, 1.0f), 50));

	std::vector<Object*> objectList;
	objectList.push_back(new Sphere(glm::vec3(0.0f, 0.0f, -7.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
	objectList.push_back(new Sphere(glm::vec3(1.0f, 1.0f, -6.0f), 0.35f, glm::vec3(0.0f, 1.0f, 0.0f)));
	objectList.push_back(new Sphere(glm::vec3(-1.0f, 1.0f, -6.0f), 0.25f, glm::vec3(0.0f, 1.0f, 0.0f)));
	objectList.push_back(new Sphere(glm::vec3(1.0f, -1.0f, -6.0f), 0.25f, glm::vec3(0.0f, 1.0f, 0.0f)));
	objectList.push_back(new Sphere(glm::vec3(-1.0f, -1.0f, -6.0f), 0.25f, glm::vec3(0.0f, 1.0f, 0.0f)));
	
	Renderer renderer(WIDTH, HEIGHT);

	std::cout << "Start raytracing scene!" << std::endl;
	renderer.render(camera, objectList, lightList);
	std::cout << "Raytracing finished!" << std::endl;

	std::cout << "Writing Image!" << std::endl;
	Image image("./out.ppm", WIDTH, HEIGHT);
	image.writeFramebufferToImage(renderer.getFramebuffer());
	std::cout << "Image Ready!" << std::endl;

	cleanup(objectList, lightList);
	return 0;
}