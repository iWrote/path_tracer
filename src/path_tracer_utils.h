#pragma once
#ifndef PATH_TRACER_UTILS_H
#define PATH_TRACER_UTILS_H

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
//#include <random>




const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385f;



inline double deg_to_rad(double degrees) { return degrees * pi / 180.0; }
inline double rad_to_deg(double radians) { return radians / pi * 180.0; }
inline double random_double() { return rand() / (RAND_MAX + 1.0); } //[0,1)
inline double random_double(double min, double max) { return min + (max - min) * random_double(); } //[min, max)
inline double clamp(double x, double min, double max) { return x < min ? min : x > max ? max : x; }

/*line double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}*/


#include "Ray.h"
#include "Vector3.h"
#include "MeshList.h"
#include "Sphere.h"
#include "MovingSphere.h"
#include "Color.h"
#include "Camera.h"
#include "Material.h"
#include "EnvMap.h"
#include "MetaballList.h"



#endif // !PATH_TRACER_UTILS_H
