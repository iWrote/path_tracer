#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "path_tracer_utils.h"

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const RayHit& hitrec, Color& attenuation, Ray& scattered) const = 0;
};



class Lambertian : public Material
{
public:
	Lambertian(const Color& a)
		: albedo(a)
	{}

	virtual bool scatter(const Ray& r_in, const RayHit& hitrec, Color& attenuation, Ray& scattered) const override
	{
		Vector3 scatter_direction = hitrec.normal + random_unit_vector();
		scattered = Ray{ hitrec.p, scatter_direction };
		attenuation = albedo;
		return true;
	}

public:
	Color albedo;
};


class Metal : public Material
{
public:
	Metal(const Color& a, double f)
		:albedo(a), fuzz(f<1?f:1)
	{}

	virtual bool scatter(const Ray& r_in, const RayHit& hitrec, Color& attenuation, Ray& scattered) const override
	{
		Vector3 scatter_direction = reflect(unit_vector(r_in.direction()), hitrec.normal);
		scattered = Ray{ hitrec.p, scatter_direction + fuzz*random_in_unit_sphere() };
		attenuation = albedo;

		return hitrec.front_face;
	}


public:
	Color albedo;
	double fuzz;
};



#endif // !MATERIAL_H

