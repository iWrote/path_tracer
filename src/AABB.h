#pragma once
#ifndef AABB_H
#define AABB_H

#include "path_tracer_utils.h"

class AABB
{
public:
	AABB(){}

	AABB(const Point3& a, const Point3& b)
		:minimum(a), maximum(b)
	{}

	Point3 min() const { return minimum; }
	Point3 max() const { return maximum; }

	bool hit(const Ray& r, double t_min, double t_max) const
	{
		for (int a = 0; a < 3; a++)
		{
			double t0 = fmin((minimum[a] - r.origin()[a]) / r.direction()[a], (maximum[a] - r.origin()[a]) / r.direction()[a]);
			double t1 = fmax((minimum[a] - r.origin()[a]) / r.direction()[a], (maximum[a] - r.origin()[a]) / r.direction()[a]);

			t_min = fmax(t0, t_min);
			t_max = fmin(t1, t_max);
			if (t_max <= t_min)
				return false;
		}

		return true;
	}

public:
	Point3 maximum;
	Point3 minimum;



};

AABB surrounding_box(AABB box0, AABB box1)
{
	Point3 small(fmin(box0.min().x(), box1.min().x()),
		fmin(box0.min().y(), box1.min().y()),
		fmin(box0.min().z(), box1.min().z()));

	Point3 big(fmax(box0.max().x(), box1.max().x()),
		fmax(box0.max().y(), box1.max().y()),
		fmax(box0.max().z(), box1.max().z()));

	return AABB(small, big);
}

#endif