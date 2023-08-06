﻿#include <fstream>

#include "common.h" // 包含了 ray.h、vec3.h

#include "color.h"
#include "hittable_list.h" // 包含了 hittable.h
#include "sphere.h"

color ray_color(const ray& r, const hittable& world) {
  // 渲染球
  hit_record rec;
  if (world.hit(r, 0, infinity, rec)) {
    return 0.5 * (rec.normal + color(1, 1, 1)); // 根据击中的法向量来显示颜色
  }

  // 渲染天空
  vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

  // Image
  //可以通过 屏幕的宽高比 计算出屏幕的高
  // 宽高决定了照片的像素大小
  const auto aspect_ratio = 16.0 / 9.0;
  const long long image_width = 400;
  const long long image_height = static_cast<long long>(image_width / aspect_ratio);

  // World
  // 场景由一堆可击中物体构成
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // 添加一个球
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // 添加一个地板（一个大球）

  // Camera
  // 画布
  auto viewport_height = 2.0; // 高2 （-1 - 1）
  auto viewport_width = aspect_ratio * viewport_height; // 宽 计算得到
  auto focal_length = 1.0; // 距离原点

  auto origin = point3(0, 0, 0); // 光线汇聚点（眼睛）
  auto horizontal = vec3(viewport_width, 0, 0);
  auto vertical = vec3(0, viewport_height, 0);
  // 画布的左下角
  auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);

  // Render

  std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
  out << "P6\n" << image_width << ' ' << image_height << "\n255\n"; 
  for (int j = image_height - 1; j >= 0; --j) {// The rows are written out from top to bottom
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) { // The pixels are written out in rows with pixels left to right

      // u、v 为屏幕像素上某一点坐标对应的[0-1][0-1]坐标
      auto u = double(i) / (image_width - 1); // 从左往右
      auto v = double(j) / (image_height - 1); // 从下往上

      // 从原点，射向画布的坐标uv
      ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
      color pixel_color = ray_color(r, world); // 获得这束光最终的颜色

      write_color6(out, pixel_color); // 将颜色写入图像文件
    }
  }
  out.close();
  std::cerr << "\nDone.\n";

  return 0;
}