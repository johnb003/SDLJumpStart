#ifndef ENGINE_H
#define ENGINE_H

#include "SDL.h"
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

	SDL_Window *sdl_window;
	SDL_GLContext sdl_gl_context;

	float zoom;

	int dx,dy,dz;

	void HandleKey(SDL_Keycode key, bool down);
	void Events();
	Uint32 last_time; 

public:
	Engine();
	~Engine();

	void Update();
	void Draw();
};


#endif // ENGINE_H
