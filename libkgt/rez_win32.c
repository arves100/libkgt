/**
\file rez_win32.c
\brief Internal resource manager for Windows NT
\author Arves100
\date 09/08/2022
\project libkgt
*/
#include "pch.h"
#include "rez.h"

void rez_freebitmap(KBITMAP* bmp)
{
	if (bmp->handle)
		DeleteObject(bmp->handle);

	bmp->handle = NULL;
}

bool rez_getbitmap(KREZINSTANCE inst, uint32_t id, KBITMAP* bmp)
{
	BITMAP gdi;

	bmp->handle = LoadBitmapW(inst, MAKEINTRESOURCEW(id));
	if (!bmp->handle)
		return false;

	GetObjectW(bmp->handle, sizeof(gdi), &gdi);
	bmp->width = gdi.bmWidth;
	bmp->height = gdi.bmHeight;
	bmp->bpp = gdi.bmBitsPixel;

	return true;
}
