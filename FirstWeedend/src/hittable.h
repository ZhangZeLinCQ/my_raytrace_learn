#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record { // ���м�¼
  point3 p; // ���е�
  vec3 normal; // ���д�������
  double t; // ���о��루ʱ�䣩
};

// �����࣬���пɱ����߻��е�����Ҫ�̳�����࣬Ȼ��ʵ��hit����
class hittable {
public:
  //  the hit only ��counts�� if t_min < t < t_max
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif