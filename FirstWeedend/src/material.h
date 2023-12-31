﻿#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"

struct hit_record;

// 生成一个散射光线scattered
// 发生散射时光线的衰减attenuation（颜色）
class material {
public:
  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

// 不同类型材质可以有不同的初始化方式、方法，但要继承自material

// Lambertian漫反射材质
class lambertian : public material {
public:
  lambertian(const color& a) : albedo(a) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
    //auto scatter_direction = rec.normal + random_in_unit_sphere();
    auto scatter_direction = rec.normal + random_unit_vector(); // 此值可能为0
    //auto scatter_direction = rec.normal + random_in_hemisphere(rec.normal);

    // Catch degenerate scatter direction
    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction);
    attenuation = albedo;
    return true;
  }

public:
  color albedo;
};

// 金属材质
class metal : public material {
public:
  metal(const color& a, double f = 0) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
    vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal); // 全镜面反射
    //scattered = ray(rec.p, reflected);
    scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere()); // 添加了随机因素，表面不那么光滑
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0); // 只保留和法向量同侧的光
  }

public:
  color albedo;
  double fuzz; // 反射的模糊程度 0表示全镜面反射 有可能发射至物体下方，那么让表面吸收这个光（discard）
};

// 介质（可发生折射的材料）
class dielectric : public material {
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double refraction_ratio = rec.front_face ? (1.0 / ir) : ir; // 内部或外部

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0; // >1 表示发生全反射，不产生折射
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
      direction = reflect(unit_direction, rec.normal);
    else
      direction = refract(unit_direction, rec.normal, refraction_ratio);

    scattered = ray(rec.p, direction);
    return true;
  }

public:
  double ir; // Index of Refraction 折射率

private:
  static double reflectance(double cosine, double ref_idx) { // Christophe Schlick方法：处理Fresnel效应
    // Use Schlick's approximation for reflectance.
    auto r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
  }
};
#endif