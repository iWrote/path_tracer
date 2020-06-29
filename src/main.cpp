#include <iostream>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headerLibs\stb_image_write.h"

#include "SimpleRaiiTimer.h"

#include "path_tracer_utils.h"



/*Color ray_color(const Ray& r, const Mesh& world, unsigned int depth)
{
	if (depth <= 0)
		return Color(0, 0, 0);
	RayHit hitrec;
	if (world.hit(r, 0.0001, infinity, hitrec))
	{
		Point3 target = hitrec.p + hitrec.normal + random_unit_vector();
		//Point3 target = hitrec.p + hitrec.normal + random_in_unit_sphere();
		//Point3 target = hitrec.p + random_in_unit_sphere(hitrec.normal);


		return 0.5 * ray_color(Ray(hitrec.p, target - hitrec.p), world, depth-1);
	}

	Vector3 unit_direction = unit_vector(r.direction());
	double t = 0.5f * (unit_direction.y() + 1.0f);
	return (1.0f - t) * Color(1, 1, 1) + t * Color(0.5f,0.7f,1);
}*/

Color ray_color(const Ray& r, const Mesh& world, unsigned int depth)
{
	RayHit hitrec;
	if (world.hit(r, 0.0001, infinity, hitrec))
	{
		Ray scattered;
		Color attenuation;

		if (hitrec.mat_ptr->scatter(r, hitrec, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, world, depth - 1); //depth = bounce counting
		}

		return Color(0, 0, 0); //no light is gathered if surface doesn't scatter the ray
	}
	
	Vector3 unit_direction = unit_vector(r.direction());
	double t = 0.5 * (unit_direction.y() + 1);
	return (1 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1);
}

int main()
{
	Timer timer{ "main.cpp" };
	
	const double aspect_ratio = 16.0f/9.0f;
	unsigned int const image_height{ 384 / 16 * 9 }; //384/16*9
	unsigned int const image_width{ static_cast<int>(image_height * aspect_ratio)};
	unsigned int const image_channels {3};
	unsigned char* const image = new unsigned char[image_height * image_width * image_channels];
	
	unsigned int const samples_per_pixel = 10;
	unsigned int const max_depth = 20;
	
	Camera cam;

	MeshList world;

	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5, std::make_shared<Lambertian>(Color(0.7, 0.3, 0.3))));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100, std::make_shared<Lambertian>(Color(0.8, 0.8, 0))));
	world.add(std::make_shared<Sphere>(Point3(1, 0, -1), 0.5, std::make_shared<Metal>(Color(.8, .6, .2), 0.3)));
	world.add(std::make_shared<Sphere>(Point3(-1, 0, -1), 0.5,std::make_shared<Metal>(Color(.8, .8, .8), 1.0)));

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
				pixel_color += ray_color(cam.get_ray(u, v), world, max_depth);
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