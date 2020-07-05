#include "Mandelbrot.hpp"
#include "Constants.hpp"

//scale aka "compressing" or "streching" a function f()
//maps one of numbers range to another
double scale(double val, double in_min, double in_max, double out_min, double out_max)
{
	return (val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned get_mandlebrot_iter(double cr, double ci, unsigned max_iter)
{
	double zx = 0.0;
	double zy = 0.0;

	unsigned n = 0; //iteration count

	while (n < max_iter && zx * zx + zy * zy <= 4.0) {
		double tx = cr + ((zx + zy) * (zx - zy));
		double ty = 2 * zx * zy + ci;

		zx = tx;
		zy = ty;

		++n;
	}

	return n;
}
