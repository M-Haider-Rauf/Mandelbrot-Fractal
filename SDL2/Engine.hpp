#ifndef ENGINE_HPP
#define ENGINE_HPP

struct SDL_Texture;
struct SDL_Color;

//The Engine class which contains everything...

class Engine {
public:
	~Engine();

	void main_loop();

	//update a *portion* of screen pixels
	//update the pixel columns [start, end) 
	//This is required for multi-threading so 
	//each thread can update a portion of screen
	void update_screen_slice(size_t start, size_t end, int pitch, void* pixels);

	//singleton pattern
	static Engine* get_instance()
	{
		static Engine engine;
		return &engine;
	}

private:
	//private ctor because of singleton pattern..
	Engine();

	void handle_events();
	void render() const;

	//calculate mandelbrot iterations for each pixel (i.e update screen pixels)
	void update_draw();
	void save_screenshot() const;

	bool running;
	double view_radius = 2.0;

	//pan offsets
	double x_off = 0.0; 
	double y_off = 0.0;

	SDL_Texture* texture = nullptr; //The texture covering whole screen
	unsigned prev_time = 0;  //for calculating time-step
	size_t max_iter = 256;	//aka precision

	SDL_Color* pixel_table = nullptr; //lookup table for pixel coloring scheme
};

#endif // !ENGINE_HPP
