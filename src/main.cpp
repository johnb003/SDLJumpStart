#include "engine.h"
#include "sdl_gl.h"
#include "SDL.h"

int main(int argc, char *argv[])
{
	Engine *engine = NULL;
	try
	{
		engine = new Engine();
		getInfo();
		for(;;)
		{
			engine->Update();
			engine->Draw();
		}
	}
	catch (int code)
	{
		if (engine) delete engine;
		return code;
	}

	// never gets here (I'm pretty sure)
	return 0;
}
