#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
#include "AGLM.h"
#include "ray.h"
#include "hittable.h"
using namespace glm;
class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const = 0;
  virtual ~material() {}
};

class lambertian : public material {
public:
  lambertian(const glm::color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
  {
     // todo

      /*vec3 unitn = normalize(rec.normal);
      vec3 lightDir = normalize(vec3(5, 5, 0) - rec.p);
      color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;

      attenuation = diffuse;
      return false;*/

      vec3 scatter_direction = rec.normal + random_unit_vector();
      if (near_zero(scatter_direction)) {
          scatter_direction = rec.normal;
      }
      scattered = ray(rec.p, scatter_direction);
      attenuation = albedo;
      return true;
  }

public:
  glm::color albedo;
};

class phong : public material {
public:
  phong(const glm::vec3& view) :
     diffuseColor(0,0,1), 
     specColor(1,1,1),
     ambientColor(.01f, .01f, .01f),
     lightPos(5,5,0),
     viewPos(view), 
     kd(0.45), ks(0.45), ka(0.1), shininess(10.0) 
  {}

  phong(const glm::color& idiffuseColor, 
        const glm::color& ispecColor,
        const glm::color& iambientColor,
        const glm::point3& ilightPos, 
        const glm::point3& iviewPos, 
        float ikd, float iks, float ika, float ishininess) : 
     diffuseColor(idiffuseColor), 
     specColor(ispecColor),
     ambientColor(iambientColor),
     lightPos(ilightPos),
     viewPos(iviewPos), kd(ikd), ks(iks), ka(ika), shininess(ishininess) 
  {}

  virtual bool scatter(const ray& r_in, const hit_record& hit, 
     glm::color& attenuation, ray& scattered) const override 
  {
      //glm::color Ia = ka * ambientColor;
      //glm::color Id = kd * (float)fmax(0.0f,(dot(normalize(lightPos - hit.p), normalize(hit.normal)))) * diffuseColor; // need to add material color (albedo)
      //vec3 view = normalize(viewPos - hit.p);
      //vec3 r = normalize(reflect(normalize(lightPos - hit.p), normalize(hit.normal))); //2 * dot(normalize(lightPos - hit.p), normalize(hit.normal)) * normalize(hit.normal) - normalize(lightPos - hit.p);
      //glm::color Is = ks * specColor * pow((dot(view, r)), shininess);
      //attenuation = Ia + Id + Is;
      attenuation = color(0);
     return false;
  }

public:
  glm::color diffuseColor;
  glm::color specColor;
  glm::color ambientColor;
  glm::point3 lightPos;
  glm::point3 viewPos; 
  float kd; 
  float ks;
  float ka; 
  float shininess;
};

class metal : public material {
public:
   metal(const glm::color& a, float f) : albedo(a), fuzz(glm::clamp(f,0.0f,1.0f)) {}

   virtual bool scatter(const ray& r_in, const hit_record& rec, 
      glm::color& attenuation, ray& scattered) const override 
   {
       vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
       scattered = ray(rec.p, reflected + fuzz * random_unit_sphere());
       attenuation = albedo;
       return (dot(scattered.direction(), rec.normal) > 0);

   }

public:
   glm::color albedo;
   float fuzz;
};

class dielectric : public material {
public:
  dielectric(float index_of_refraction) : ir(index_of_refraction) {}


  vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat)const {
      float cos_theta = fmin(dot(-uv, n), 1.0);
      vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
      vec3 r_out_parallel = (float)-sqrt(fabs(1.0 - r_out_perp.length() * r_out_perp.length())) * n;
      return r_out_perp + r_out_parallel;
  }

  virtual bool scatter(const ray& r_in, const hit_record& rec, 
     glm::color& attenuation, ray& scattered) const override 
   {
      attenuation = color(1.0, 1.0, 1.0);
      float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;
      vec3 unit_direction = normalize(r_in.direction());
      vec3 refracted = refract(unit_direction, rec.normal, refraction_ratio);
      scattered = ray(rec.p, refracted);
      return true;
   }


public:
  float ir; // Index of Refraction
};


#endif

