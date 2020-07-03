#ifndef MANDELBROT_HPP
#define MANDELBROT_HPP

constexpr size_t MAX_IERATIONS = 127;
constexpr double MAX_BOUND = 4.0;

double scale(double val, double in_min, double in_max, double out_min, double out_max);
int get_mandlebrot_iter(double x, double y, int max_iter);

#endif // !MANDELBROT_HPP
