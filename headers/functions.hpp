#pragma once

#include <GL/gl.h>
#include <complex>
#include <vector>

int juliaFunction(std::complex<double> &z, const std::complex<double> &c,
                  int maxIter);

void normalizePixels(std::vector<GLubyte> &pixels);

double mapRange(double value, double min1, double max1, double min2,
                double max2);
