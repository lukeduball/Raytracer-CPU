#include "Renderer.h"

#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"
#include "Ray.h"
#include "Lights/Light.h"
#include "../Math/MathFunctions.h"

Renderer::Renderer(uint32_t w, uint32_t h) : width(w), height(h)
{
	framebuffer.resize(width * height);
}

void Renderer::render(Camera & camera, std::vector<Object*>& objectList, std::vector<Light*>& lightList)
{
	float scale = tan(MathFunctions::degreesToRadians(camera.getFieldOfView()) * 0.5f);
	float aspectRatio = width / (float)height;
	//Calculate matrix ahead of raytracing to reduce time redoing the calculation each pixel during rendering
	camera.calculateCameraToWorldSpaceMatrix();
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			float pX = (1.0f - 2.0f * (x + 0.5f) / (float)width) * scale * aspectRatio;
			float pY = (1.0f - 2.0f * (y + 0.5f) / (float)height) * scale;
			glm::vec3 position = camera.convertCameraSpaceToWorldSpace(glm::vec3(pX, pY, 1));
			Ray ray = Ray(position, glm::normalize(position - camera.getOrigin()));
			framebuffer[x + width * y] = getColorFromRaycast(ray, objectList, lightList);
		}
	}
}

std::vector<glm::vec3>& Renderer::getFramebuffer()
{
	return framebuffer;
}

glm::vec3 Renderer::getColorFromRaycast(const Ray & ray, std::vector<Object*>& objectList, std::vector<Light*>& lightList)
{
	//Background color is specified, if there is no hit in the trace, the background color will be provided
	glm::vec3 hitColor = glm::vec3(0.0f, 0.0f, 0.0f);

	float nearestHitParameter;
	Object* nearestHit = nullptr;
	IntersectionData intersectionData;

	if (trace(ray, objectList, nearestHitParameter, nearestHit, MathFunctions::T_INFINITY, intersectionData))
	{
		glm::vec3 intersectionPoint = ray.getOrigin() + (ray.getDirectionVector() * nearestHitParameter);
		glm::vec3 normal;
		glm::vec2 textureCoords;
		nearestHit->getSurfaceData(intersectionPoint, intersectionData, normal, textureCoords);

		for (Light* light : lightList)
		{
			glm::vec3 lightDirection;
			glm::vec3 attenuatedLight;
			float tMaximum;
			light->getLightDirectionAndIntensity(intersectionPoint, lightDirection, attenuatedLight, tMaximum);

			float t;
			Object* shadowHit = nullptr;
			IntersectionData shadowData;
			bool inShadow = trace(Ray(intersectionPoint, -lightDirection), objectList, t, shadowHit, tMaximum, shadowData);
			if (!inShadow)
			{
				hitColor += nearestHit->getAlbedo() / (float)M_PI * attenuatedLight * std::max(0.0f, glm::dot(normal, -lightDirection));
			}
		}
	}

	return hitColor;
}

bool Renderer::trace(const Ray & ray, std::vector<Object*>& objectList, float & nearestHitParameter, Object *& objectHit, float upperBound, IntersectionData & intersectionData)
{
	nearestHitParameter = MathFunctions::T_INFINITY;
	for (Object * object : objectList)
	{
		float rayParameter = MathFunctions::T_INFINITY;
		//Checks to see if the ray and object intersect and if this hit is the closest hit
		if (object->intersect(ray, rayParameter, intersectionData) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < nearestHitParameter && rayParameter < upperBound)
		{
			objectHit = object;
			nearestHitParameter = rayParameter;
		}
	}

	return objectHit != nullptr;
}
