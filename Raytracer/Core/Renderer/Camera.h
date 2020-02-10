#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera
{
public:
	Camera(float aspect);
	Camera(glm::vec3 position, float y, float p, float fieldOfView, float aspect);

	glm::vec3 getOrigin();

	glm::vec3 convertCameraSpaceToWorldSpace(glm::vec3 cameraPoint);

	glm::mat4 getCameraSpaceToWorldSpace();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	glm::vec3 getFrontVector();

private:
	glm::vec3 origin;
	float yaw;
	float pitch;
	float fov;
	float aspectRatio;
};