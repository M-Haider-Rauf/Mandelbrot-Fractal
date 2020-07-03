#ifndef RENDERCONTEXT_HPP
#define RENDERCONTEXT_HPP

//I made the renderer and window as globals because there will be never 2 of them anyway
//and I need convenient access to them in a lot of SDL2 funcs...

struct SDL_Renderer;
struct SDL_Window;

extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;

#endif // !RENDERCONTEXT_HPP
