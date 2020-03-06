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
#include "../Objects/Triangle.h"
#include "../Objects/Models/Mesh.h"
#include "../Objects/Models/Model.h"
#include "../Renderer/Materials/Material.h"

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
	Mesh planeMesh;
	planeMesh.vertices = {glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, 0.5f)};
	planeMesh.indices = { 0, 2, 1,
						  2, 0, 3 };

	Mesh halfBoxMesh;
	halfBoxMesh.vertices = { glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.5f, 0.0f, 0.5f),
							 glm::vec3(-0.5f, 1.0f, -0.5f), glm::vec3(0.5f, 1.0f, -0.5f) };
	halfBoxMesh.indices = { 0, 2, 1,
							2, 0, 3,
							5, 4, 1,
							5, 1, 2};	

	//Defines the camera to be used in the scene, WIDTH / HEIGHT is the aspect ratio
	Camera camera(glm::vec3(0.0f, 2.5f, 2.0f), 0, 0, 45, (float)WIDTH / (float)HEIGHT);

	std::vector<Light*> lightList;
	lightList.push_back(new PointLight(glm::vec3(0.0f, 2.0f, -4.0f), glm::vec3(1.0f, 1.0f, 1.0f), 50));
	lightList.push_back(new DirectionalLight(glm::vec3(1, -1, -1), glm::vec3(1, 1, 1), 2));

	std::vector<Object*> objectList;
	Material whiteDiffuse = Material(Material::Type::DIFFUSE, glm::vec3(1.0f, 1.0f, 1.0f));
	Material orangeDiffuse = Material(Material::Type::DIFFUSE, glm::vec3(1.0f, 0.5f, 0.0f));
	Material greenDiffuse = Material(Material::Type::DIFFUSE, glm::vec3(0.0f, 1.0f, 0.0f));
	Material reflect = Material(Material::Type::REFLECT, glm::vec3(1.0f, 1.0f, 1.0f));
	objectList.push_back(new Sphere(glm::vec3(0.0f, 1.0f, -7.0f), 1.0f, &whiteDiffuse));
	//objectList.push_back(new Triangle(glm::vec3(-1.5f, 0.5f, -5.0f), glm::vec3(0.5f, 1.5f, -7.0f), glm::vec3(0.0f, 2.5f, -7.0f), &orangeDiffuse));
	objectList.push_back(new Sphere(glm::vec3(1.0f, 2.0f, -6.0f), 0.35f,  &greenDiffuse));
	objectList.push_back(new Sphere(glm::vec3(-1.0f, 2.0f, -6.0f), 0.25f, &greenDiffuse));
	objectList.push_back(new Sphere(glm::vec3(1.0f, 0.0f, -6.0f), 0.25f,  &greenDiffuse));
	objectList.push_back(new Sphere(glm::vec3(-1.0f, 0.0f, -6.0f), 0.25f, &greenDiffuse));
	objectList.push_back(new Model(glm::vec3(0.0f, 0.0f, -6.0f), 10.0f, &halfBoxMesh, &reflect));
	//objectList.push_back(new Model(glm::vec3(0.0f, 2.0f, -7.0f), 2.0f, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f), "Resources/Models/monkey.obj"));
	
	//Initializes the raytracer renderer
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