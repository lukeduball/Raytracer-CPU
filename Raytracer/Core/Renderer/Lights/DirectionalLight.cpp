#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 c, float i) : direction(dir), Light(c, i) {}
