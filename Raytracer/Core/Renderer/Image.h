#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

class Image
{
public:
	Image(std::string filename, uint32_t width, uint32_t height);
	~Image();

	void writeFramebufferToImage(std::vector<glm::vec3> framebuffer);

private:
	std::ofstream ofs;
	uint32_t width, height;
};