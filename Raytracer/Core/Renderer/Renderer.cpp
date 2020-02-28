#include "Renderer.h"

#include <algorithm>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Camera.h"
#include "Ray.h"
#include "Lights/Light.h"
#include "../Math/MathFunctions.h"

Renderer::Renderer(uint32_t w, uint32_t h) : width(w), height(h)
{
	//Resize the framebuffer to the total amount of pixels
	framebuffer.resize(width * height);
}

void Renderer::render(Camera & camera, std::vector<Object*>& objectList, std::vector<Light*>& lightList)
{
	//Calculate the scale value by taking the tangent of the half angle of the cameras field of view
	float scale = tan(MathFunctions::degreesToRadians(camera.getFieldOfView()) * 0.5f);
	float aspectRatio = width / (float)height;
	//Calculate matrix ahead of raytracing to reduce time redoing the calculation each pixel during rendering
	camera.calculateCameraToWorldSpaceMatrix();
	//Loop through all pixels to send a ray from to render the scene
	int counter = 0;
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			//Calculate the pixel's position in camera space by calculating the location of the center of each pixel. Apply the aspect ratio to the x to get the correct scaling
			float pX = (1.0f - 2.0f * (x + 0.5f) / (float)width) * scale * aspectRatio;
			float pY = (1.0f - 2.0f * (y + 0.5f) / (float)height) * scale;
			//Convert the pixel position to world space placing the plane 1 unit in front of the camera
			glm::vec3 position = camera.convertCameraSpaceToWorldSpace(glm::vec3(pX, pY, 1));
			//Create the ray by calculating the direction the ray will be cast in by subracting the origin of the camera
			Ray ray = Ray(position, glm::normalize(position - camera.getOrigin()));
			//Populate the color of the framebuffer by casting the ray into the scene and checking for intersections
			framebuffer[x + width * y] = getColorFromRaycast(ray, objectList, lightList);
			//std::cout << "Percent Finsihed: " << (counter / (float)(width*height)) << std::endl;
			counter++;
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

	//Value used in ray parameterization to calculate the intersection point
	float nearestHitParameter;
	//The reference to the nearest intersected object when the ray is cast
	Object* nearestHit = nullptr;
	//Extra data passed back from the intersection object to be used in surface calculations
	IntersectionData intersectionData;

	//Finds if an object is intersected and outputs the nearest object, intersection data, and ray parameter value of the intersection
	if (trace(ray, objectList, nearestHitParameter, nearestHit, MathFunctions::T_INFINITY, intersectionData))
	{
		//Calculate the intersection point based on the ray parameter value
		glm::vec3 intersectionPoint = ray.getOrigin() + (ray.getDirectionVector() * nearestHitParameter);
		glm::vec3 normal;
		glm::vec2 textureCoords;
		//Outputs the normal and texture coordinates for the object that was intersected
		nearestHit->getSurfaceData(intersectionPoint, intersectionData, normal, textureCoords);

		//Loop through each light in the scene
		for (Light* light : lightList)
		{
			glm::vec3 lightDirection;
			glm::vec3 attenuatedLight;
			float tMaximum;
			//Output the direction of the light and light data to be used in shading. The tMaximum value is used to determine which t-value to throw away when casting a shadow ray
			light->getLightDirectionAndIntensity(intersectionPoint, lightDirection, attenuatedLight, tMaximum);

			float t;
			Object* shadowHit = nullptr;
			IntersectionData shadowData;
			//Check if the intersection point is in shadow by casting a shadow ray to the light source
			bool inShadow = trace(Ray(intersectionPoint, -lightDirection), objectList, t, shadowHit, tMaximum, shadowData);
			if (!inShadow)
			{
				//Contribute shading from the light source
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
		//Checks to see if the ray and object intersect and if this hit is the closest hit, ignore rayParameter that is zero because this indicates that the ray intersects with the same face its being cast from
		if (object->intersect(ray, rayParameter, intersectionData) && !ARE_FLOATS_EQUAL(rayParameter, 0.0f) && rayParameter < nearestHitParameter && rayParameter < upperBound)
		{
			objectHit = object;
			nearestHitParameter = rayParameter;
		}
	}

	return objectHit != nullptr;
}
