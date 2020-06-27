#pragma once

#include <iostream>
#include "Vector3.h"

void write_rgb_color(unsigned char*& img, Color pixel_color)
{
	//takes a pointer to pixel, writes color values, increments pointer
	*img = static_cast<unsigned int>(pixel_color.r()); 
	*(img + 1) = static_cast<unsigned int>(pixel_color.g());
	*(img + 2) = static_cast<unsigned int>(pixel_color.b());
	img += 3;
}