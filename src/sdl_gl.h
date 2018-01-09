#ifndef SDL_OPEN_GL_H
#define SDL_OPEN_GL_H

#include <SDL.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _WIN32
#include <windows.h>
#endif  // _WIN32

#include <GL/gl.h>
#include <GL/glu.h>
#endif  // Anything other than __APPLE__

void sdl_application_abort( const char *);
void getInfo(void);
void getTextureInfo( SDL_Texture *);

#endif  /* SDL_OPENGL_H */

