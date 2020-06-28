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

	virtual bool hit(const Ray& r, double tmin, double tmax, RayHit& hitrec) const;

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

#endif // !#MESHLIST_H
