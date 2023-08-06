#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class camera {
public:

  /// <summary>
  /// 相机构造
  /// </summary>
  /// <param name="lookfrom">眼睛（相机）位置</param>
  /// <param name="lookat">眼睛看向哪里</param>
  /// <param name="vup">上方向</param>
  /// <param name="vfov">vertical field-of-view in degrees 视线角度（上下）</param>
  /// <param name="aspect_ratio"></param>
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio) {
    auto theta = degrees_to_radians(vfov); // 视角
    auto h = tan(theta / 2); // 一半视角

    // 画布
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height; // 宽 计算得到

    //auto focal_length = 1.0; // 距离原点
    auto w = unit_vector(lookfrom - lookat); // 观察方向
    auto u = unit_vector(cross(vup, w)); // 观察点右边
    auto v = cross(w, u); // 观察点头顶方向

    //origin = point3(0, 0, 0);
    origin = lookfrom; // 原点（相机位置）改变为相机位置
    //horizontal = vec3(viewport_width, 0.0, 0.0);
    horizontal = viewport_width * u; // 设置画布（无像素概念）宽
    //vertical = vec3(0.0, viewport_height, 0.0);
    vertical = viewport_height * v; // 设置画布高
    //lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w; // 画布左下角（单位距离远）
  }

  //// u、v 为屏幕像素上某一点坐标对应的[0-1][0-1]坐标
  //// 生成从原点到画布(非0-1)上的光线
  //ray get_ray(double u, double v) const {
  //  return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
  //}
  ray get_ray(double s, double t) const {
    return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
  }

private:
  point3 origin;  // 光线汇聚点（眼睛）
  point3 lower_left_corner; // 画布的左下角
  vec3 horizontal; // 三维坐标表示 宽
  vec3 vertical; // 三位坐标表示 高
};
#endif