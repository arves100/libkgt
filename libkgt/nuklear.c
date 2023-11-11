/**
\file nuklear.c
\brief Simple importer of Nuklear-UI
\author Arves100
\date 09/08/2022
*/

// nuklear
#define NK_IMPLEMENTATION 1

// nuklear dx9
#define NK_D3D9_IMPLEMENTATION 1

// nuklear ogl2.0 xlib
#define NK_XLIB_GL2_IMPLEMENTATION 1

#include "pch.h"

#undef free
#undef malloc

#include "nuklear.h"

#ifdef _WIN32
#include "nuklear_d3d9.h"
#else
#include "nuklear_xlib_gl2.h"
#endif
