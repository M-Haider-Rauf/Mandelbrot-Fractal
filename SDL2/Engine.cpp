#include "Engine.hpp"
#include "RenderContext.hpp"
#include "Constants.hpp"
#include "Mandelbrot.hpp"
#include <SDL.h>
#include <cassert>
#include <iostream>
#include <cstdlib>
#include "Complex.hpp"
#include <cstring>
#include <ctime>

Engine::Engine()
{
	SDL_Init(SDL_INIT_VIDEO);
	g_window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_SHOWN);
	assert(g_window && "Failed to create Window");
	g_renderer = SDL_CreateRenderer(g_window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	assert(g_renderer && "Failed to create Renderer");

	running = true;
	texture = SDL_CreateTexture(g_renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, WIN_WIDTH, WIN_HEIGHT);

	

	pixel_table = new SDL_Color[iter_count]{};
	

	for (size_t i = 0; i < iter_count; ++i) {
		double factor = i * 1.0 / (iter_count - 1);
		factor = std::sqrt(factor);

		unsigned char val = std::round(factor * 255.0);
		pixel_table[i] = {
			val, val, val, 255
		};
	}
}

Engine::~Engine()
{
	delete pixel_table;
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
	double speed = view_radius * dt;


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


	SDL_LockTexture(this->texture, nullptr, &pixels, &pitch);

	auto SetPixel = [&](int x, int y, const SDL_Color& pixel)
	{
		Uint8* target = (Uint8*)(pixels)+(y * pitch) + x * 4;
		target[0] = pixel.b;
		target[1] = pixel.g;
		target[2] = pixel.r;
	};

	for (size_t y = 0; y < WIN_HEIGHT; ++y) {
		for (size_t x = 0; x < WIN_WIDTH; ++x) {

			Complex c = {
				scale(x, 0, WIN_WIDTH - 1, -view_radius + x_off, view_radius + x_off),
				scale(y, WIN_HEIGHT - 1, 0, -view_radius + y_off, view_radius + y_off)
			};

			int iters = get_mandlebrot_iter(c.x, c.y, iter_count);
			if (iters != -1) {
				SetPixel(x, y, pixel_table[iters]);
			}
			else {
				SetPixel(x, y, {0, 0, 0, 255});
			}
		}
	}

	SDL_UnlockTexture(texture);
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
	SDL_Surface* ss = SDL_CreateRGBSurface(0, WIN_WIDTH, WIN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	SDL_RenderReadPixels(g_renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, ss->pixels, ss->pitch);
	sprintf_s(file_name, "ss_%ld.bmp", time(nullptr));
	SDL_SaveBMP(ss, file_name);
}