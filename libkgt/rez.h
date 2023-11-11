/**
\file rez.h
\brief Internal resource manager
\author Arves100
\date 09/08/2022
\project libkgt
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
/// Defines the type of a resource instance that can be accessed
typedef HINSTANCE KREZINSTANCE;
#endif

/**
A resource that is a bitmap
*/
typedef struct KBITMAP
{
#ifdef _WIN32
	/// Bitmap native handle
	HBITMAP handle;
#endif

	/// Bitmap width
	int width;

	/// Bitmap height
	int height;

	/// Bytes per pixel
	int bpp;
} KBITMAP;

/**
\param inst Resource instance
\param id Resource ID
\param bmp Output bitmap
\return true in case of loading succeeded, otherwise false

Loads a bitmap from the application resources
*/
extern bool rez_getbitmap(KREZINSTANCE inst, uint32_t id, KBITMAP* bmp);

/**
\param bmp Bitmap to free

Frees the allocated memory of a resource bitmap
*/
extern void rez_freebitmap(KBITMAP* bmp);

#ifdef __cplusplus
}
#endif
