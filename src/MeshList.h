#pragma once
#ifndef MESHLIST_H
#define MESHLIST_H

#include <vector>
#include <memory> //https://en.cppreference.com/book/intro/smart_pointers
/*We'll use shared pointers in our code, because it allows multiple geometries to 
share a common instance (for example, a bunch of spheres that all use the same 
texture map material), and because it makes memory management automatic and easier 
to reason about.*/

#include "Mesh.h"

class MeshList : public Mesh
{
public:
	MeshList()
	{}

	MeshList(std::shared_ptr<Mesh> mesh)
	{
		add(mesh);
	}

	void clear()
	{
		meshes.clear();
	}

	void add(std::shared_ptr<Mesh> mesh)
	{
		meshes.push_back(mesh);
	}

	virtual bool hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;


public:
	std::vector<std::shared_ptr<Mesh>> meshes;
};


//returns RayHit for the closest mesh hit in list
bool MeshList::hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const
{
	RayHit temp_hitrec;
	bool hit_anything = false;	
	double closest_so_far = tmax;

	for (const std::shared_ptr<Mesh>& mesh : meshes)
	{
		if (mesh->hit(r, tmin, closest_so_far, temp_hitrec))
		{
			hit_anything = true;
			closest_so_far = temp_hitrec.t;
			hitrec = temp_hitrec;
		}
	}

	return hit_anything;


}

bool MeshList::bounding_box(double time0, double time1, AABB& output_box) const
{
	if (meshes.empty()) 
		return false;

	AABB temp_box;
	bool first_box = true;

	for (const auto& mesh : meshes)
	{
		//so don't mix mesh lists of stuff with AABBs and stuff without AABBs ryt
		if (!mesh->bounding_box(time0, time1, temp_box))
			return false;

		output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
		first_box = false;
	}
	return true;
}


#endif // !#MESHLIST_H
