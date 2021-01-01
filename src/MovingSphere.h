#pragma once
#ifndef MOVINGSPHERE_H
#define MOVINGSPHERE_H
#include "path_tracer_utils.h"

class MovingSphere : public Mesh
{
public:
	MovingSphere()
	{}

	MovingSphere(
		Point3 cen0, Point3 cen1,double t0, double t1, double r, std::shared_ptr<Material> m)
		: center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m)
	{}

	virtual bool hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;


	Point3 center(double time) const;


public:
	Point3 center0, center1;
	double time0, time1;
	double radius;
	std::shared_ptr<Material> mat_ptr;
};


Point3 MovingSphere::center(double time) const
{
	return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const 
{
	Vector3 oc = r.origin() - center(r.time());
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
			hitrec.set_face_normal(r, (hitrec.p - center(r.time())) / radius); //passing in "outward normal"
			hitrec.mat_ptr = mat_ptr;
			return true;
		}

		tmp = (-half_b + root) / a;
		if (tmp < tmax && tmp > tmin)
		{
			hitrec.t = tmp;
			hitrec.p = r.at(tmp);
			hitrec.set_face_normal(r, (hitrec.p - center(r.time())) / radius); //passing in "outward normal"
			hitrec.mat_ptr = mat_ptr;
			return true;
		}
	}//if hit

	return false;

}


bool MovingSphere::bounding_box(double _time0, double _time1, AABB& output_box) const 
{
	AABB box0(
		center(_time0) - Vector3(radius, radius, radius),
		center(_time0) + Vector3(radius, radius, radius));
	AABB box1(			 
		center(_time1) - Vector3(radius, radius, radius),
		center(_time1) + Vector3(radius, radius, radius));
	output_box = surrounding_box(box0, box1);
	return true;
	return true;
}




#endif // !MOVINGSPHERE_H
