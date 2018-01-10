#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
class IFont;

class Engine
{
	int width, height, flags;
	int clientW, clientH;
	float xr,yr;
	float x,y,z;
	float elapsedtime;
	float objectYaw;
	float objectPitch;
	double dtMult;

	IFont *font;

	SDL_GLContext sdl_gl_context;
	SDL_Window *sdl_window;

	float zoom;
	int dx, dy, dz;		// input deltas
	Uint32 last_time; 
	
	void ResizeWindow(int newWidth, int newHeight);
	void HandleWindowEvents(const SDL_Event &event);
	void HandleKey(SDL_Keycode key, bool down);
	void HandleInputEvents(const SDL_Event &event);
	void Events();

public:
	Engine();
	~Engine();

	void Update();
	void Draw();
	void HandleEventImmediate(const SDL_Event &event);
};


#endif // ENGINE_H
