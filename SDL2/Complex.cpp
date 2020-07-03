#include "Complex.hpp"
#include <cmath>

Complex::Complex(double x, double y) : x (x), y (y)
{
}

double Complex::mod_sqr() const
{
	return x * x + y * y;
}

double Complex::mod() const
{
	return std::sqrt(this->mod_sqr());
}
