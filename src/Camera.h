#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "path_tracer_utils.h"

class Camera
{
public:
	Camera()
	{
		const double aspect_ratio = 16.0f / 9.0f;

		double viewport_height{ 2.0f };
		double viewport_width{ aspect_ratio * viewport_height };
		double focal_length{ 1.0f };

		origin = Point3( 0,0,0 );
		horizontal = Vector3( viewport_width, 0, 0 );
		vertical = Vector3( 0,viewport_height, 0 );
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3{ 0,0,focal_length };
				
	}

	Ray get_ray(double u, double v) const
	{
		return Ray{ origin, lower_left_corner + u * horizontal + v * vertical - origin };

	}

private:
	Point3 origin;
	Point3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
};


#endif // !CAMERA_H
