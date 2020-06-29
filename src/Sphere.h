#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "path_tracer_utils.h"

class Sphere : public Mesh
{
public:
	Sphere()
	{}

	Sphere(Point3 cen, double r, std::shared_ptr<Material> m)
		:center(cen), radius(r), mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const;

public:
	Point3 center;
	double radius;
	std::shared_ptr<Material> mat_ptr;
};

bool Sphere::hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const
{
	Vector3 oc = r.origin() - center;
	double a = r.direction().length_squared();
	double half_b = dot(oc, r.direction());
	double c = oc.length_squared() - radius * radius;
	double inroot = half_b * half_b - a * c;
	if (inroot > 0)
	{
		double root = std::sqrt(inroot);

		double tmp = (-half_b - root) / a;
		if (tmp < tmax && tmp > tmin)
		{
			hitrec.t = tmp;
			hitrec.p = r.at(tmp);
			hitrec.set_face_normal(r, (hitrec.p - center) / radius); //passing in "outward normal"
			hitrec.mat_ptr = mat_ptr;
			return true;
		}

		tmp = (-half_b + root) / a;
		if (tmp < tmax && tmp > tmin)
		{
			hitrec.t = tmp;
			hitrec.p = r.at(tmp);
			hitrec.set_face_normal(r, (hitrec.p - center) / radius); //passing in "outward normal"
			hitrec.mat_ptr = mat_ptr;
			return true;
		}
	}//if hit

	return false;
		
}

#endif // !SPHERE_H
