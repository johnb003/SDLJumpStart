#include "math_stub.h"
#include "sdl_gl.h"

#include <cmath>

#define PI_OVER_360 (M_PI/360.0f)

void BuildPerspProjMat(float *m, float fov, float aspect, float znear, float zfar)
{
    float f = 1.0f / tan(fov * PI_OVER_360);

    m[0]  = f / aspect;
    m[1]  = 0;
    m[2]  = 0;
    m[3]  = 0;

    m[4]  = 0;
    m[5]  = f;
    m[6]  = 0;
    m[7]  = 0;

    m[8]  = 0;
    m[9]  = 0;
    m[10] = (zfar + znear) / (znear - zfar);
    m[11] = -1;

    m[12] = 0;
    m[13] = 0;
    m[14] = 2 * zfar * znear / (znear - zfar);
    m[15] = 0;
}

void SetGLPerspectiveMat(float fov, float aspect, float znear, float zfar)
{
    float m[16];
    BuildPerspProjMat(m, fov, aspect, znear, zfar);
    glLoadMatrixf(m);
}
