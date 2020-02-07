#pragma once

#include <glm/vec3.hpp>

class Object
{
public:
	virtual bool intersect(const Ray & ray, float & parameter) = 0;

protected:
	Object(glm::vec3 pos);
	glm::vec3 position;

private:
};