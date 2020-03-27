#include "ImageLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

ImageLoader::ImageLoader()
{
	//Load the missing texture image to first to occupy index 0 o the loadedTextures list
	loadTexture("Resources/Textures/missing_texture.png");
}

int32_t ImageLoader::loadTexture(const char * path)
{
	int width, height;

	unsigned char* image = stbi_load(path, &width, &height, 0, STBI_rgb_alpha);

	if (image == nullptr)
	{
		std::cout << "WARNING: Could not find image at path: " << path << std::endl;
		//this returns the image id for the missing texture image located at loadedTextures[0]
		return 0;
	}

	int32_t textureID = this->loadedTextures.size();
	Texture2D texture;
	texture.image = image;
	texture.width = width;
	texture.height = height;
	this->loadedTextures.push_back(texture);

	return textureID;
}

glm::vec3 ImageLoader::getColorAtTextureUV(int32_t textureID, float u, float v)
{
	Texture2D texture = this->loadedTextures[textureID];

	int pixelX = (int)(u * texture.width);
	int pixelY = (int)(v * texture.height);

	unsigned bytePerPixel = STBI_rgb_alpha;
	unsigned char * pixelOffset = texture.image + (pixelX + texture.width * pixelY) * bytePerPixel;
	unsigned char r = pixelOffset[0];
	unsigned char g = pixelOffset[1];
	unsigned char b = pixelOffset[2];
	unsigned char a = pixelOffset[3];

	return glm::vec3(r, g, b);
}
