# SDLJumpStart
Basic structure of an engine, suitable as a starting point to develop a realtime application.

## Dependencies
C++98 compatible.
OpenGL
SDL2  (this is the only one you need to download.)

## Build
Run cmake from a temporary directory like this:

```
mkdir build
cd build
cmake ..
make
cd ..
```

## Run
Run the binary. If successful an app with an OpenGL window should show.

```
build/SDLJumpStart
```

## Notes
This is really minimal barebones stuff, often designed for using the completely deprecated opengl fixed function pipeline,
just because it's easy and convenient and the rendering quality is not important for many of the apps I often build.

In the future, I'll probably build a cleaner Fixed Function-like alternative, that has an immediate mode API to build primitives,
and a standard shader for rendering those primitives.

