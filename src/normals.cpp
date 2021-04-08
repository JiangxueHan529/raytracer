#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"

using namespace glm;
using namespace agl;

float hit_sphere(const ray& ray, const vec3& center, float radius)
{
	vec3 oc = ray.origin() - center;
	float a = dot(ray.direction(), ray.direction());
	float b = 2.0f * dot(oc, ray.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1;
	}
	else {
		return (-b - sqrt(discriminant)) / (2 * a);
	}

	return -1;
}

color ray_color(const ray& ray)
{
	float t = hit_sphere(ray, vec3(0, 0, -1), 0.5f);
	if (t > 0)
	{
		vec3 hit_point = ray.origin() + t * ray.direction();
		vec3 normal = normalize(hit_point - vec3(0, 0, -1));
		color c = 0.5f * (normal + vec3(1));
		return c;
	}

	// let's use the y coordinate to make a gradient
	vec3 unit_direction = normalize(ray.direction());

	t = 0.5f * (unit_direction.y + 1.0f);
	color c1(0.5f, 0.7f, 1.0f);
	color c2(1.0);
	color c = c2 * (1 - t) + c1 * t;
	return c;
}

//ray_color(ray)
//….
//sphere_center = point3(0, 0, -1)
//t = hit_sphere(sphere_center, 0.5, ray)
//if t > 0
//hit_point = ray.origin + t * ray.direction
//normal = normalize(hit_point – sphere_center);
//color = 0.5 * (normal + vec3(1))
//return color
//dir = normalize(ray.direction);
//t = 0.5f * (dir.y + 1.0f);
//return white * (1 - t) + lightBlue * t;




void ray_trace(ppm_image& image)
{
	int height = image.height();
	int width = image.width();
	float aspect = (float)width / (float)height;
	vec3 camera_pos(0.0f, 0.0f, 0.0f);
	float focal_length = 1.0f;
	float viewport_half_width = 0.5f * (2.0f * aspect);
	float viewport_half_height = 0.5f * 2.0f;
	vec3 corner = camera_pos - vec3(0.0f, 0.0f, focal_length) - 0.5f * vec3(2.0f * aspect, 2.0f, 0.0f);
	for (int row = 0; row < height;row++) {
		for (int col = 0; col < width; col++) {
			float v = (float)(height - row - 1) / (float)(height - 1);
			float u = (float)col / (float)(width - 1);
			vec3 pixel_pos = corner + vec3(u * 2.0f * aspect, v * 2.0f, 0);
			ray r(camera_pos, pixel_pos - camera_pos);
			color c = ray_color(r);
			image.set_vec3(row, col, c);
		}
	}

	image.save("normals.png");
}