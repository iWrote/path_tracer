#pragma once
#ifndef ENVMAP_H
#define ENVMAP_H

#include "path_tracer_utils.h"

//just doing skyspheres
class EnvMap
{

public:

	EnvMap(int w,  int h,  int c, unsigned char const* img, Vector3 vupdir)
		:height{h}, width{w}, channels{c}, image{img}, vup{vupdir}
	{

	}

	Color get_color(const Vector3& u) const //optimise? eats up many cycles
	{
		float phi = atan2(u.y(), u.x());
		float theta = atan(std::sqrt(u.x() * u.x() + u.y() * u.y()) / u.z()) + pi/2;

		int p = (int)(clamp(phi / pi, -0.99, 0.99)/2 * width);
		int q = (int)(clamp(theta / pi, 0, 0.99) * height);

		int x = width / 2 + p;
		int y = q;

		
		unsigned char const* pixelptr = image + (y * width * channels) + (x*channels);
		return read_rgb_color(pixelptr);
	}

	
private:
	int const height;
	int const width;
	int const channels;
	unsigned char const* const image;
	Vector3 vup; //to orient the skyshpere

	

};

#endif // !ENVMAP_H
