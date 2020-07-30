#include "Engine.hpp"
#include "RenderContext.hpp"
#include "Constants.hpp"
#include "Mandelbrot.hpp"
#include <SDL.h>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <thread>
#include <functional>
#include <iostream>

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO);
	g_window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!g_window) {
		std::cerr << "Error creating window!\n";
		abort();
	}

	g_renderer = SDL_CreateRenderer(g_window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!g_renderer) {
		std::cerr << "Error creating renderer\n";
		abort();
	}

	g_rend_info = new SDL_RendererInfo;

	SDL_GetRendererInfo(g_renderer, g_rend_info);

	running = true;
	texture = SDL_CreateTexture(g_renderer, 
		g_rend_info->texture_formats[0],
		SDL_TEXTUREACCESS_STREAMING,
		WIN_WIDTH, WIN_HEIGHT
		);

	if (!texture) {
		std::cerr << "Error creating texture\n";
		abort();
	}


	pixel_table = new uint32_t[max_iter + 1]{};

	//Here we map iter number to pixel color
	//There are a lot of different coloring schemes, 
	//I just copied one I liked from the internet, which is a
	//sinusoidal coloring scheme

	SDL_PixelFormat* pix_format = SDL_AllocFormat(g_rend_info->texture_formats[0]);

	for (size_t i = 0; i < max_iter + 1; ++i) {
		constexpr double p = 1.0 / 10.0; //period (in rads)

		//Normalized rgb values
		double nr = 0.5 * std::sin(p * i) + 0.5;
		double ng = 0.5 * std::sin(p * i + (2.0 * M_PI / 3.0)) + 0.5;
		double nb = 0.5 * std::sin(p * i + (4.0 * M_PI / 3.0)) + 0.5;

		//scale to [0, 255] range
		unsigned char r = std::round(nr * 255.0);
		unsigned char g = std::round(ng * 255.0);
		unsigned char b = std::round(nb * 255.0);

		pixel_table[i] = SDL_MapRGB(pix_format, r, g, b);
	}

	SDL_FreeFormat(pix_format);
}

Engine::~Engine()
{
	delete[] pixel_table;
	delete g_rend_info;
	SDL_DestroyWindow(g_window);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyTexture(texture);
	SDL_Quit();
}

void Engine::main_loop()
{
	update_draw();
	while (running) {
		handle_events();
		render();
	}
}

void Engine::handle_events()
{
	SDL_Event event_q;
	bool update = false;
	double dt = (SDL_GetTicks() - prev_time) / 1000.0;
	this->prev_time = SDL_GetTicks();
	double speed = view_radius * dt / 1.1;


	while (SDL_PollEvent(&event_q)) {
		if (event_q.type == SDL_QUIT) {
			running = false;
		}
		else if (event_q.type == SDL_KEYDOWN) {
			switch (event_q.key.keysym.sym) {
			case SDLK_ESCAPE:
				running = false;
				break;

			case SDLK_c:
				save_screenshot();
				break;

			default:
				break;
			}
		}
	}

	auto key_states = SDL_GetKeyboardState(nullptr);
	if (key_states[SDL_SCANCODE_A]) {
		x_off -= speed;
		update = true;
	}
	if (key_states[SDL_SCANCODE_D]) {
		x_off += speed;
		update = true;
	}
	if (key_states[SDL_SCANCODE_W]) {
		y_off -= speed;
		update = true;
	}
	if (key_states[SDL_SCANCODE_S]) {
		y_off += speed;
		update = true;
	}
	if (key_states[SDL_SCANCODE_EQUALS]) {
		view_radius *= 0.90;
		update = true;
	}
	if (key_states[SDL_SCANCODE_MINUS]) {
		view_radius *= 1.10;
		update = true;
	}

	if (update) update_draw();
}

void Engine::update_draw()
{
	void* pixels = nullptr;
	int pitch;

	//lock texture for write-access
	SDL_LockTexture(this->texture, nullptr, &pixels, &pitch);
	
	std::thread threads[4]; //4 threads
	//split whole screen into 4 slices, run update pixel routine for each slice on
	//separate thread.....
	for (size_t i = 0; i < 4; ++i) {
		auto slice_thread = [&](size_t j)
		{
			size_t start = j * (WIN_WIDTH / 4);
			size_t end = start + (WIN_WIDTH / 4);
			if (j == 3) end = WIN_WIDTH;

			this->update_screen_slice(start, end, pitch, pixels);;

		};

		threads[i] = std::thread(slice_thread, i);
	}

	for (size_t i = 0; i < 4; ++i) {
		threads[i].join();
	}

	SDL_UnlockTexture(this->texture);
}

void Engine::render() const
{
	SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
	SDL_RenderClear(g_renderer);

	SDL_RenderCopy(g_renderer, texture, nullptr, nullptr);

	SDL_RenderPresent(g_renderer);
}

void Engine::save_screenshot() const 
{
	char file_name[20];
	SDL_Surface* ss = SDL_CreateRGBSurfaceWithFormat(
		0, WIN_WIDTH, WIN_HEIGHT,
		32, g_rend_info->texture_formats[0]
		);

	SDL_RenderReadPixels(g_renderer, nullptr, g_rend_info->texture_formats[0], ss->pixels, ss->pitch);
	sprintf_s(file_name, "ss_%ld.bmp", time(nullptr));
	SDL_SaveBMP(ss, file_name);

	SDL_FreeSurface(ss);

}

void Engine::update_screen_slice(size_t start, size_t end, int pitch, void* pixels)
{
	for (size_t y = 0; y < WIN_HEIGHT; ++y) {
		for (size_t x = start; x < end; ++x) {

			uint32_t* target = (uint32_t*) (((Uint8*)(pixels)) + y * pitch + x * 4);

			double cr = scale(x, 0, WIN_WIDTH - 1, -view_radius + x_off, view_radius + x_off);
			double ci = scale(y, WIN_HEIGHT - 1, 0, -view_radius + y_off, view_radius + y_off);

			unsigned iters = get_mandlebrot_iter(cr, ci, max_iter);
			*target = pixel_table[iters];
		}
	}
}
