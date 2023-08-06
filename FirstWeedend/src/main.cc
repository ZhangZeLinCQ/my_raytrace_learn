#include <fstream>

#include "common.h" // 包含了 ray.h、vec3.h

#include "color.h"
#include "hittable_list.h" // 包含了 hittable.h
#include "sphere.h"
#include "camera.h"

// 设置递归深度（光线反射次数）
color ray_color(const ray& r, const hittable& world, int depth) {
  // 渲染球
  hit_record rec;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0)
    return color(0, 0, 0);

  if (world.hit(r, 0.001, infinity, rec)) { //solve shadow acne problem //太小的偏置可能无法消除阴影痤疮，而太大的偏置可能会导致其他的渲染错误，如阴影漂移
    point3 target = rec.p + rec.normal + random_unit_vector(); // 从 以击中点为切点 的另一个单位圆 中找一个随机点 作为光线反射目的地
    return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
    //return 0.5 * (rec.normal + color(1, 1, 1)); // 根据击中的法向量来显示颜色
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
  const int samples_per_pixel = 100; // 每个像素采样100个光线
  const int max_depth = 50; // 光线弹动次数

  // World
  // 场景由一堆可击中物体构成
  hittable_list world;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5)); // 添加一个球
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 100)); // 添加一个地板（一个大球）

  // Camera
  camera cam;

  // Render

  std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
  out << "P6\n" << image_width << ' ' << image_height << "\n255\n"; 
  for (int j = image_height - 1; j >= 0; --j) {// The rows are written out from top to bottom
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) { // The pixels are written out in rows with pixels left to right
      color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixel; ++s) {
        // u、v 为屏幕像素上某一点坐标对应的[0-1][0-1]坐标
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);
        ray r = cam.get_ray(u, v); // 从原点，射向画布的坐标uv
        pixel_color += ray_color(r, world, max_depth); // 获得这些光最终的颜色和
      }
      write_color6(out, pixel_color, samples_per_pixel); // 将颜色写入图像文件
    }
  }
  out.close();
  std::cerr << "\nDone.\n";

  return 0;
}