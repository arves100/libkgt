/**
\file pch.h
\brief Precompiler headers
\author Arves100
\date 08/08/2022
\project libkgt
*/
#pragma once

#if defined(_DEBUG) && defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC 1
#endif

/// Enables experimental support for native GUI components (bypasses Nuklear UI)
#define KGT_NATIVE_UI 0

// C RUNTIME

#include <stdlib.h>
#if defined(_DEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// PLATFORM RUNTIME

#ifdef _WIN32

#ifndef _UNICODE
#error "UNICODE only support!"
#endif // _UNICODE

// Windows defs
#define COBJMACROS 1
#define WIN32_LEAN_AND_MEAN 1
#define STRICT 1

// Windows
#include <Windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <Psapi.h>
#include <winternl.h>

// DirectX9
#include <d3d9.h>

typedef NTSTATUS LONG;

#else


// X11
#include <X11/Xlib.h>

// UNIX runtime
#include <unistd.h>

// OpenGL
#include <GL/glx.h>
#include <GL/glxext.h>

#endif // _WIN32

#ifdef _MSC_VER
/// Inline
#define KGT_INLINE __inline

/// Force inline
#define KGT_FORCE_INLINE __forceinline

/// Weak function
#define KGT_WEAK 
#else
/// Inline
#define KGT_INLINE __attribute__((inline))

/// Force inline
#define KGT_FORCE_INLINE __attribute__((always_inline))

/// Weak function
#define KGT_WEAK __attribute__((weak))
#endif

// Nuklear configs

#define NK_INCLUDE_FIXED_TYPES 1
//#define NK_INCLUDE_STANDARD_IO 1
#define NK_INCLUDE_STANDARD_VARARGS 1
#define NK_INCLUDE_DEFAULT_ALLOCATOR 1
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT 1
#define NK_INCLUDE_FONT_BAKING 1
#define NK_INCLUDE_DEFAULT_FONT 1
