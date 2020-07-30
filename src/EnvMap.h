#pragma once
#ifndef ENVMAP_H
#define ENVMAP_H

#include "path_tracer_utils.h"

//https://coeleveld.com/skydomes-equirectangular-environment-textures-and-hdri/ 


//just doing skyspheres
class EnvMap
{

public:

	EnvMap(int w,  int h,  int c, unsigned char const* img, Vector3 vupdir)
		:height{h}, width{w}, channels{c}, image{img}, vup{vupdir}
	{

	}

	//takes directiontion to long-lat to UV texture coord assuming equirectangular projection
	Color get_color(const Vector3& ud) const //optimise? eats up many cycles
	{
		
		/*
		i fled to the city with so much discounted. . . 
		. . .	AAAHhh but i'm singing like a bird 'bout it now
		*/


		float longitude = atan2(ud.x(), ud.z()); //[-pi, pi]; 0 is z+
		
		
		float nodalAngle = atan((std::sqrt(ud.x() * ud.x() + ud.z() * ud.z()) / ud.y()));
		float latitude = nodalAngle > 0 ? nodalAngle : nodalAngle + pi; 
		
		//sky_sph texture coord
		int u = (int)(clamp ( longitude / (2*pi), -0.499, 0.499) * width) + width/2;
		int v = (int)(clamp(latitude / pi, 0, 0.99) * height);
		
		//std::cout << longitude << ", " << u << " | ";
		//std::cout << latitude << ", " << v << std::endl;		
		
			

		unsigned char const* pixelptr = image + (v * width * channels) + (u * channels);



		return read_rgb_color(pixelptr);
	}

	
private:
	int const height;
	int const width;
	int const channels;
	unsigned char const* const image;
	Vector3 vup; //to orient the skyshpere, NOT USING RN, ASSUMES vup = y+

	

};

#endif // !ENVMAP_H
