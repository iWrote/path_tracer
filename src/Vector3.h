#pragma once

//Operator Overloading Guidelines: http://users.cms.caltech.edu/~donnie/cs11/cpp/cpp-ops.html

#include <cmath>
#include <iostream>

class Vector3 {
	
public:
	Vector3()
		: e{ 0,0,0 }
	{}
	Vector3(float e0, float e1, float e2)
		:e{ e0, e1, e2 }
	{
		//std::cout << "Vector3 constructed\n";
	}
	~Vector3()
	{
		//std::cout << "Vector3 destructed\n";
	}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }
	float r() const { return e[0]; }
	float g() const { return e[1]; }
	float b() const { return e[2]; }

	Vector3 operator -() const 
	{ 
		return Vector3(-e[0], -e[1], -e[2]); 
	}

	float operator [](int i) const
	{
		return e[i];
	}

	float& operator [](int i)
	{
		return e[i];
	}

	Vector3& operator += (const Vector3& v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
	}
	Vector3& operator -= (const Vector3& v)
	{
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
	}
	Vector3& operator *= (const float t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
	}
	Vector3& operator /= (const float t)
	{
		e[0] /= t;
		e[1] /= t;
		e[2] /= t;
	}
	float length() const
	{
		return std::sqrt(length_squared());
	}
	float length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	

public:
	float e[3];
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

inline Vector3 operator * (const Vector3& u, float t)
{
	return Vector3{ u.e[0] * t, u.e[1] * t,u.e[2] * t };
}

inline Vector3 operator * (float t, const Vector3& u)
{
	return u*t;
}

inline Vector3 operator / (const Vector3& u, float t)
{
	return Vector3{ u.e[0] / t, u.e[1] / t,u.e[2] / t };
}

inline float dot(const Vector3& u, const Vector3& v)
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

using Point3 = Vector3;
using Color = Vector3;