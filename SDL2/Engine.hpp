#ifndef ENGINE_HPP
#define ENGINE_HPP

struct SDL_Texture;
struct SDL_Color;


class Engine {
public:
	Engine();
	~Engine();

	void main_loop();

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
	unsigned prev_time = 0;
	size_t iter_count = 256;

	SDL_Color* pixel_table = nullptr;
};

#endif // !ENGINE_HPP
