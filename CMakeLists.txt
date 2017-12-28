cmake_minimum_required(VERSION 3.4)

project(SDLJumpStart)

# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long")

# includes cmake/FindSDL2.cmake
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(SDL2_PATH ${PROJECT_SOURCE_DIR}/third_party/sdl ${PROJECT_SOURCE_DIR}/third_party/sdl/make)
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

# find_package(GLEW REQUIRED)


# target_include_directories(SDLJumpStart
# 	${OPENGL_INCLUDE_DIR}
# 	${GLEW_INCLUDE_DIRS})


# include_directories("${SDL2_INCLUDE_DIR}")

add_executable(SDLJumpStart
	src/AssertMsg.cpp
	src/sdl_gl_texture.cpp
	src/engine.cpp
	src/font.cpp
	src/main.cpp
)

target_include_directories(SDLJumpStart PUBLIC
	"${SDL2_INCLUDE_DIR}")

# project, [libs, ...]
# ok to call multiple times on a single target.
# PROJECT PRIVATE/PUBLIC/??? [LIB, ...]

target_link_libraries(SDLJumpStart
	"${SDL2_LIBRARY}"
	"${OPENGL_LIBRARIES}")
