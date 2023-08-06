#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record { // 击中记录
  point3 p; // 击中点
  vec3 normal; // 击中处法向量
  double t; // 击中距离（时间）
};

// 抽象类，所有可被光线击中的物体要继承这个类，然后实现hit方法
class hittable {
public:
  //  the hit only “counts” if t_min < t < t_max
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif