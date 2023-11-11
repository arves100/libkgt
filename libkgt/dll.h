/**
\file dll.h
\brief Shared module utility
\author Arves100
\date 04/09/2023
\project libkgt
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
Structure that contains all the information of a shared module
*/
typedef struct KDLL
{
	/// shared library path
	char path[MAX_PATH + 1];

#ifdef _WIN32
	/// Handle to the DLL
	HMODULE hm;
#endif
} KDLL;

/**
KDLL load flags
*/
enum KDLL_LOADFLAGS
{
	/// Does not resolve the dependency of the DLL
	KDLL_LOAD_NO_RESOLVE = 1 << 0,

	/// Does not load the DLL at all, just to have the info loaded
	KDLL_LOAD_NO_ENTRYPOINT = 1 << 1,
};

/**
\param dll DLL input structure
\param flags Load flags
\return true if the module was loaded, otherwise false

Loads a shared module into memory
*/
extern bool dll_load(KDLL* dll, uint8_t flags);

/**
\param dll DLL structure

Frees the loaded shared module
*/
extern void dll_free(KDLL* dll);

/**
\param dll DLL structure
\param name Exported function name
\return pointer to the exported function or NULL if it does not exists

Gets the exported function from a shared module
*/
extern void* dll_getproc(KDLL* dll, const char* name);

#ifdef __cplusplus
}
#endif
