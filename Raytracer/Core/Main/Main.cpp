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
#include "../Renderer/Lights/PointLight.h"

#define _USE_MATH_DEFINES
#include <math.h>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

#include <limits>
const float infinity = std::numeric_limits<float>::max();

bool trace(const Ray & ray, std::vector<Object*> & objectList, float &nearestHitParameter, Object *& objectHit, float upperBound)
{
	nearestHitParameter = infinity;
	for (Object * object : objectList)
	{
		float rayParameter = infinity;
		//Checks to see if the ray and object intersect and if this hit is the closest hit
		if (object->intersect(ray, rayParameter) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < nearestHitParameter && rayParameter < upperBound)
		{
			objectHit = object;
			nearestHitParameter = rayParameter;
		}
	}

	return objectHit != nullptr;
}

glm::vec3 getColorFromRaycast(const Ray & ray, std::vector<Object*> & objectList, std::vector<PointLight*> & lightList)
{
	//Background color is specified, if there is no hit in the trace, the background color will be provided
	glm::vec3 hitColor = glm::vec3(0.0f, 0.0f, 0.0f);

	PointLight* light = lightList[0];
	float nearestHitParameter;
	Object* nearestHit = nullptr;

	if (trace(ray, objectList, nearestHitParameter, nearestHit, infinity))
	{
		glm::vec3 intersectionPoint = ray.getOrigin() + (ray.getDirectionVector() * nearestHitParameter);
		glm::vec3 normal = nearestHit->getNormalData(intersectionPoint);
		glm::vec2 texCoords = nearestHit->getTextureCoordData(intersectionPoint, normal);

		glm::vec3 lightDirection;
		glm::vec3 attenuatedLight;
		float tMaximum;
		light->getLightDirectionAndIntensity(intersectionPoint, lightDirection, attenuatedLight, tMaximum);

		float t;
		Object* shadowHit = nullptr;
		bool inShadow = trace(Ray(intersectionPoint, -lightDirection), objectList, t, shadowHit, tMaximum);
		if (!inShadow)
		{
			hitColor = nearestHit->getAlbedo() / (float)M_PI * attenuatedLight * std::max(0.0f, glm::dot(normal, -lightDirection));
		}
	}

	return hitColor;
}

void render(Camera &camera, std::vector<Object*> &objectList, std::vector<PointLight*> &lightList, std::vector<glm::vec3> &framebuffer)
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

	std::vector<PointLight*> lightList;
	lightList.push_back(new PointLight(glm::vec3(2, 2, -3), glm::vec3(1.0f, 1.0f, 1.0f), 50));

	std::vector<Object*> objectList;
	objectList.push_back(new Sphere(glm::vec3(0.0f, 0.0f, -5.0f), 1.0f, glm::vec3(1.0f, 0.0f, 0.0f)));
	objectList.push_back(new Sphere(glm::vec3(1.0f, 1.0f, -4.0f), 0.25f, glm::vec3(0.0f, 1.0f, 0.0f)));
	
	std::vector<glm::vec3> framebuffer(WIDTH*HEIGHT);

	std::cout << "Start raytracing scene!" << std::endl;
	render(camera, objectList, lightList, framebuffer);
	std::cout << "Raytracing finished!" << std::endl;

	std::cout << "Writing Image!" << std::endl;
	Image image("./out.ppm", WIDTH, HEIGHT);
	image.writeFramebufferToImage(framebuffer);
	std::cout << "Image Ready!" << std::endl;

	cleanup(objectList);
	return 0;
}