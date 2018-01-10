cmake_minimum_required(VERSION 3.4)

project(SDLJumpStart)

# Place to find the sdl script for "find_package"
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

# 'find_package' is meant to find SDL via an installed version on the system.
#
# If the find_package fails below (because SDL is not installed).
# You can grab SDL from the official source or (use the git mirror):
# https://github.com/SDL-mirror/SDL
# Clone it, build it:
#    mkdir build; cd build; cmake ..; make
#
# Then, uncomment the SDL2_PATH below and set to the appropriate path
# Tell the find_package script where we expect to find sdl that you built
# set(SDL2_PATH
#	${PROJECT_SOURCE_DIR}/third_party/sdl
#	${PROJECT_SOURCE_DIR}/third_party/sdl/build
#)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(SDLJumpStart
	src/AssertMsg.cpp
	src/sdl_gl_texture.cpp
	src/engine.cpp
	src/font.cpp
	src/main.cpp
	src/math_stub.cpp
)

# Do you need C++11? Maybe just indicate the features you need like this
# for better compatibility.
# target_compile_features(SDLJumpStart PRIVATE
#	cxx_auto_type
#	cxx_lambdas
#	cxx_range_for
#	cxx_constexpr)

target_include_directories(SDLJumpStart PUBLIC
	"${SDL2_INCLUDE_DIR}")

target_link_libraries(SDLJumpStart
	"${SDL2_LIBRARY}"
	"${OPENGL_LIBRARIES}")
