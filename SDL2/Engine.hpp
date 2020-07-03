#ifndef ENGINE_HPP
#define ENGINE_HPP

struct SDL_Texture;
struct SDL_Color;

//The Engine class which contains everything...

class Engine {
public:
	Engine();
	~Engine();

	void main_loop();
	void update_screen_slice(size_t start, size_t end, int pitch, void* pixels);

private:
	void handle_events();
	void render() const;
	void update_draw();
	void save_screenshot() const;

	bool running;
	double view_radius = 2;
	double x_off = 0.0;
	double y_off = 0.0;
	SDL_Texture* texture = nullptr;
	unsigned prev_time = 0;  //for calculating time-step
	size_t max_iter = 256;	//aka precision

	SDL_Color* pixel_table = nullptr; //lookup table for pixel coloring scheme
};

#endif // !ENGINE_HPP
