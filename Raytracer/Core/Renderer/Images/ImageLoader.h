#pragma once

struct Texture2D
{
	unsigned char* image;
	int width;
	int height;
};

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vector>
#include <glm/vec3.hpp>

class ImageLoader
{
public:
	uint32_t loadTexture(const char* path);

	glm::vec3 getColorAtTextureUV(uint32_t textureID, float u, float v);

private:
	std::vector<Texture2D> loadedTextures;
};