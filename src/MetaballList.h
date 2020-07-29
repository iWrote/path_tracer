#pragma once
#ifndef MetaballList_H
#define MetaballList_H

#include <vector>
#include <memory> 

#include "path_tracer_utils.h"

//COULD INHERIT FROM MESHLIST AND ASSERT THAT ALL MESHES ARE SPHERES, repeating some code for now because Implicit Surfaces are different 


double GaussianFalloff(double d_squared, double r)  //Blobby, expensive, simple. Also called blinn falloff
{
	return exp(-r * d_squared);
}

double WyvilFalloff(double d_squared, double r)
{
	return pow((1 - d_squared / (r * r)), 3);
}

class MetaballList : public Mesh
{
public:

	MetaballList()
	{}

	MetaballList(double l, std::shared_ptr<MetaballListMaterial> m)
		:level(l), matlist_ptr(m)
	{
		
	}

	MetaballList(std::shared_ptr<Sphere> ball)
	{
		add(ball);
	}

	void clear()
	{
		balls.clear();
	}

	void add(std::shared_ptr<Sphere> ball)
	{
		balls.push_back(ball);
	}

	virtual bool hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const;

	
	double get_potential_at(const Point3& p) const
	{
		double potential = 0;
		double d_squared;
		for(auto ball : balls)
		{
			d_squared = (ball->center - p).length_squared();
			potential += GaussianFalloff(d_squared, ball->radius);

		}

		return potential;
	}

	Vector3 get_gradient_at(const Point3& p) const
	{
		double eps = 0.001;

		double gradFx = get_potential_at(p + xcap() * eps) - get_potential_at(p);
		double gradFy = get_potential_at(p + ycap() * eps) - get_potential_at(p);
		double gradFz = get_potential_at(p + zcap() * eps) - get_potential_at(p);

		return { gradFx, gradFy, gradFz };
	}

protected:
	std::vector<std::shared_ptr<Sphere>> balls;
	std::shared_ptr<MetaballListMaterial> matlist_ptr;
	double level;
	
};


bool MetaballList::hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const
{
	float step = 0.01;
	double eps = 0.01;
	double potential = 0;
	
	for (float t = tmin; t < tmax; t+=step)
	{

		potential = get_potential_at(r.at(t));
		//std::cout << "raymarching " << t << "/" << tmax << std::endl;
		//std::cout << potential << std::endl;
		if (abs(potential - level) < eps)
		{
			hitrec.t = t;
			hitrec.p = r.at(t);
			hitrec.set_face_normal(r, unit_vector(get_gradient_at(r.at(t))));
			
			hitrec.mat_ptr = matlist_ptr;
			return true;
		}
	}

	return false;


}



#endif // !#MetaballList_H
