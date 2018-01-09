#include "engine.h"
#include "font.h"

#include "sdl_gl.h"

// #define _USE_MATH_DEFINES 1
#include <math.h>
#include <iostream>


using namespace std;

static const char *APP_NAME = "SDLJumpStart";

//static float l0_position[] = {-1.0f, -0.3f, 1.0f, 0.0f};
static float l0_position[] = {0.0f, 0.0f, 1.0f, 0.0f};
static float l0_ambient[] =  {0.2f, 0.2f, 0.2f, 1.0f};

Engine::Engine()
{
	xr = 0;
	yr = 0;
	x = 0;
	y = 0.0f;
	z = 1.75f;
	dx = 0;
	dy = 0;
	dz = 0;
	objectYaw = 0.0f;
	objectPitch = 0.0f;
	dtMult = 1.0;
	zoom = 0.1f;

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
		throw(1);
	}

	width = 800;
	height = 600;
	flags = SDL_WINDOW_OPENGL;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	sdl_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if (sdl_window == NULL)
	{
		cerr << "Video mode set failed: " << SDL_GetError() << endl;
		throw(1);
	}

	sdl_gl_context = SDL_GL_CreateContext(sdl_window);
	SDL_GL_GetDrawableSize(sdl_window, &clientW, &clientH);
    
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDrawBuffer(GL_BACK);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.05f);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01f);
	glEnable(GL_LIGHT0);
	
	glViewport(0, 0, clientW, clientH);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective( 60.0, (float)clientW/(float)clientH, 0.00000000001, 1024.0 );
	
	font = new FixedWidthBMPFont("../images/font.bmp", 16);


	// TODO: More initialization calls ehre.


	last_time = SDL_GetTicks();
//    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
}

void Engine::Update()
{
	Events();
	Uint32 current_time = SDL_GetTicks();
	if (current_time == last_time) // damn we're fast
	{
		elapsedtime = 0.0;
		return;
	}
	
	elapsedtime = (double)(current_time - last_time) / 1000.0;
	last_time = current_time;

	if (elapsedtime > 0.1)
		elapsedtime = 0.1;

	// TEMP HACK - use a fixed timestep for now, even though this isn't linked to framerate.
	// Too lazy to do this properly, but want consistent simulation results because I was testing the stability of the integrator.
	elapsedtime = 0.016;

	// TODO: Make update calls here.


}

void DrawTestPrim()
{
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 0.0f, 0.0f, 1); glVertex3f(  0.0f, 0.5660f, 0.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1);	glVertex3f( -0.5f, -0.3f,    0.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1);	glVertex3f(  0.5f, -0.3f,    0.0f);
	glEnd(); 
}

void Engine::Draw()
{
// 	if (elapsedtime == 0.0f)
// 		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, clientW, clientH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)clientW/(float)clientH, 0.0001, 1024.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-yr, 1.0, 0.0, 0.0);
	glRotatef(-xr, 0.0, 1.0, 0.0);
	glTranslatef(-x, -y, -z);

	glLightfv(GL_LIGHT0, GL_AMBIENT, l0_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0_position);

	glRotatef(objectPitch, 1, 0, 0);
	glRotatef(objectYaw, 0, 1, 0);



	// TODO: Make draw calls here.

	// Sorta. -- this is old-school style. Better to evolve this later to an IM API that
	// generates primitives that are rendered with a proper opengl pipeline, instead of fixed.
	// Even for fixed pipeline, this style of everything having to draw itself is not great.

	// However from a quick and dirty demo point of view, it works quite well for every object to
	// just handle making immediate mode draw calls for itself.


	DrawTestPrim();



	// Setup 2D View - (0,0,xres,yres)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, clientW, clientH, 0.0f, -10, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// This hack is because GL primitives do not line up with the correct pixels.
	// This makes things pixel perfect.
	glTranslatef(0.375, 0.375, 0.0);

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);


	font->print(10, 10, "Hello, I'm a dorky barely usable font.");

	// TODO: Do your 2D drawing here, in pixels.
	// Top left corner is 0, 0.

	glPopAttrib();
	SDL_GL_SwapWindow(sdl_window);
}

Engine::~Engine()
{
	delete font;

 	SDL_GL_DeleteContext(sdl_gl_context);
 	SDL_DestroyWindow(sdl_window);

	SDL_Quit();
}

void Engine::HandleKey(SDL_Keycode key, bool down)
{
	switch(key)
	{
	case SDLK_w:
		if (down)
			dy += 1;
		else
			dy -= 1;
		break;
	case SDLK_a:
		if (down)
			dx -= 1;
		else
			dx += 1;
		break;
	case SDLK_s:
		if (down)
			dy -= 1;
		else
			dy += 1;
		break;
	case SDLK_d:
		if (down)
			dx += 1;
		else
			dx -= 1;
		break;
	case SDLK_q:
		if (down)
			dz -= 1;
		else
			dz += 1;
		break;
	case SDLK_e:
		if (down)
			dz += 1;
		else
			dz -= 1;
		break;
	// case SDLK_z:

	// 	break;
	case SDLK_KP_PLUS:
		if (down)
			dtMult *= 2;
		break;
	case SDLK_KP_MINUS:
		if (down)
			dtMult /= 2;
		break;
	case SDLK_ESCAPE:
		throw(0); 
		break;
	}
}

void Engine::Events()
{
	static bool mbuttondown1 = false;
	static bool mbuttondown2 = false;
	static bool mbuttondown3 = false;
	static bool modCtrl = false;
	static bool modAlt = false;
	static bool modGui = false;

	SDL_Event event;
	while( SDL_PollEvent(&event) )
	{
		switch( event.type )
		{
		case SDL_KEYDOWN:
			modCtrl = (event.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL;
			modAlt = (event.key.keysym.mod & KMOD_LALT) == KMOD_LALT;
			modGui = (event.key.keysym.mod & KMOD_LGUI) == KMOD_LGUI;
			HandleKey(event.key.keysym.sym, true);
			break;
		case SDL_KEYUP:
			modCtrl = (event.key.keysym.mod & KMOD_LCTRL) == KMOD_LCTRL;
			modAlt = (event.key.keysym.mod & KMOD_LALT) == KMOD_LALT;
			modGui = (event.key.keysym.mod & KMOD_LGUI) == KMOD_LGUI;
			HandleKey(event.key.keysym.sym, false);
			break;
   		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
				mbuttondown1 = true;
			else if (event.button.button == SDL_BUTTON_RIGHT)
				mbuttondown2 = true;
			else if (event.button.button == SDL_BUTTON_MIDDLE)
				mbuttondown3 = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				mbuttondown1 = false;
			if (event.button.button == SDL_BUTTON_RIGHT)
				mbuttondown2 = false;
			if (event.button.button == SDL_BUTTON_MIDDLE)
				mbuttondown3 = false;
            break;
		case SDL_MOUSEWHEEL:
		    if (event.wheel.y < 0)
				zoom *= 0.8f;
		    else
				zoom *= 1.25f;
			break;
		case SDL_MOUSEMOTION:
			if (mbuttondown2 || (mbuttondown1 && modAlt))
			{
				float delta = (float)(event.motion.xrel + event.motion.yrel)/10;
				z *= 1 - delta * elapsedtime;
				if (z < 0.0000001f)
					z = 0.0000001f;
			}
			else if (mbuttondown3 || (mbuttondown1 && modCtrl))
			{
				y += (event.motion.yrel)* z * elapsedtime * 0.1f;
				x -= (event.motion.xrel)* z * elapsedtime * 0.1f;
			}
			else if (mbuttondown1)
			{
				objectYaw += event.motion.xrel * elapsedtime * 10.0f;
				objectPitch += event.motion.yrel * elapsedtime * 10.0f;
			}
			break;
		case SDL_QUIT:
			throw(0); 
			break;
		}
    }
}
