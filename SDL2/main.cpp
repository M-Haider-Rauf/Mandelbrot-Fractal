#include <iostream>
#include "Engine.hpp"

/*
	Mandelbrot Fractal explorer by Haider Rauf
	7/3/2020
	lolis forever!
*/

int main(int argc, char** args)
{
	Engine* engine = Engine::get_instance();
	engine->main_loop();

	return 0;
}