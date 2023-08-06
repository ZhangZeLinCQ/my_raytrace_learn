#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <fstream>
#include <iostream>

void write_color3(std::ostream& out, color pixel_color) {
  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(255.999 * pixel_color.x()) << ' '
    << static_cast<int>(255.999 * pixel_color.y()) << ' '
    << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

void write_color6(std::ofstream& out, const color& pixel_color) {
  out << static_cast<char>(255.999 * pixel_color.x())
    << static_cast<char>(255.999 * pixel_color.y())
    << static_cast<char>(255.999 * pixel_color.z());
}

void write_color3(std::ostream& out, color pixel_color, int samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples.
  auto scale = 1.0 / samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;

  // Write the translated [0,255] value of each color component.
  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
    << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}

/// <summary>
/// 写入颜色值
/// </summary>
/// <param name="out"></param>
/// <param name="pixel_color">记录当前像素上超采样所有光线颜色值之和</param>
/// <param name="samples_per_pixel">记录超采样光线数量</param>
void write_color6(std::ofstream& out, color pixel_color, int samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide the color by the number of samples.
  auto scale = 1.0 / samples_per_pixel;
  r *= scale;
  g *= scale;
  b *= scale;

  // Write the translated [0,255] value of each color component.
  out << static_cast<char>(256 * clamp(r, 0.0, 0.999))
    << static_cast<char>(256 * clamp(g, 0.0, 0.999))
    << static_cast<char>(256 * clamp(b, 0.0, 0.999));
}

#endif