#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "hittable.h"
#include "AGLM.h"

float eps = 0.0001f; // epsilon for comparing floats
class triangle : public hittable {
public:
   triangle() : a(0), b(0), c(0), mat_ptr(0) {}
   triangle(const glm::point3& v0, const glm::point3& v1, const glm::point3& v2, 
      std::shared_ptr<material> m) : a(v0), b(v1), c(v2), mat_ptr(m) {};

   bool hit(const ray& r, hit_record& rec) const
   {
       vec3 e1 = b - a;
       vec3 e2 = c - a;
       vec3 normal = cross(e1, e2);
       if (dot((r.origin() - a), normal )== 0 && dot((r.origin() + r.direction() - a), normal) == 0) {
           return false;
       }
       vec3 p = cross(normalize(r.direction()), e2);
       float a1 = dot(e1, p);
     
       if (fabs(a1) < eps) {
           return false;
       }
       float f = 1 / a1;
       vec3 s = r.origin() - a;
       float u = f * (dot(s, p));
       if (u < 0.0 || u > 1.0) {
           return false;
       }
       vec3 q = cross(s, e1);
       float v = f * (dot(normalize(r.direction()), q));
       if (v < 0.0 || (u+v) > 1.0) {
           return false;
       }
       float t = f * (dot(e2, q));
       if (t < 0) {
           return false;
       }
       rec.t = t;
       rec.p = r.at(t);
       rec.mat_ptr = mat_ptr;
       glm::vec3 outward_normal = normalize(cross(e1,e2));
       rec.set_face_normal(r, outward_normal);
      return true;
   }

public:
   glm::point3 a;
   glm::point3 b;
   glm::point3 c;
   std::shared_ptr<material> mat_ptr;
};

#endif
