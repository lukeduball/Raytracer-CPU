#include "Renderer.h"

#include <list>
#include <algorithm>
#include <iostream>

#include "Camera.h"
#include "Ray.h"
#include "Lights/Light.h"
#include "../Math/MathFunctions.h"
#include "Materials/Material.h"
#include "Materials/RefractiveMaterial.h"
#include "Materials/PhongMaterial.h"

const int Renderer::MAX_RAY_DEPTH = 4;

Renderer::Renderer(uint32_t w, uint32_t h) : width(w), height(h), imageLoader(ImageLoader())
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
	float inverseWidth = 1 / (float)width;
	float inverseHeight = 1 / (float)height;
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			//Calculate the pixel's position in camera space by calculating the location of the center of each pixel. Apply the aspect ratio to the x to get the correct scaling
			float pX = (1.0f - 2.0f * (x + 0.5f) * inverseWidth) * scale * aspectRatio;
			float pY = (1.0f - 2.0f * (y + 0.5f) * inverseHeight) * scale;
			//Convert the pixel position to world space placing the plane 1 unit in front of the camera
			glm::vec3 position = camera.convertCameraSpaceToWorldSpace(glm::vec3(pX, pY, 1));
			//Create the ray by calculating the direction the ray will be cast in by subtracting the origin of the camera
 			Ray ray = Ray(position, glm::normalize(position - camera.getOrigin()));
			//Populate the color of the framebuffer by casting the ray into the scene and checking for intersections
			framebuffer[x + width * y] = getColorFromRaycast(ray, objectList, lightList);
		}
	}
}

ImageLoader & Renderer::getImageLoader()
{
	return this->imageLoader;
}

std::vector<glm::vec3>& Renderer::getFramebuffer()
{
	return framebuffer;
}

glm::vec3 Renderer::getColorFromRaycast(const Ray & ray, std::vector<Object*>& objectList, std::vector<Light*>& lightList, const uint32_t & depth)
{
	//Background color which will be returned if the ray hits no objects or the depth limit is reached
	glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);

	//Background color is specified, if there is no hit in the trace, the background color will be provided
	glm::vec3 hitColor = glm::vec3(0.0f, 0.0f, 0.0f);
	
	//Return the background color if the current ray has cast more reflection rays than the max depth allows
	if (depth > MAX_RAY_DEPTH)
	{
		return backgroundColor;
	}

	//Value used in ray parameterization to calculate the intersection point
	float nearestHitParameter = MathFunctions::T_INFINITY;
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
		Material * material = nullptr;

		//Outputs the normal and texture coordinates for the object that was intersected
		nearestHit->getSurfaceData(intersectionPoint, intersectionData, normal, textureCoords, material);

		switch (material->getMaterialType())
		{
			case Material::Type::PHONG:
			{
				glm::vec3 colorAtIntersection = getObjectHitColor(textureCoords, material);
				PhongMaterial * phongMaterial = (PhongMaterial*)(material);
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
					bool inShadow = trace(Ray(intersectionPoint, -lightDirection, Ray::Type::SHADOW), objectList, t, shadowHit, tMaximum, shadowData);

					if (!inShadow)
					{
						//Calculate the diffuse component
						glm::vec3 diffuse = colorAtIntersection * attenuatedLight * std::max(0.0f, glm::dot(normal, -lightDirection));
						//colorAtIntersection / (float)M_PI * attenuatedLight * std::max(0.0f, glm::dot(normal, -lightDirection));

						glm::vec3 reflectionVector = getReflectionVector(lightDirection, normal);
						//Calculate the speclar component
						glm::vec3 specular = attenuatedLight * std::pow(std::max(0.0f, glm::dot(reflectionVector, -ray.getDirectionVector())), phongMaterial->getPowerComponent());

						hitColor += diffuse * phongMaterial->getDiffuseComponent() + specular * phongMaterial->getSpecularComponent();
					}
				}
			}
			break;
			case Material::Type::REFLECT:
			{
				glm::vec3 reflectionDir = getReflectionVector(ray.getDirectionVector(), normal);
				hitColor += 0.8f * getColorFromRaycast(Ray(intersectionPoint, reflectionDir), objectList, lightList, depth + 1);
				break;
			}
			case Material::Type::REFLECT_AND_REFRACT:
			{
				RefractiveMaterial * refractiveMaterial = (RefractiveMaterial*)(material);

				glm::vec3 reflectionColor(0.0f);
				glm::vec3 refractionColor(0.0f);
				float reflectionMix = computeFresnel(ray.getDirectionVector(), normal, refractiveMaterial->getIndexOfRefraction());
				//there is no total interal reflection
				if (reflectionMix < 1.0f)
				{
					glm::vec3 refractionDirection = getRefractionVector(ray.getDirectionVector(), normal, refractiveMaterial->getIndexOfRefraction());
					refractionColor = getColorFromRaycast(Ray(intersectionPoint, refractionDirection), objectList, lightList, depth + 1);
				}

				glm::vec3 reflectionDirection = getReflectionVector(ray.getDirectionVector(), normal);
				reflectionColor = getColorFromRaycast(Ray(intersectionPoint, reflectionDirection), objectList, lightList, depth + 1);

				//Find a mix of the reflection and refraction with a linear interpolation
				hitColor += reflectionColor * reflectionMix + refractionColor * (1 - reflectionMix);
				break;
			}
		}
		return hitColor;
	}
	else
	{
		return backgroundColor;
	}
}

bool Renderer::trace(const Ray & ray, std::vector<Object*>& objectList, float & nearestHitParameter, Object *& objectHit, float upperBound, IntersectionData & intersectionData)
{
	std::list<IntersectionPair> possibleIntersectionList;
	//Loop through all objects to check for possible intersections with the ray
	for (Object * object : objectList)
	{
		//TODO Change this line to work with per-face materials
		//Allows shadow rays to disregard reflect and refract materials so shadows are not cast when the object should be transparent
		if (ray.getRayType() == Ray::Type::SHADOW && object->getMaterial()->getMaterialType() == Material::Type::REFLECT_AND_REFRACT)
		{
			continue;
		}

		float t = MathFunctions::T_INFINITY;
		if (object->possibleIntersection(ray, t) && t < upperBound)
		{
			//Add the intersection pair with its ray parameter to the list
			IntersectionPair pair;
			pair.objectHit = object;
			pair.parameter = t;
			possibleIntersectionList.push_back(pair);
		}
	}
	//Sort the possible intersections by the closest ray parameter
	possibleIntersectionList.sort();

	while (!possibleIntersectionList.empty())
	{
		nearestHitParameter = MathFunctions::T_INFINITY;
		//Need to pass a temporary intersection data struct so that intersection data does not get overwritten when an intersection is not the closest intersection point
		IntersectionData tempData;
		if (possibleIntersectionList.front().objectHit->intersect(ray, nearestHitParameter, tempData) && !ARE_FLOATS_EQUAL(nearestHitParameter, 0.0f) && nearestHitParameter < upperBound)
		{
			objectHit = possibleIntersectionList.front().objectHit;
			intersectionData = tempData;
			return true;
		}
		possibleIntersectionList.pop_front();
	}

	return false;
}

glm::vec3 Renderer::getObjectHitColor(const glm::vec2 & textureCoords, const Material * material)
{
	if (material->getTextureID() < 0)
	{
		return material->getAlbedo();
	}

	return imageLoader.getColorAtTextureUV(material->getTextureID(), textureCoords.x, textureCoords.y);
}

glm::vec3 Renderer::getReflectionVector(const glm::vec3 incidentDirection, const glm::vec3 normal)
{
	return glm::normalize(incidentDirection - 2 * glm::dot(incidentDirection, normal) * normal);
}

glm::vec3 Renderer::getRefractionVector(const glm::vec3 incidentDirection, const glm::vec3 normal, const float indexOfRefraction)
{
	glm::vec3 refractionNormal = normal;
	//Find the cos of the normal and incident direction
	float incidentNormalCosine = MathFunctions::clamp(-1.0f, 1.0f, glm::dot(incidentDirection, normal));
	//Value of 1 indicates the index of refraction for air, index for the material the ray is passing from
	float currentMediumIndexOfRefraction = 1;
	//Index of refraction for the material the ray is passing into
	float nextMediumIndexOfRefraction = indexOfRefraction;

	//Outside of the surface so we need to make the normalIncidentDot positive
	if (incidentNormalCosine < 0)
	{
		incidentNormalCosine = -incidentNormalCosine;
	}
	//Inside of the surface, need to reverse the normal and indices of refraction
	else
	{
		refractionNormal = -refractionNormal;
		//swaps the two values to change which material the ray is coming from and to
		std::swap(currentMediumIndexOfRefraction, nextMediumIndexOfRefraction);
	}

	//ratio of the indices of refraction described by Snell's Law
	float indicesOfRefractionRatio = currentMediumIndexOfRefraction / nextMediumIndexOfRefraction;
	//this value is calculated to indicate if the critical angle has been reached and there is no refraction
	float criticalValue = 1 - indicesOfRefractionRatio * indicesOfRefractionRatio * (1 - incidentNormalCosine * incidentNormalCosine);
	//Don't need to check if criticalValue < 0 because this is checked by the fresnel equation
	
	//Calculates teh direction of the refracted ray
	return glm::normalize(indicesOfRefractionRatio * incidentDirection + (indicesOfRefractionRatio * incidentNormalCosine - sqrt(criticalValue)) * refractionNormal);
}

float Renderer::computeFresnel(const glm::vec3 incidentDirection, const glm::vec3 normal, const float indiceOfRefraction)
{
	//Calculate the cosine between the incident ray and the normal vector
	float incidentCosine = MathFunctions::clamp(-1.0f, 1.0f, glm::dot(incidentDirection, normal));
	//the current mediums index of refraction, air has a value of 1
	float currentMediumIndexOfRefraction = 1;
	//the index of refraction for the medium the ray is passing into
	float nextMediumIndexOfRefraction = indiceOfRefraction;

	//the ray is starting inside the surface so the medium being traveled to and from need to be swapped
	if (incidentCosine > 0)
	{
		std::swap(currentMediumIndexOfRefraction, nextMediumIndexOfRefraction);
	}

	//sine of the angle of refraction using Snell's law
	float sineAngleOfRefraction = (currentMediumIndexOfRefraction / nextMediumIndexOfRefraction) * sqrtf(std::max(0.0f, 1 - incidentCosine * incidentCosine));
	//sine is greater than zero indicates their is a total internal reflection
	if (sineAngleOfRefraction >= 1.0f)
	{
		return 1.0f;
	}
	
	//cosine of the angle of refraction
	float cosineAngleOfRefraction = sqrtf(std::max(0.0f, 1 - sineAngleOfRefraction * sineAngleOfRefraction));
	//ensures that the incident cosine is a positive value
	incidentCosine = fabsf(incidentCosine);
	//calculates the parallel fresnel equation
	float parallelFresnel = ((nextMediumIndexOfRefraction*incidentCosine) - (currentMediumIndexOfRefraction*cosineAngleOfRefraction)) / ((nextMediumIndexOfRefraction*incidentCosine) + (currentMediumIndexOfRefraction*cosineAngleOfRefraction));
	//calculates the perpendicular fresnel equation
	float perpendicularFresnel = ((nextMediumIndexOfRefraction*cosineAngleOfRefraction) - (currentMediumIndexOfRefraction*incidentCosine)) / ((nextMediumIndexOfRefraction*cosineAngleOfRefraction) + (currentMediumIndexOfRefraction*incidentCosine));
	//returns the average of the two equations
	return (parallelFresnel * parallelFresnel + perpendicularFresnel * perpendicularFresnel) * 0.5f;
}
