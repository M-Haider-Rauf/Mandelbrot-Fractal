#ifndef COMPLEX_HPP
#define COMPLEX_HPP


struct Complex {
	Complex(double x = 0.0, double y = 0.0);

	double mod_sqr() const;
	double mod() const;

	double x, y;
};

#endif // !COMPLEX_HPP
