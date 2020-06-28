#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headerLibs\stb_image_write.h"

#include "SimpleRaiiTimer.h"

#include "path_tracer_utils.h"
#include "MeshList.h"
#include "Sphere.h"
#include "Color.h"
#include "Camera.h"


double hit_sphere(const Point3& center, double radius, const Ray& r)
{
	Vector3 oc{ r.origin() - center };
	double a = r.direction().length_squared();
	double half_b = dot(oc, r.direction());
	double c = oc.length_squared() - radius * radius;
	double inroot = half_b*half_b - a*c;
	if (inroot < 0)
	{
		return -1;
	}
	else
	{
		return (-half_b - std::sqrt(inroot)) / a;
	}
	
}

Color ray_color(const Ray& r, const Mesh& world)
{
	RayHit hitrec;
	if (world.hit(r, 0, infinity, hitrec))
	{
		return 0.5 * (hitrec.normal + Color(1, 1, 1));
	}

	Vector3 unit_direction = unit_vector(r.direction());
	double t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Color(1, 1, 1) + t * Color(0.5f,0.7f,1);
}

int main()
{
	Timer timer{ "main.cpp" };
	
	const double aspect_ratio = 16.0f/9.0f;
	unsigned int const image_height{ 384/16*9 };
	unsigned int const image_width{ static_cast<int>(image_height * aspect_ratio)};
	unsigned int const image_channels {3};
	unsigned char* const image = new unsigned char[image_height * image_width * image_channels];
	
	unsigned int samples_per_pixel = 20;

	Camera cam;

	MeshList world;
	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	unsigned char* img = image;
	for (int j = image_height-1; j >= 0; j--)
	{
		for (int i = 0; i < image_width; i++)
		{
			Color pixel_color{ 0,0,0 };
			for (unsigned int s = 0; s < samples_per_pixel; s++)
			{
				double u = (i + random_double()) / (image_width - 1);
				double v = (j + random_double()) / (image_height - 1);
				pixel_color += ray_color(cam.get_ray(u, v), world);
			}	
			
			write_rgb_color(img, pixel_color, samples_per_pixel);
		}
		std::cout << "\rScanlines remaining: " << j-1 << ' ' << std::flush;
	}
	std::cout << "\nDone.\n";
	stbi_write_png("output.png", image_width, image_height, image_channels, image, 0);
	delete[] image;
	return 0;
}