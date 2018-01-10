#ifndef MATH_STUB_H
#define MATH_STUB_H

// Check out the math library I wrote: https://github.com/johnb003/mathing
//
// There's not much point in forcing a reference to a math library for this project, however a math
// lib will inevitably be needed for any real work based on this.

// Here are some basic implementations that can be replaced easily with a math lib.

void BuildPerspProjMat(float *m, float fov, float aspect, float znear, float zfar);

// direct alternative to gluPerspective
void SetGLPerspectiveMat(float fov, float aspect, float znear, float zfar);

#endif  // MATH_STUB_H
