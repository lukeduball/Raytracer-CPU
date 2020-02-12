#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>

#include "../Objects/Sphere.h"
#include "../Renderer/Ray.h"
#include "../Renderer/Camera.h"
#include "../Math/MathFunctions.h"
#include "../Renderer/Image.h"
#include "../Renderer/Lights/DirectionalLight.h"

#define _USE_MATH_DEFINES
#include <math.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#include <limits>
const float infinity = std::numeric_limits<float>::max();

bool trace(const Ray & ray, std::vector<Object*> & objectList, float &nearestHitParameter, Object *& objectHit)
{
	nearestHitParameter = infinity;
	for (Object * object : objectList)
	{
		float rayParameter = infinity;
		//Checks to see if the ray and object intersect and if this hit is the closest hit
		if (object->intersect(ray, rayParameter) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < nearestHitParameter)
		{
			objectHit = object;
			nearestHitParameter = rayParameter;
		}
	}

	return objectHit != nullptr;
}

glm::vec3 getColorFromRaycast(const Ray & ray, std::vector<Object*> & objectList, std::vector<DirectionalLight*> & lightList)
{
	//Background color is specified, if there is no hit in the trace, the background color will be provided
	glm::vec3 hitColor = glm::vec3(0.0f, 0.0f, 0.0f);

	DirectionalLight* light = lightList[0];
	float nearestHitParameter;
	Object* nearestHit = nullptr;

	if (trace(ray, objectList, nearestHitParameter, nearestHit))
	{
		glm::vec3 intersectionPoint = ray.getOrigin() + (ray.getDirectionVector() * nearestHitParameter);
		glm::vec3 normal = nearestHit->getNormalData(intersectionPoint);
		glm::vec2 texCoords = nearestHit->getTextureCoordData(intersectionPoint, normal);

		glm::vec3 L = -light->getDirection();

		float t;
		Object* shadowHit = nullptr;
		bool inShadow = trace(Ray(intersectionPoint, L), objectList, t, shadowHit);
		if (!inShadow)
		{
			hitColor = nearestHit->getAlbedo() / (float)M_PI * light->getIntensity() * light->getColor() * std::max(0.0f, glm::dot(normal, L));
		}
	}

	return hitColor;
}

void render(Camera &camera, std::vector<Object*> &objectList, std::vector<DirectionalLight*> &lightList, std::vector<glm::vec3> &framebuffer)
{
	float scale = tan(MathFunctions::degreesToRadians(90.0f) * 0.5f);
	float aspectRatio = WIDTH / (float)HEIGHT;
	for (uint32_t y = 0; y < HEIGHT; y++)
	{
		for (uint32_t x = 0; x < WIDTH; x++)
		{
			float pX = (2.0f * (x + 0.5f) / (float)WIDTH - 1.0f)  * scale;
			float pY = (1.0f - 2.0f * (y + 0.5f) / (float)HEIGHT) * scale;
			glm::vec3 position = camera.convertCameraSpaceToWorldSpace(glm::vec3(pX, pY, -1));
			Ray ray = Ray(position, glm::normalize(position - camera.getOrigin()));
			framebuffer[x + WIDTH * y] = getColorFromRaycast(ray, objectList, lightList);
		}
	}
}

void cleanup(std::vector<Object*> objectList)
{
	for (uint32_t i = 0; i < objectList.size(); i++)
	{
		delete objectList[i];
	}
}

int main()
{
	Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 90, 0, 45, (float)WIDTH / (float)HEIGHT);

	std::vector<DirectionalLight*> lightList;
	lightList.push_back(new DirectionalLight(glm::vec3(-1, -1, -1), glm::vec3(1.0f, 1.0f, 1.0f), 2));

	std::vector<Object*> objectList;
	objectList.push_back(new Sphere(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
	objectList.push_back(new Sphere(glm::vec3(1.0f, 1.0f, -4.0f), 0.25f, glm::vec3(0.0f, 1.0f, 0.0f)));
	
	std::vector<glm::vec3> framebuffer(WIDTH*HEIGHT);

	render(camera, objectList, lightList, framebuffer);

	std::cout << "Writing Image!" << std::endl;
	Image image("./out.ppm", WIDTH, HEIGHT);
	image.writeFramebufferToImage(framebuffer);
	std::cout << "Image Ready!" << std::endl;

	cleanup(objectList);
	return 0;
}