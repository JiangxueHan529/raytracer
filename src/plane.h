#ifndef PLANE_H_
#define PLANE_H_

#include "hittable.h"
#include "AGLM.h"

class plane : public hittable {
public:
   plane() : a(0), n(0), mat_ptr(0) {}
   plane(const glm::point3& p, const glm::vec3& normal, 
      std::shared_ptr<material> m) : a(p), n(normal), mat_ptr(m) {};

   bool hit(const ray& r, hit_record& rec) const
   {
       if (dot(normalize(r.direction()), n) == 0) {
           return false;
      }
       if (dot((r.origin() - a), n) == 0 && dot((r.origin() + r.direction() - a), n) == 0) {
           return false;
       }
       float t = (float)dot((a - r.origin()), n) / (float)dot(normalize(r.direction()), n);
       if (t < 0) {
           return false;
       }
       rec.t = t;
       rec.p = r.at(t);
       rec.mat_ptr = mat_ptr;
       glm::vec3 outward_normal = normalize(n);
       rec.set_face_normal(r, outward_normal);
      return true;
   }

public:
   glm::vec3 a;
   glm::vec3 n;
   std::shared_ptr<material> mat_ptr;
};

#endif
