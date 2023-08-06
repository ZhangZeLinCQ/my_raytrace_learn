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
  /// <param name="aperture">光圈孔径（透光大小）</param>
  /// <param name="focus_dist">焦距</param>
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) {
    auto theta = degrees_to_radians(vfov); // 视角
    auto h = tan(theta / 2); // 一半视角

    // 画布
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height; // 宽 计算得到

    //auto focal_length = 1.0; // 距离原点
    w = unit_vector(lookfrom - lookat); // 观察方向
    u = unit_vector(cross(vup, w)); // 观察点右边
    v = cross(w, u); // 观察点头顶方向

    //origin = point3(0, 0, 0);
    origin = lookfrom; // 原点（相机位置）改变为相机位置
    //horizontal = vec3(viewport_width, 0.0, 0.0);
    horizontal = focus_dist * viewport_width * u; // 设置画布（无像素概念）宽
    //vertical = vec3(0.0, viewport_height, 0.0);
    vertical = focus_dist * viewport_height * v; // 设置画布高
    //lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w; // 画布左下角（单位距离远）

    lens_radius = aperture / 2; // 光圈半径
  }

  ray get_ray(double s, double t) const {
    vec3 rd = lens_radius * random_in_unit_disk(); // 随机镜头内一点
    vec3 offset = u * rd.x() + v * rd.y(); // 光线从镜头中心的偏离

    //return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    // 光线从相机中某个地方发出
    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - (origin + offset));
  }

private:
  point3 origin;  // 光线汇聚点（眼睛）
  point3 lower_left_corner; // 画布的左下角
  vec3 horizontal; // 三维坐标表示 宽
  vec3 vertical; // 三位坐标表示 高
  vec3 u, v, w;
  double lens_radius;
};
#endif