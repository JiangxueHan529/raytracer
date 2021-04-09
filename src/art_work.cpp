// Raytracer framework from https://raytracing.github.io by Peter Shirley, 2018-2020
// alinen 2021, modified to use glm and ppm_image class

#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"
#include "plane.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"

using namespace glm;
using namespace agl;
using namespace std;
int number = 3;
color ray_color(const ray& r, const hittable_list& world, int depth)
{
    hit_record rec;
    if (depth <= 0)
    {
        return color(0);
    }

    if (world.hit(r, 0.001f, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            color recurseColor = ray_color(scattered, world, depth - 1);
            return attenuation * recurseColor;
        }
        return attenuation;
    }
    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color(1, 1, 1) + t * color(0.7f, 1.0f, 0.8f);
}

color normalize_color(const color& c, int samples_per_pixel)
{
    // todo: implement me!
    float scale = 1.0f / samples_per_pixel;
    float r = std::min(0.999f, std::max(0.0f, c.r * scale));
    float g = std::min(0.999f, std::max(0.0f, c.g * scale));
    float b = std::min(0.999f, std::max(0.0f, c.b * scale));

    // apply gamma correction 
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    return color(r, g, b);
}

void ray_trace(ppm_image& image)
{
    // Image
    int height = image.height();
    int width = image.width();
    float aspect = width / float(height);
    int samples_per_pixel = 10; // higher => more anti-aliasing
    int max_depth = 100; // higher => less shadow acne
    shared_ptr<material> phongDefault;
    point3 lookfrom(1, 0, 6);
    point3 lookat(1.3, 0, 0);
    vec3 vup(0, 1, 0);
    float dist_to_focus = length(lookfrom - lookat);//
    float aperture = 0.3;

    camera cam1(lookfrom, lookat, vup, 40, aspect, aperture, dist_to_focus);
    if (blur == true) {
        phongDefault = make_shared<phong>(lookfrom);
    }


    // Camera
    vec3 camera_pos(0, 0, 6);
    float viewport_height = 2.0f;
    float focal_length = 4.0;
    camera cam2(camera_pos, viewport_height, aspect, focal_length);
    if (blur == false) {
        phongDefault = make_shared<phong>(camera_pos);
    }


    // World
    shared_ptr<material> gray = make_shared<lambertian>(color(0.5f));
    shared_ptr<material> matteGreen = make_shared<lambertian>(color(0, 0.5f, 0));
    shared_ptr<material> matteLightGreen = make_shared<lambertian>(color(0.8f, 1.0f, 0.8f));
    shared_ptr<material> matteBlue = make_shared<lambertian>(color(0.9f, 0.9f, 1.0f));
    shared_ptr<material> matteRed = make_shared<lambertian>(color(1.0f, 0.7f, 0.7f));
    shared_ptr<material> white= make_shared<lambertian>(color(1, 1, 1));
    shared_ptr<material> metalBlue = make_shared<metal>(color(0.5f, 0.5f, 1.0f), 0);
    shared_ptr<material> metalRed = make_shared<metal>(color(1, 0.5f, 0.5f), 0);
    shared_ptr<material> metalYellow = make_shared<metal>(color(1, 0.7f, 1.0f), 0);
    shared_ptr<material> glass = make_shared<dielectric>(1.5f);


    hittable_list world;
    if (number == 1) {
        world.add(make_shared<triangle>(point3(0, 1.0, -1), point3(-1.9, -0.8, -1.5), point3(0, -0.85, 1.5), phongDefault));
        world.add(make_shared<triangle>(point3(0, 1.0, -1), point3(1.7, -0.8, -1.5), point3(0, -0.85, 1.5), phongDefault));
    }
    else if (number == 2) {
        world.add(make_shared<plane>(point3(-1, 0.5, 0), vec3(1, 0, 0), metalRed));
        world.add(make_shared<plane>(point3(0, 1, 0), vec3(0, 0, 1), metalBlue));
        world.add(make_shared<plane>(point3(0, -0.5, 0), vec3(0, 1, 0),white));
    }
    else {
        world.add(make_shared<plane>(point3(-1, 0.5, 0), vec3(1, 0, 0), matteRed));
        world.add(make_shared<plane>(point3(0, 1, 0), vec3(0, 0, 1), matteLightGreen));
        world.add(make_shared<plane>(point3(0, -0.5, 0), vec3(0, 1, 0), matteBlue));
        world.add(make_shared<sphere>(point3(1.3, 0, 0), 0.2f, phongDefault));
        world.add(make_shared<sphere>(point3(1.3, 0.5, 0), 0.2f, metalRed));
        world.add(make_shared<sphere>(point3(0.8, 0, 0), 0.2f, metalBlue));
        world.add(make_shared<sphere>(point3(1.3, -0.5, 0), 0.2f, metalYellow));
        world.add(make_shared<sphere>(point3(1.8, 0, 0), 0.2f, white));
        world.add(make_shared<triangle>(point3(0, 0.2, 2.4), point3(-0.5, -0.5, 2.2), point3(0, -0.55, 3), phongDefault));
        world.add(make_shared<triangle>(point3(0, 0.2, 2.4), point3(0.5, -0.5, 2.2), point3(0, -0.55, 3), phongDefault));
        world.add(make_shared<triangle>(point3(-0.6, 0.1, 2.9), point3(-1, -0.8, 2.7), point3(-0.8, -0.85, 3.2), metalYellow));
        world.add(make_shared<triangle>(point3(-0.6, 0.1, 2.9), point3(-0.25, -0.8, 2.7), point3(-0.8, -0.85, 3.2), glass));
    }
    //world.add(make_shared<sphere>(point3(-2.25, 0, -1), 0.5f, phongDefault));
    //world.add(make_shared<sphere>(point3(-0.75, 0, -1), 0.5f, glass));
    //world.add(make_shared<sphere>(point3(0, 0, -1), 1.0f, metalBlue));
    //world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, gray));


    // Ray trace
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            color c(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) // antialias
            {
                float u = float(i + random_float()) / (width - 1);
                float v = float(height - j - 1 - random_float()) / (height - 1);
                ray r;
                if (blur == true) {
                    r = cam1.get_ray(u, v);
                }
                else {
                    r = cam2.get_ray(u, v);
                }
                c += ray_color(r, world, max_depth);
            }
            c = normalize_color(c, samples_per_pixel);
            image.set_vec3(j, i, c);
        }
    }
    if (number == 1 && blur == false) {
        image.save("test_triangle.png");
    }
    else if (number == 2 && blur == false) {
        image.save("test_plane.png");
    }
    else if (blur == false) {
        image.save("art_work.png");
    }
    else {
        image.save("art_work_blurred.png");
    }
}
