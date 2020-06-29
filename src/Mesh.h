#pragma once
#ifndef MESH_H
#define MESH_H

#include "path_tracer_utils.h"

class Material;

struct RayHit
{
	Point3 p;
	Vector3 normal;
	std::shared_ptr<Material> mat_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const Ray& r, const Vector3& outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Mesh
{
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, RayHit& hit) const = 0;
};

#endif // !MESH_H
