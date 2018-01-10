#include "engine.h"

#include "SDL.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	Engine *engine = NULL;
	try
	{
		engine = new Engine();

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
