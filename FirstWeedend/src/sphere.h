#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r) : center(cen), radius(r) {};

  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
  point3 center;
  double radius;
};

// 记录是否击中球，如果击中，记录击中数据
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  // 解方程t^2*b⋅b+2*t*b⋅(A−C)+(A−C)⋅(A−C)−r^2=0
  vec3 oc = r.origin() - center; // 光射向圆心
  //auto a = dot(r.direction(), r.direction());
  auto a = r.direction().length_squared();
  //auto b = 2.0 * dot(oc, r.direction());
  auto half_b = dot(oc, r.direction()); //令 b = 2 * half_b
  //auto c = dot(oc, oc) - radius * radius;
  auto c = oc.length_squared() - radius * radius;
  //auto discriminant = b * b - 4 * a * c; // delta = b^2 - 4*a*c
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) return false;
  auto sqrtd = sqrt(discriminant);

  // res = (-b +- sqrt(b^2 - 4*a*c)) / (2*a) 
  // 令 b = 2 * half_b 公式可简化为==> res = (-half_b +- sqrt(half_b^2 - a*c))
  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  // 确定范围内是否有解
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  rec.normal = (rec.p - center) / radius;

  return true;
}

#endif