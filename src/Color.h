#pragma once
#ifndef COLOR_H
#define COLOR_H


#include <iostream>

#include "Vector3.h"

void write_rgb_color(unsigned char*& pixelptr, Color pixel_color, unsigned int samples_per_pixel) //passing pixel ptr by ref 'cause i add to it 
{
	double scale = 1.0 / samples_per_pixel;
	pixel_color *= scale;
	pixel_color = sqrt(pixel_color); // GAMMA CORRECTION https://graphics.stanford.edu/gamma.html
	pixel_color = clamp(pixel_color, 0, 0.999);
	pixel_color *= 256;

	//std::cout << pixel_color << std::endl;

	//takes a pointer to pixel, writes color values
	*pixelptr = static_cast<unsigned int>(pixel_color.r()); 
	*(pixelptr + 1) = static_cast<unsigned int>(pixel_color.g());
	*(pixelptr + 2) = static_cast<unsigned int>(pixel_color.b());
	pixelptr += 3;
}


void write_rgb_color(int nx, int ny, unsigned char* pixelptr, Color pixel_color, unsigned int samples_per_pixel, int image_width)
{
	double scale = 1.0 / samples_per_pixel;
	pixel_color *= scale;
	pixel_color = sqrt(pixel_color);
	pixel_color = clamp(pixel_color, 0, 0.999);
	pixel_color *= 256;

	pixelptr = pixelptr + 3*nx;
	pixelptr = pixelptr + 3 * image_width * ny;

	*pixelptr = static_cast<unsigned int>(pixel_color.r());
	*(pixelptr + 1) = static_cast<unsigned int>(pixel_color.g());
	*(pixelptr + 2) = static_cast<unsigned int>(pixel_color.b());


}


Color read_rgb_color(const unsigned char* pixelptr)
{
	float r = *pixelptr / 255.0f;
	float g = *(pixelptr + 1) / 255.0f;
	float b = *(pixelptr + 2) / 255.0f;
	return Color(r, g, b); //if the image is gamma corrected i'm not doing anything about it for now; HDRIs for lighting are gamma 1.0 anyways
}

#endif // !COLOR_H