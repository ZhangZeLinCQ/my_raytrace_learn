﻿#include <fstream>

#include "common.h" // 包含了 ray.h、vec3.h

#include "color.h"
#include "hittable_list.h" // 包含了 hittable.h
#include "sphere.h"
#include "camera.h"
#include "material.h"

hittable_list random_scene() {
  hittable_list world;

  auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5)); // 大地颜色
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material)); // 添加大地

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double(); // 随机一个0-1决定材质
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()); //高0.2

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          sphere_material = make_shared<metal>(albedo, fuzz);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
        else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          world.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  // 最大的玻璃球
  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  // 最大的漫反射球
  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  // 最大的金属球
  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  return world;
}

// 设置递归深度（光线反射次数）
color ray_color(const ray& r, const hittable& world, int depth) {
  hit_record rec;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0)
    return color(0, 0, 0);

  // 渲染击中物体
  if (world.hit(r, 0.001, infinity, rec)) { //0 -> 0.001 solve shadow acne problem
    ray scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return attenuation * ray_color(scattered, world, depth - 1); // 计算颜色
    return color(0, 0, 0);
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
  //const auto aspect_ratio = 16.0 / 9.0;
  const auto aspect_ratio = 3.0 / 2.0;
  //const long long image_width = 400;
  const int image_width = 1200;
  const long long image_height = static_cast<long long>(image_width / aspect_ratio);
  //const int samples_per_pixel = 100; // 每个像素采样100个光线
  const int samples_per_pixel = 500;
  const int max_depth = 50; // 光线弹动次数

  // World
  auto R = cos(pi / 4);
  // 场景由一堆可击中物体构成
  hittable_list world;

  switch (3) {
  case 0: {
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left)); // 反向物体，与正向包围起来形成空心玻璃球
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
  }
  break;
  case 1: {
    auto material_left = make_shared<lambertian>(color(0, 0, 1));
    auto material_right = make_shared<lambertian>(color(1, 0, 0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));
  }
  break;
  default:
    world = random_scene();
    break;
  }


  // Camera
  //point3 lookfrom(3,3,2);
  point3 lookfrom(13, 2, 3);
  //point3 lookat(0, 0, -1);
  point3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  //auto dist_to_focus = (lookfrom - lookat).length(); // 焦距
  auto dist_to_focus = 10.0;
  //auto aperture = 2.0; // 光圈越大越模糊
  auto aperture = 0.1;

  //camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 20, aspect_ratio);
  //camera cam(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0), 90, aspect_ratio);
  camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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