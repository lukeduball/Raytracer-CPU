#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float aspect) : Camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, 90.0f, aspect)
{
}

Camera::Camera(glm::vec3 position, float y, float p, float fieldOfView, float aspect) : origin(position), yaw(y), pitch(p), fov(fieldOfView), aspectRatio(aspect)
{
}

glm::vec3 Camera::getOrigin()
{
	return origin;
}

glm::vec3 Camera::convertCameraSpaceToWorldSpace(const glm::vec3 cameraPoint)
{
	return this->cameraToWorldSpaceMatrix * glm::vec4(cameraPoint, 1.0f);
}

void Camera::calculateCameraToWorldSpaceMatrix()
{
	//Computes the camera to world space matrix by taking the inverse of the world to camera space view matrix
	this->cameraToWorldSpaceMatrix = glm::inverse(getViewMatrix());
}

glm::mat4 Camera::getViewMatrix()
{
	//Computes the world to camera space matrix by looking from the cameras origin in the direction of its front facing vector
	return glm::lookAt(origin, origin + this->getFrontVector(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(fov, aspectRatio, 0.1f, 100.0f);
}

glm::vec3 Camera::getFrontVector()
{
	//Calculate the front facing vector using the yaw and pitch of the camera
	glm::vec3 front;
	front.x = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	front.y = sinf(glm::radians(pitch));
	front.z = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
	return glm::normalize(front);
}

float Camera::getFieldOfView()
{
	return fov;
}
