#pragma once
#ifndef BVH_H
#define BVH_H

#include "path_tracer_utils.h"

class BVH_Node : public Mesh
{
public:

	BVH_Node();

	BVH_Node(const MeshList& list, double time0, double time1)
		:BVH_Node(list.meshes, 0, list.meshes.size(), time0, time1)
	{}

	BVH_Node(
		const std::vector<std::shared_ptr<Mesh>>& src_meshes,
		size_t start, size_t end, double time0, double time1);

	virtual bool hit(const Ray& r, double t_min, double t_max, RayHit& rec) const override;
	virtual bool bounding_box(double time0, double time1, AABB& output_box) const override;


public:
	std::shared_ptr<Mesh> left;
	std::shared_ptr<Mesh> right;
	AABB box;

};







bool BVH_Node::bounding_box(double time0, double time1, AABB& output_box) const
{
	output_box = box;
	return true;
}


bool BVH_Node::hit(const Ray& r, double t_min, double t_max, RayHit& hitrec) const
{
	if (!box.hit(r, t_min, t_max))
		false;
	
	bool hit_left = left->hit(r, t_min, t_max, hitrec);
	bool hit_right = right->hit(r, t_min, hit_left ? hitrec.t : t_max, hitrec); //in case one's behind the other, don't bother checking past it

	return hit_left || hit_right;
}

inline bool box_compare(const std::shared_ptr<Mesh> a, const std::shared_ptr<Mesh> b, int axis)
{
	AABB box_a;
	AABB box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor. \n";

	return box_a.min().e[axis] < box_b.min().e[axis];
}
bool box_x_compare(const std::shared_ptr<Mesh> a, const std::shared_ptr<Mesh> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<Mesh> a, const std::shared_ptr<Mesh> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<Mesh> a, const std::shared_ptr<Mesh> b) {
	return box_compare(a, b, 2);
}



BVH_Node::BVH_Node(const std::vector<std::shared_ptr<Mesh>>& src_meshes, size_t start, size_t end, double time0, double time1)
{
	std::vector<std::shared_ptr<Mesh>> meshes = src_meshes;

	int axis = random_int(0, 2);
	auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

	size_t object_span = end - start;

	if (object_span == 1)
	{
		left = right = meshes[start];
	}
	else if (object_span == 2)
	{
		if (comparator(meshes[start], meshes[start + 1])) {
			left = meshes[start];
			right = meshes[start + 1];
		}
		else {
			left = meshes[start + 1];
			right = meshes[start];
		}
	}
	else
	{
		std::sort(meshes.begin() + start, meshes.begin() + end, comparator);
		size_t mid = start + object_span / 2;
		left = std::make_shared<BVH_Node>(meshes, start, mid, time0, time1);
		right = std::make_shared<BVH_Node>(meshes, mid, end, time0, time1);
	}

	AABB box_left, box_right;

	if (!left->bounding_box(time0, time1, box_left)
		|| !right->bounding_box(time0, time1, box_right)
		)
		std::cerr << "No bounding box in bvh_node constructor.\n";

	box = surrounding_box(box_left, box_right);



}



#endif // !BVH_H

