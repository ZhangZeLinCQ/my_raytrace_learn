#include <iostream>
#include <fstream>

int main() {

  // Image

  const int image_width = 256;
  const int image_height = 256;

  // Render

  //// use ppm p3
  //std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  //for (int j = image_height - 1; j >= 0; --j) {
  //  std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
  //  for (int i = 0; i < image_width; ++i) {
  //    auto r = double(i) / (image_width - 1);
  //    auto g = double(j) / (image_height - 1);
  //    auto b = 0.25;

  //    int ir = static_cast<int>(255.999 * r);
  //    int ig = static_cast<int>(255.999 * g);
  //    int ib = static_cast<int>(255.999 * b);

  //    std::cout << ir << ' ' << ig << ' ' << ib << '\n';
  //  }
  //}
  //std::cerr << "\nDone.\n";

  // use ppm p6 这种格式文件更小，但看到不到具体数值
  std::ofstream out("image.ppm", std::ios::out | std::ios::binary);
  out << "P6\n" << image_width << ' ' << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      auto r = double(i) / (image_width - 1);
      auto g = double(j) / (image_height - 1);
      auto b = 0.25;
      out << static_cast<char>(255.999 * r)
        << static_cast<char>(255.999 * g)
        << static_cast<char>(255.999 * b);
    }
  }
  out.close();
  std::cerr << "\nDone.\n";

  return 0;
}