#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class camera {
public:
  camera() {
    auto aspect_ratio = 16.0 / 9.0;
    // 画布
    auto viewport_height = 2.0; // 高2 （-1 - 1）
    auto viewport_width = aspect_ratio * viewport_height; // 宽 计算得到
    auto focal_length = 1.0; // 距离原点

    origin = point3(0, 0, 0);
    horizontal = vec3(viewport_width, 0.0, 0.0);
    vertical = vec3(0.0, viewport_height, 0.0);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
  }

  // u、v 为屏幕像素上某一点坐标对应的[0-1][0-1]坐标
  // 生成从原点到画布(非0-1)上的光线
  ray get_ray(double u, double v) const {
    return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
  }

private:
  point3 origin;  // 光线汇聚点（眼睛）
  point3 lower_left_corner; // 画布的左下角
  vec3 horizontal; // 三维坐标表示 宽
  vec3 vertical; // 三位坐标表示 高
};
#endif