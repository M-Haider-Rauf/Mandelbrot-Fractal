#include "Mandelbrot.hpp"
#include "Constants.hpp"
#include "Complex.hpp"

//scale aka "compressing" or "streching" a function f()
//maps one of numbers range to another
double scale(double val, double in_min, double in_max, double out_min, double out_max)
{
	return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int get_mandlebrot_iter(double x, double y, int max_iter)
{
	const Complex c = { x, y };
	Complex z = { 0.0, 0.0 };
	int n = 0;

	while (n < max_iter && z.mod_sqr() <= 4.0) {
		z = { z.x * z.x - z.y * z.y + c.x,
			2 * z.x * z.y + c.y };

		++n;
	}

	return n;
}
