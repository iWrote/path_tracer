#pragma once
#ifndef CAMERA_H
#define CAMERA_H


#include "path_tracer_utils.h"

class Camera
{
public:
	Camera(
		Point3 lookfrom, 
		Point3 lookat,
		Vector3 vup,//global up
		double vfov,
		double aspect_ratio,
		double aperture,
		double focus_dist,
		double t0 = 0,
		double t1 = 0
	)
	{
		double theta = deg_to_rad(vfov);
		double h = std::tan(theta / 2);
		double viewport_height{ 2.0f * h };
		double viewport_width{ aspect_ratio * viewport_height };
		
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u; //need to multiply focus_dist here
		vertical = focus_dist * viewport_height * v; //to maintain correct vfov
		lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist*w;

		lens_radius = aperture / 2;
		time0 = t0;
		time1 = t1;
	}

	Ray get_ray(double s, double t) const 
	{
		Vector3 rd = lens_radius * random_in_unit_disk();
		Vector3 offset = u * rd.x() + v * rd.y();

		return Ray{ origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, random_double(time0, time1) };
		//notice offset added to origin and SUBTRACTED form target (imaging plane is at C a.k.a 2F a.k.a point of sharp imaging, no matter what the aperture is) 
		//and yep, that *plane of sharp imaging is "focus-dist" away from "origin"

	}

private:
	Point3 origin;
	Point3 lower_left_corner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 u, v, w;
	double lens_radius;
	double time0, time1; // cam "shutter" open/close times
};


#endif // !CAMERA_H
