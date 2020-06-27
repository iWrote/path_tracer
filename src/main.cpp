#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main()
{
	const unsigned int image_height = 256;
	const unsigned int image_width = 256;
	const unsigned int image_channels = 3;
	unsigned char* const image = new unsigned char[image_height * image_width * image_channels];


	unsigned char* img = image;
	for (int j = 0; j < image_height; j++)
	{
		for (int i = 0; i < image_width; i++)
		{
			if (j < image_height / 2 && i < image_width / 2)
			{
				*img = 0; *(img + 1) = 0; *(img + 2) = 0;
				img += image_channels;
			}

			else if (j >= image_height / 2 && i < image_width / 2)
			{
				*img = 0; *(img + 1) = 0; *(img + 2) = 255;
				img += image_channels;
			}
			else if (j < image_height / 2 && i >= image_width / 2)
			{
				*img = 0; *(img + 1) = 255; *(img + 2) = 0;
				img += image_channels;
			}
			else
			{
				*img = 255; *(img + 1) = 0; *(img + 2) = 255;
				img += image_channels;
			}
		}
		std::cout << "\rScanlines remaining: " << (image_height - j - 1) << ' ' << std::flush;
	}
	std::cout << "\nDone.\n";
	stbi_write_png("output.png", image_width, image_height, image_channels, image, 0);
	delete[] image;
	std::cin.get();
}