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

#endif