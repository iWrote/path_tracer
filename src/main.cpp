#include <iostream>
#include <omp.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "headerLibs\stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headerLibs\stb_image.h"

#include "SimpleRaiiTimer.h"
#include "path_tracer_utils.h"




MeshList random_scene_1() {
	MeshList world;

	auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			Point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = Color::random() * Color::random();
					sphere_material = std::make_shared<Lambertian>(albedo);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// Metal
					auto albedo = Color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<Dielectric>(1.5);
					world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
				
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectric>(1.5);
	world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
	world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

	return world;
}

Color ray_color(const Ray& r, const Mesh& world, const EnvMap& envmap, unsigned int depth)
{
	if (depth <= 0)
		return Color{ 0,0,0 };
	RayHit hitrec;
	if (world.hit(r, 1, 10, hitrec)) //if we hit something
	{
		Ray scattered;
		Color attenuation;

		if (hitrec.mat_ptr->scatter(r, hitrec, attenuation, scattered))
		{
			return attenuation * ray_color(scattered, world, envmap, depth - 1); //depth = bounce counting
		}

		return Color(0, 0, 0); //no light is gathered if surface doesn't scatter the ray
	}
	
	Vector3 unit_direction = unit_vector(r.direction());	
	return envmap.get_color(unit_direction); //LDR env map
	
	//return Color(1, 1, 1); //pure white bg useful for debug
	
	//background sky colour gradient
	//double t = 0.5 * (unit_direction.y() + 1);
	//return (1 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1);	
}

int main()
{

	Timer timer{ "main.cpp" };
	
	const double aspect_ratio = 1;// 16.0f / 9.0f;
	unsigned int const image_height{ 400}; //384/16*9
	unsigned int const image_width{ static_cast<int>(image_height * aspect_ratio)};
	unsigned int const image_channels {3};
	unsigned char* const image = new unsigned char[image_height * image_width * image_channels]; //pointer fixed at img "start"
	
	int envmap_width = 1024; 
	int envmap_height = 512;
	int envmap_channels = 3;
	unsigned char* envmap_image = stbi_load("assets/skytexture_debug.jpg", &envmap_width, &envmap_height, &envmap_channels, 3);
	//float* envmap_hdr_image = stbi__hdr_load(stbi_context ptr needed here, &envmap_width, &envmap_height, &envmap_channels, 3);



	unsigned int const samples_per_pixel = 4;
	unsigned int const max_depth = 4; //recursion depth aka ray-bounce-limit

	Point3 lookfrom(0,0,0);
	Point3 lookat(0,0,1);
	Vector3 vup(0, 1, 0);
	double dist_to_focus = 1.0;
	double aperture = 0;
	double vfov = 45;
	
	Camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0, 1);	
	EnvMap envmap{ envmap_width, envmap_height, envmap_channels, envmap_image, vup };
	
	MeshList world;

	
	std::shared_ptr<MetaballListMaterial> metaballmat= std::make_shared<MetaballListMaterial>();
	std::shared_ptr<MetaballList> metaballs_ptr = std::make_shared<MetaballList>(.8, metaballmat);
	//metaballs_ptr->add(std::make_shared<Sphere>(Point3(0,1,3), 1, metaballmat));
	//metaballs_ptr->add(std::make_shared<Sphere>(Point3(0, 0, 3), 1, metaballmat));
	metaballs_ptr->add(std::make_shared<Sphere>(Point3(0, 0, 10), 1, metaballmat));
	world.add(metaballs_ptr);
	

	
	//world.add(std::make_shared<MovingSphere>(Point3(0, 0, -1), Point3(0, .5, -1), 0, 1, 1, std::make_shared<Lambertian>(Color(0, 1, 0))));

	//omp_set_num_threads(2); //https://stackoverflow.com/questions/4817789/hyper-threading-made-my-renderer-10-times-slower
	//#pragma omp parallel for
	for (int j = image_height - 1; j >= 0; j--)
	{
		
		for (int i = 0; i < image_width; i++)
		{
			Color pixel_color{ 0,0,0 };

			for (unsigned int s = 0; s < samples_per_pixel; s++)
			{
				double u = (i + random_double()) / (image_width - 1);
				double v = (j + random_double()) / (image_height - 1);
				pixel_color += ray_color(cam.get_ray(u, v), world, envmap, max_depth);
			}

			write_rgb_color(i, (image_height - 1)-j, image, pixel_color, samples_per_pixel, image_width);
		}
		//std::cout << "\rScanlines remaining: " << j - 1 << ' ' << std::flush;
	}
	
	std::cout << "\nDone.\n";
	stbi_write_png("output.png", image_width, image_height, image_channels, image, 0);
	delete[] image;
	return 0;
}