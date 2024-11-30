#include "functions.hpp"
#include <GL/gl.h>
#include <algorithm>
#include <cmath>
#include <complex>
#include <vector>

int juliaFunction(std::complex<double> &z, const std::complex<double> &c,
                  int maxIter) {
  int n = 0;
  while (std::abs(z) < 2 && n < maxIter) {
    z = z * z + c;
    n++;
  }
  return n;
}

void normalizePixels(std::vector<GLubyte> &pixels) {
  int max = *std::max_element(pixels.begin(), pixels.end());
  for (int i = 0; i < pixels.size(); ++i) {
    pixels[i] = (GLubyte)mapRange(pixels[i], 0, max, 0, 255);
  }
}

double mapRange(double value, double in_min, double in_max, double out_min,
                double out_max) {

  if (in_min == in_max) {
    return out_min;
  }

  return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
