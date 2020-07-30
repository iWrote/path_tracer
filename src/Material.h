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
		scattered = Ray{ hitrec.p, scatter_direction, r_in.time() };
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

		return (dot(scattered.direction(), hitrec.normal) > 0); 
	}


public:
	Color albedo;
	double fuzz;
};


double schlick(double cosine, double ref_idx)
{
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Dielectric : public Material
{
public:
	Dielectric(double ri)
		:ref_idx(ri)
	{}

	virtual bool scatter(const Ray& r_in, const RayHit& hitrec, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0); //no attenuation :/
		double etai_over_etat;
		if (hitrec.front_face)
		{
			etai_over_etat = 1.0 / ref_idx; //air outside, this'll glitch for glass in glass
		}
		else
		{
			etai_over_etat = ref_idx;
		}

		Vector3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, hitrec.normal), 1.0); //guard against floating point error?
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		//angle dependent chance of reflection (fresnel effect)
		double reflect_prob = schlick(cos_theta, etai_over_etat);

		if (etai_over_etat * sin_theta > 1.0 || random_double() < reflect_prob) //REFLECTING
		{
			Vector3 reflected = reflect(unit_direction, hitrec.normal);
			scattered = Ray(hitrec.p, reflected);
			return true;
		}


		Vector3 refracted = refract(unit_direction, hitrec.normal, etai_over_etat);
		scattered = Ray(hitrec.p, refracted);
		return true;
	}

public:
	double ref_idx;
};

class MetaballListMaterial : public Material
{
public:

	//temporary, no material blending
	virtual bool scatter(const Ray& r_in, const RayHit& hitrec, Color& attenuation, Ray& scattered) const
	{
		/*

		Vector3 scatter_direction = hitrec.normal + random_unit_vector();
		scattered = Ray{ hitrec.p, scatter_direction, r_in.time() };
		const Vector3& N = hitrec.normal;
		attenuation = 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
		return true;

		*/

		const Vector3& N = hitrec.normal;
		attenuation = 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
		double etai_over_etat;
		if (hitrec.front_face)
		{
			etai_over_etat = 1.0 / 1.5; //air outside, this'll glitch for glass in glass
		}
		else
		{
			etai_over_etat = 1.5;
		}

		Vector3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, hitrec.normal), 1.0); //guard against floating point error?
		double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

		//angle dependent chance of reflection (fresnel effect)
		double reflect_prob = schlick(cos_theta, etai_over_etat);

		if (etai_over_etat * sin_theta > 1.0 || random_double() < reflect_prob) //REFLECTING
		{
			Vector3 reflected = reflect(unit_direction, hitrec.normal);
			scattered = Ray(hitrec.p, reflected);
			return true;
		}


		Vector3 refracted = refract(unit_direction, hitrec.normal, etai_over_etat);
		scattered = Ray(hitrec.p, refracted);
		return true;


	}

public:
	//std::unique_ptr<std::vector<std::shared_ptr<Sphere>>> balls_ptr; //for material blending among metaballs
};


#endif // !MATERIAL_H

