#pragma once

#include <string>

struct Texture2D
{
	unsigned char* image;
	int width;
	int height;
	std::string path;
};

#include <vector>
#include <glm/vec3.hpp>

class ImageLoader
{
public:
	ImageLoader();

	int32_t loadTexture(const char* path);

	glm::vec3 getColorAtTextureUV(int32_t textureID, float u, float v);

private:
	std::vector<Texture2D> loadedTextures;
};