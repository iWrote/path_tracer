#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H


//Operator Overloading Guidelines: http://users.cms.caltech.edu/~donnie/cs11/cpp/cpp-ops.html
#include "path_tracer_utils.h"

class Vector3 {
	
public:
	Vector3()
		: e{ 0,0,0 }
	{}
	Vector3(double e0, double e1, double e2)
		:e{ e0, e1, e2 }
	{
		//std::cout << "Vector3 constructed\n";
	}
	~Vector3()
	{
		//std::cout << "Vector3 destructed\n";
	}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	double r() const { return e[0]; }
	double g() const { return e[1]; }
	double b() const { return e[2]; }

	Vector3 operator -() const 
	{ 
		return Vector3(-e[0], -e[1], -e[2]); 
	}

	double operator [](int i) const
	{
		return e[i];
	}

	double& operator [](int i)
	{
		return e[i];
	}

	Vector3& operator += (const Vector3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	Vector3& operator -= (const Vector3& v)
	{
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}
	Vector3& operator *= (const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	Vector3& operator /= (const double t)
	{
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
		return *this;
	}

	double length() const
	{
		return std::sqrt(length_squared());
	}
	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	inline static Vector3 random()
	{
		return Vector3{ random_double(), random_double(), random_double() };
	}

	inline static Vector3 random(double min, double max)
	{
		return Vector3{ random_double(min, max), random_double(min, max), random_double(min, max) };
	}


public:
	double e[3];
};



inline std::ostream& operator <<(std::ostream& out, const Vector3& v)
{
	return out << "Vector3(" << v.e[0] << ',' << v.e[1] << ',' << v.e[2] << ')';
}

inline Vector3 operator + (const Vector3& u, const Vector3& v)
{
	return Vector3{ u.e[0] + v.e[0], u.e[1] + v.e[1],u.e[2] + v.e[2] };
}

inline Vector3 operator - (const Vector3& u, const Vector3& v)
{
	return Vector3{ u.e[0] - v.e[0], u.e[1] - v.e[1],u.e[2] - v.e[2] };
}

inline Vector3 operator * (const Vector3& u, const Vector3& v) //etto . . .
{
	return Vector3{ u.e[0] * v.e[0], u.e[1] * v.e[1],u.e[2] * v.e[2] };
}

inline Vector3 operator * (const Vector3& u, double t)
{
	return Vector3{ u.e[0] * t, u.e[1] * t,u.e[2] * t };
}

inline Vector3 operator * (double t, const Vector3& u)
{
	return u*t;
}

inline Vector3 operator / (const Vector3& u, double t)
{
	return Vector3{ u.e[0] / t, u.e[1] / t,u.e[2] / t };
}

inline double dot(const Vector3& u, const Vector3& v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3& u, const Vector3& v) 
{
	return Vector3{ u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0] };
}

inline Vector3 unit_vector(const Vector3& u)
{
	return u / u.length();
}

inline Vector3 clamp(const Vector3& u, double min, double max)
{
	return Vector3{ clamp(u.e[0], min, max), clamp(u.e[1], min, max), clamp(u.e[2], min, max) };
}

inline Vector3 sqrt(const Vector3& u)
{
	return Vector3{ std::sqrt(u.e[0]), std::sqrt(u.e[1]) , std::sqrt(u.e[2]) };
}

Vector3 random_in_unit_sphere() //random pt. in cube, is in sphere? No? Retry. Faster, no division
{
	while (true)
	{
		Vector3 p = Vector3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

Vector3 random_unit_vector()
{
	double a = random_double(0, 2 * pi);
	double z = random_double(-1, 1);
	double r = std::sqrt(1 - z * z);
	return Vector3{ r * cos(a), r * sin(a), z };
}

Vector3 random_in_unit_hemisphere(const Vector3& normal)
{
	Vector3 r = random_in_unit_sphere();
	if (dot(r, normal) > 0.0)
		return r;
	else
		return -r;
}

Vector3 reflect(const Vector3& v, const Vector3& n)
{
	return v - 2 * dot(v, n) * n;
}

using Point3 = Vector3;
using Color = Vector3;

#endif // !VECTOR3_H
