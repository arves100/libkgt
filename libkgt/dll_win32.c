/**
\file dll.c
\brief Shared module utilities for NT
\author Arves100
\date 04/09/2023
\project libkgt
*/
#include "pch.h"
#include "dll.h"
#include "utls.h"

void* dll_getproc(KDLL* dll, const char* name)
{
	return GetProcAddress(dll->hm, name);
}

void dll_free(KDLL* dll)
{
	FreeLibrary(dll->hm);
}

bool dll_load(KDLL* dll, uint8_t flags)
{
	DWORD dwFlags = 0;
	wchar_t* path = u_utf8_to_utf16(dll->path);
	if (!path)
		return false;

	if (flags & KDLL_LOAD_NO_RESOLVE)
		dwFlags |= DONT_RESOLVE_DLL_REFERENCES;
	if (flags & KDLL_LOAD_NO_ENTRYPOINT)
		dwFlags |= LOAD_LIBRARY_AS_IMAGE_RESOURCE;

	dll->hm = LoadLibraryExW(path, NULL, dwFlags);

	free(path);

	return dll->hm != NULL;
}
