#pragma once
#ifndef COLOR_H
#define COLOR_H


#include <iostream>

#include "Vector3.h"

void write_rgb_color(unsigned char*& img, Color pixel_color, unsigned int samples_per_pixel)
{
	double scale = 1.0 / samples_per_pixel;
	pixel_color *= scale;
	pixel_color = sqrt(pixel_color); // GAMMA CORRECTION https://graphics.stanford.edu/gamma.html
	pixel_color = clamp(pixel_color, 0, 0.999);
	pixel_color *= 256;

	//std::cout << pixel_color << std::endl;

	//takes a pointer to pixel, writes color values
	*img = static_cast<unsigned int>(pixel_color.r()); 
	*(img + 1) = static_cast<unsigned int>(pixel_color.g());
	*(img + 2) = static_cast<unsigned int>(pixel_color.b());
	img += 3;
}


#endif // !COLOR_H