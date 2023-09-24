#include <iostream>

#include "colour.h"
#include "ray.h"
#include "utilities.h"
#include "hittable.h";
#include "hittable_list.h"
#include "sphere.h"

colour ray_colour(const ray& r, const hittable& world) {
	hit_record rec;
	if (world.hit(r, interval(0, infinity), rec)) {
		return 0.5 * (rec.normal + colour(1, 1, 1));
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	// return blended value, blue to white gradient
	return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
}
// PPM Image Format

int main() {

	double aspect_ratio = 16.0 / 9.0;
	double image_width = 400;

	// Calculate the image_height, make sure that it's at least 1
	double image_height = static_cast<int>(image_width / aspect_ratio);
	// if height < 1, make it 1, else keep as is
	image_height = (image_height < 1) ? 1 : image_height;

	// World
	hittable_list world;
	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	// Camera
	auto focal_length = 1.0;
	// Viewport widths less than one are ok since they are real valued
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);
	auto camera_center = point3(0, 0, 0);

	// Calculate the vectors across the horizontal and down the vertical viewport edges
	auto viewport_u = vec3(viewport_width, 0, 0);
	auto viewport_v = vec3(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// Calculate the location of the upper left pixel
	auto viewpoint_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
	auto pixel00_loc = viewpoint_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	// generate pixels per column
	for (int j = 0; j < image_height; ++j) {
		std::clog << "\rScanlines remaining " << (image_height - j) << ' ' << std::flush;
		// generate pixel per row
		for (int i = 0; i < image_width; ++i) {
			auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);

			colour pixel_colour = ray_colour(r, world);
			write_colour(std::cout, pixel_colour);
		}
	}
	std::clog << "\rDone.                 \n";
}