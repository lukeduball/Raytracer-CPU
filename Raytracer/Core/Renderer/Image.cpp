#include "Image.h"

#include "../Math/MathFunctions.h"

Image::Image(std::string filename, uint32_t width, uint32_t height) : ofs(std::ofstream(filename, std::ios::out | std::ios::binary)), width(width), height(height)
{
	//Write the header data for .PPM file, specify width and height in pixels along with size for a color component
	ofs << "P6\n" << width << " " << height << "\n255\n";
}

Image::~Image()
{
	ofs.close();
}

void Image::writeFramebufferToImage(const std::vector<glm::vec3> & framebuffer)
{
	//Loop through all pixels of file
	for (uint32_t i = 0; i < width * height; i++)
	{
		glm::vec3 color = framebuffer[i];
		//Clamp functions make sure the values are between 0 and 1 so that the image is not distrupted with larger values
		char r = (char)(255 * MathFunctions::clamp(0.0f, 1.0f, color.x));
		char g = (char)(255 * MathFunctions::clamp(0.0f, 1.0f, color.y));
		char b = (char)(255 * MathFunctions::clamp(0.0f, 1.0f, color.z));
		//Write the r, g, and b values for a given pixel out to the file
		ofs << r << g << b;
	}
}
