#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headerLibs\stb_image_write.h"
#include "Vector3.h"
#include "Color.h"


int main()
{
	unsigned int const image_height{ 256 };
	unsigned int const image_width{ 256 };
	unsigned int const image_channels {3};
	unsigned char* const image = new unsigned char[image_height * image_width * image_channels];
	
	unsigned char* img = image;
	for (int j = 0; j < image_height; j++)
	{
		for (int i = 0; i < image_width; i++)
		{
			if (j < image_height / 2 && i < image_width / 2)
			{
				write_rgb_color(img, Color(0, 0, 0));
			}

			else if (j < image_height / 2 && i >= image_width / 2)
			{
				write_rgb_color(img, Color(255, 0, 0));

			}
			else if (j >= image_height / 2 && i < image_width / 2)
			{
				write_rgb_color(img, Color(0, 255, 0));

			}
			else
			{
				write_rgb_color(img, Color(0, 0, 255));

			}
		}
		std::cout << "\rScanlines remaining: " << (image_height - j - 1) << ' ' << std::flush;
	}
	std::cout << "\nDone.\n";
	stbi_write_png("output.png", image_width, image_height, image_channels, image, 0);
	delete[] image;
	std::cin.get();
	return 0;
}