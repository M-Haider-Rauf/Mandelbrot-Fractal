#include "Mandelbrot.hpp"
#include "Constants.hpp"
#include "Complex.hpp"

double scale(double val, double in_min, double in_max, double out_min, double out_max)
{
	return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int get_mandlebrot_iter(double x, double y, int max_iter)
{
	const Complex c = { x, y };
	Complex z = { 0.0, 0.0 };
	int n = 0;

	while (true) {
		z = { z.x * z.x - z.y * z.y + c.x,
			2 * z.x * z.y + c.y };

		if (max_iter == n) {
			return -1;
		}

		if (z.mod_sqr() > 4) {
			return n;
		}
		++n;
	}
}
