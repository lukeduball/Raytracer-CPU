#include <iostream>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <algorithm>
#include <chrono>

#include "../Geometry/Sphere.h"
#include "../Renderer/Camera.h"
#include "../Renderer/Image.h"
#include "../Renderer/Lights/DirectionalLight.h"
#include "../Renderer/Lights/PointLight.h"
#include "../Renderer/Renderer.h"
#include "../Objects/Models/Mesh.h"
#include "../Objects/Models/Model.h"
#include "../Objects/Entity.h"
#include "../Renderer/Materials/RefractiveMaterial.h"
#include "../Renderer/Materials/PhongMaterial.h"
#include "../Renderer/Materials/ReflectMaterial.h"

#define _USE_MATH_DEFINES
#include <math.h>

//Width and Height of the image in pixels
const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

//Deletes all the dynamically allocated memory in the object and light list
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
	//Initializes the raytracer renderer
	Renderer renderer(WIDTH, HEIGHT);

	//Defines the camera to be used in the scene, WIDTH / HEIGHT is the aspect ratio
	Camera camera(glm::vec3(0.0f, 2.5f, 2.0f), 0, 0, 45, (float)WIDTH / (float)HEIGHT);

	std::vector<Light*> lightList;
	//lightList.push_back(new PointLight(glm::vec3(0.0f, 2.0f, -4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 50));
	lightList.push_back(new DirectionalLight(glm::vec3(1, -1, -1), glm::vec3(1, 1, 1), 2.0f));
	lightList.push_back(new PointLight(glm::vec3(1.0f, 1.5f, -9.9f), glm::vec3(1, 1, 1), 50.0f));

	int32_t missing_texture = renderer.getImageLoader().loadTexture("bad_path");
	int32_t marble_texture = renderer.getImageLoader().loadTexture("Resources/Textures/marble_floor.png");
	int32_t trex_texture = renderer.getImageLoader().loadTexture("Resources/Textures/T-REX.png");

	std::vector<Object*> objectList;
	PhongMaterial whiteDiffuse = PhongMaterial(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, 0.0f, true);
	PhongMaterial orangeDiffuse = PhongMaterial(glm::vec3(1.0f, 0.5f, 0.0f), 1.0f, 0.0f, 0.0f);
	PhongMaterial greenDiffuse = PhongMaterial(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f, 0.0f);
	PhongMaterial missingTextureDiffuse = PhongMaterial(missing_texture, 0.5f, 0.5f, 50.0f);
	PhongMaterial marbleFloor = PhongMaterial(marble_texture, 1.0f, 0.0f, 0.0f);
	PhongMaterial tRex = PhongMaterial(trex_texture, 1.0f, 0.0f, 0.0f, true);
	ReflectMaterial reflect = ReflectMaterial();
	RefractiveMaterial water = RefractiveMaterial(1.3f);
	
	objectList.push_back(new Sphere(glm::vec3(-2.5f, 2.0f, -7.0f), 1.0f, &missingTextureDiffuse));
	

	Model strawModel = Model("Resources/Models/straw.obj");
	Entity * straw = new Entity(glm::vec3(0.0f, 1.0f, -7.0f), 1.0f, &strawModel, &greenDiffuse);
	straw->setRotation(0.0f, 0.0f, -45.0f);
	objectList.push_back(straw);

	Model cylinderModel = Model("Resources/Models/cylinder.obj");
	objectList.push_back(new Entity(glm::vec3(0.0f, 1.0f, -7.0f), 1.0f, &cylinderModel, &water));

	Model planeModel = Model("Resources/Models/plane.obj");
	objectList.push_back(new Entity(glm::vec3(0.0f, 0.0f, -6.0f), 10.0f, &planeModel, &marbleFloor));

	Entity * reflectPlaneEntity = new Entity(glm::vec3(1.0f, 1.5f, -10.0f), 3.0f, &planeModel, &reflect);
	reflectPlaneEntity->setRotation(0.0f, 45.0f, 90.0f);
	objectList.push_back(reflectPlaneEntity);

	Model sphereModel = Model("Resources/Models/uvsphere.obj");
	objectList.push_back(new Entity(glm::vec3(3.0f, 1.0f, -6.0f), 1.0f, &sphereModel, &whiteDiffuse));

	Model tRexModel = Model("Resources/Models/t-rex.obj");
	Entity * tRexEntity = new Entity(glm::vec3(0.0f, 1.0f, -8.0f), 1.0f, &tRexModel, &tRex);
	tRexEntity->setRotation(0.0f, 45.0f, 0.0f);
	objectList.push_back(tRexEntity);

	std::cout << "Start raytracing scene!" << std::endl;
	//Stores the clock time at the start of the rendering process
	auto startTime = std::chrono::high_resolution_clock::now();

	renderer.render(camera, objectList, lightList);

	std::cout << "Raytracing finished!" << std::endl;
	//Stores the clock time at the end of the rendering process
	auto endTime = std::chrono::high_resolution_clock::now();
	//Calculates the elapsed time in milliseconds that the rendering process took by subtracting the start time from the end time
	auto elapsedTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
	//Prints out the elapsed time in seconds to 2 decimal places
	printf("Completed Rendering in: %.2f sec\n", elapsedTime / 1000.0f);

	std::cout << "Writing Image!" << std::endl;
	Image image("./out.ppm", WIDTH, HEIGHT);
	image.writeFramebufferToImage(renderer.getFramebuffer());
	std::cout << "Image Ready!" << std::endl;

	cleanup(objectList, lightList);
	return 0;
}