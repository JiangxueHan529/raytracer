// camera.h, from https://raytracing.github.io by Peter Shirley, 2018-2020
// modified 2021, by alinen, to support glm/float

#ifndef CAMERA_H
#define CAMERA_H

#include "AGLM.h"
using namespace glm;
bool blur = false;
class camera 
{
public:
   camera() : origin(0), horizontal(2, 0, 0), vertical(0, 2, 0)
   {
      lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - glm::vec3(0,0,1);
   }

   camera(glm::point3 pos, float viewport_height, float aspect_ratio, float focal_length) 
   {
      origin = pos;
      float viewport_width = aspect_ratio * viewport_height;
      horizontal = viewport_width * glm::vec3(1, 0, 0);
      vertical = viewport_height * glm::vec3(0, 1, 0);
      lower_left_corner = origin - horizontal * 0.5f - vertical * 0.5f - glm::vec3(0,0,focal_length);
   }

   camera(glm::point3 lookfrom,
          glm::point3 lookat,
          glm::vec3   vup,
          float vfov, // vertical field-of-view in degrees
          float aspect_ratio,
          float aperture,
          float focus_d) 
   {
       float theta =vfov* glm::pi<float>() /180.0f;
       float h = tan(theta / 2);
       float viewport_height = 2.0 * h;
       float viewport_width = aspect_ratio * viewport_height;

       w = normalize(lookfrom - lookat);
       un = normalize(cross(vup, w));
       vn = cross(w, un);

       origin = lookfrom;
       horizontal = focus_d * viewport_width * un;
       vertical = focus_d * viewport_height * vn;
       lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - focus_d * w;

       lens_radius = aperture / 2;
   }

   virtual ray get_ray(float u, float v) const 
   {
       if (blur == true) {
            vec3 rd = lens_radius * random_unit_disk();
            vec3 offset = un * rd.x + vn * rd.y;

            return ray(
                origin + offset,
                lower_left_corner + u * horizontal + v * vertical - origin - offset
            );
       }
              return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
   }

protected:
  glm::point3 origin;
  glm::point3 lower_left_corner;
  glm::vec3 horizontal;
  glm::vec3 vertical;
  vec3 un;
  vec3 vn;
  vec3 w;
  float lens_radius;
};
#endif

