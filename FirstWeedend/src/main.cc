#include <fstream>

#include "color.h"
#include "ray.h"
#include "vec3.h"

double hit_sphere(const point3& center, double radius, const ray& r) {
  // 一个向量与自身的点积等于该向量的长度的平方
  // 解方程t^2*b⋅b+2*t*b⋅(A−C)+(A−C)⋅(A−C)−r^2=0
  vec3 oc = r.origin() - center; // 光射向圆心
  auto a = dot(r.direction(), r.direction());
  auto b = 2.0 * dot(oc, r.direction());
  auto c = dot(oc, oc) - radius * radius;
  auto discriminant = b * b - 4 * a * c; // delta = b^2 - 4*a*c
  //return (discriminant > 0); 直接返回这个会有不一样的效果

  if (discriminant < 0) {
    return -1.0;
  }
  else {
    // res = (-b +- sqrt(b^2 - 4*a*c)) / (2*a) 但是-+中-是最近的，不去渲染远处的，所以用最近的
    return (-b - sqrt(discriminant)) / (2.0 * a); // 击中点
  }
}

color ray_color(const ray& r) {
  // 渲染球
  auto t = hit_sphere(point3(0, 0, -1), 0.5, r); // t 乘以ray方向为击中点
  // 球的颜色不再是单纯的红色，而是根据击中的法向量来显示颜色
  if (t > 0.0) {
    vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
    return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
  }

  // 渲染天空
  vec3 unit_direction = unit_vector(r.direction());
  t = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

  // Image
  //可以通过 屏幕的宽高比 计算出屏幕的高
  // 宽高决定了照片的像素大小
  const auto aspect_ratio = 16.0 / 9.0;
  const long long image_width = 400;
  const long long image_height = static_cast<long long>(image_width / aspect_ratio);

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
      color pixel_color = ray_color(r); // 获得这束光最终的颜色

      write_color6(out, pixel_color); // 将颜色写入图像文件
    }
  }
  out.close();
  std::cerr << "\nDone.\n";

  return 0;
}