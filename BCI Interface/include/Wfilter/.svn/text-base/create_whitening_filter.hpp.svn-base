#ifndef create_whitening_filter_HPP
#define create_whitening_filter_HPP

#define DEBUG_OUTPUT 0

#include "fftw3.h"

#if DEBUG_OUTPUT
#include "m_file.hpp"
#endif

#include <cmath>
#include <complex>
#include <algorithm>
#include <functional>
#include <vector>
#include <iterator>
#include <fstream>



void make_real(fftw_complex *H, int length);

void create_whitening_filter(double * x, int x_length, double * dest, int n);

#endif