#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record { // 击中记录
  point3 p; // 击中点
  vec3 normal; // 击中处法向量
  double t; // 击中距离（时间）
  bool front_face; 

  inline void set_face_normal(const ray& r, const vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0; // 点乘小于0 两个向量反向
    normal = front_face ? outward_normal : -outward_normal; // 小于0光线来自外部，大于0光线来自内部
  }
};

// 抽象类，所有可被光线击中的物体要继承这个类，然后实现hit方法
class hittable {
public:
  //  the hit only “counts” if t_min < t < t_max
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif