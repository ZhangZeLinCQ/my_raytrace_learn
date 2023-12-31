﻿#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r, shared_ptr<material> m): center(cen), radius(r), mat_ptr(m) {};

  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
  point3 center;
  double radius;
  shared_ptr<material> mat_ptr; // 材质
};

// 记录是否击中球，如果击中，记录击中数据
bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;
  auto discriminant = half_b * half_b - a * c;

  if (discriminant < 0) return false;
  auto sqrtd = sqrt(discriminant);
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr; // 击中则会记录击中物体的材质（决定了颜色、散射方式）

  return true;
}

#endif