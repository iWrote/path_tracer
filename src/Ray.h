#pragma once
#ifndef RAY_H
#define RAY_H


#include "Vector3.h"

class Ray
{
public:
	Ray(){}

	Ray(const Point3& r0, const Vector3& t)
		: o{r0}, d{t}
	{}

	Point3 origin() const { return o; }
	Vector3 direction() const { return d; }

	Point3 at(double t) const
	{
		return o + t * d;
	}
private:
	Point3 o;
	Vector3 d;
};

#endif // !RAY_H
