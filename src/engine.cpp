#include "engine.h"
#include "font.h"
#include "AssertMsg.h"

#include "sdl_gl.h"

// #define _USE_MATH_DEFINES 1
#include <math.h>
#include <iostream>

using namespace std;

static const char *APP_NAME = "SDLJumpStart";
static int INITIAL_WIDTH = 800;
static int INITIAL_HEIGHT = 600;

static float l0_position[] = {0.0f, 0.0f, 1.0f, 0.0f};
static float l0_ambient[] =  {0.2f, 0.2f, 0.2f, 1.0f};

// Work around since the poll event doesn't update continuously.
// This can happen on another thread, unlike when polling the event.
static int EventWatcher(void *data, SDL_Event *event)
{
	reinterpret_cast<Engine *>(data)->HandleEventImmediate(*event);
	return 0;
}

Engine::Engine() : font(NULL), sdl_gl_context(NULL), sdl_window(NULL)
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
	flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	sdl_window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if (sdl_window == NULL)
	{
		cerr << "Video mode set failed: " << SDL_GetError() << endl;
		throw(1);
	}

	// Polling for window resize events do not seem to work while actively resizing.
	// Therefore we need this listener.
	SDL_AddEventWatch(EventWatcher, this);

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
	
	font = new FixedWidthBMPFont("images/font.bmp", 16);

	// TODO: Your initialization calls here.
	// subsystem = new SubSystem();

	last_time = SDL_GetTicks();
//    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
}

Engine::~Engine()
{
	// TODO: Your cleanup calls here.
	// if (subsystem) delete subsystem;

	if (font) delete font;

	if (sdl_gl_context) SDL_GL_DeleteContext(sdl_gl_context);
	if (sdl_window) SDL_DestroyWindow(sdl_window);

	SDL_Quit();
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

	// If you're in the debugger or there's some issue which causes a frame to get massively
	// delayed, we don't want to update the game by a huge timestep, so we'll cap it.
	if (elapsedtime > 0.1)
		elapsedtime = 0.1;

	// Sometimes the frame runs faster than the timer resolution, so, if you have
	// code that can't handle 0 time updates, you may want to skip the update.
// 	if (elapsedtime == 0.0f)
// 		return;

	// TODO: Make update calls here.
	// subsystem->Update(elapsedtime);
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
	// subsystem->Draw();

	// Sorta. -- this is old-school style. Better to evolve this later to an IM API that
	// generates primitives that are rendered with a proper opengl pipeline, instead of fixed.
	// However from a quick and dirty demo point of view, it works quite well for every object to
	// just handle making immediate mode draw calls for itself.

	// Draw something for now, so we can tell this is working.
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
	// subsystem->Draw2D();

	glPopAttrib();
	SDL_GL_SwapWindow(sdl_window);
}

void Engine::HandleKey(SDL_Keycode key, bool down)
{
	// TODO: Call functions in your subsystem based on the key events here.
	// Note how the engine keeps track of state in dx, dy, dz for the camera.
	switch(key)
	{
	// Example:
	// case SDLK_z:
	// 	if (down) subsystem->ZPressed();
	// 	break;
	case SDLK_w:
		dy += down? 1 : -1;
		break;
	case SDLK_a:
		dx += down? -1 : 1;
		break;
	case SDLK_s:
		dy += down? -1 : 1;
		break;
	case SDLK_d:
		dx += down? 1 : -1;
		break;
	case SDLK_q:
		dz += down? -1 : 1;
		break;
	case SDLK_e:
		dz += down? 1 : -1;
		break;
	case SDLK_KP_PLUS:
		if (down) dtMult *= 2;
		break;
	case SDLK_KP_MINUS:
		if (down) dtMult /= 2;
		break;
	case SDLK_ESCAPE:
		throw(0); 	// Terminate the application with success code.
		break;
	}
}

void Engine::ResizeWindow(int newWidth, int newHeight) {
	width = newWidth;
	height = newHeight;
	SDL_GL_GetDrawableSize(sdl_window, &clientW, &clientH);
}

void Engine::HandleEventImmediate(const SDL_Event &event) {
	if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
	{
		SDL_Window *window = SDL_GetWindowFromID(event.window.windowID);
		if (window == sdl_window)
			ResizeWindow(event.window.data1, event.window.data2);

		// HACK: This is an ugly non-scalable hack.
		// The problem is that during resize, the event handler blocks, which in turn blocks the whole game loop.
		// We're only getting here due to the event listener, but we're not guaranteed to be on the main thread.
		// Calling Draw here, will at least let us see content, even though it's not updating.
		// BUT BEWARE: If you're draw code is not thread-safe this could cause big problems (hence not scalable).
		//
		// Some ideas:
		// 1) Find a way to tell SDL or the OS not to block during resize.
		// 2) Run the game loop and event handler on different threads and pass the events along.
		Draw();
	}
}

void Engine::HandleWindowEvents(const SDL_Event &event) {
	ASSERT_MSG(event.type == SDL_WINDOWEVENT, "This isn't a window event!");
	
	switch (event.window.event) {
	// NOTE: These don't fire while dragged only when done.
	case SDL_WINDOWEVENT_RESIZED:
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		{
			SDL_Window *window = SDL_GetWindowFromID(event.window.windowID);
			if (window == sdl_window)
				ResizeWindow(event.window.data1, event.window.data2);
			break;
		}
	}
}

void Engine::HandleInputEvents(const SDL_Event &event) {
	// There's also Joystick, Controller and touch events not shown here.
	// See: https://wiki.libsdl.org/SDL_EventType

	static bool mbuttondown1 = false;
	static bool mbuttondown2 = false;
	static bool mbuttondown3 = false;
	static bool modCtrl = false;
	static bool modAlt = false;
	static bool modGui = false;

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
	}
}

void Engine::Events()
{
	SDL_Event event;
	while( SDL_PollEvent(&event) )
	{
		// There's a lot of events that can or ought to be hanleded here.
		// See: https://wiki.libsdl.org/SDL_EventType
		//
		// Application events:
		//    forground, background, mem warning, and quit
		// Window events:
		//    min/max-imize, focus, resize, show/hide
		// Keyboard events (example provided):
		//    Key down/up, IME text input systems (not shown)
		// Mouse events:
		//    Buttons pressed/released, mouse movement, scrool wheel
		// Joystick & Controller events.
		// Touchpad & Multi-touch events
		// Clipboard events
		// Drag and Drop (like files from the system)
		// Render notifications (like device context and targets lost)
		switch( event.type )
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
   		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
		case SDL_MOUSEMOTION:
			HandleInputEvents(event);
			break;
		case SDL_WINDOWEVENT:
			HandleWindowEvents(event);
			break;
		case SDL_QUIT:
			throw(0); 	// Terminate the application with success code.
			break;
		}
    }
}
