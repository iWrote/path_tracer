#pragma once
#ifndef SPHERE_H
#define SPHERE_H


#include "Mesh.h"
#include "Vector3.h"

class Sphere : public Mesh
{
public:
	Sphere()
	{}

	Sphere(Point3 cen, double r)
		:center(cen), radius(r)
	{}

	virtual bool hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const;

public:
	Point3 center;
	double radius;
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
			return true;
		}

		tmp = (-half_b + root) / a;
		if (tmp < tmax && tmp > tmin)
		{
			hitrec.t = tmp;
			hitrec.p = r.at(tmp);
			hitrec.set_face_normal(r, (hitrec.p - center) / radius); //passing in "outward normal"
			return true;
		}
	}//if hit

	return false;
		
}

#endif // !SPHERE_H
