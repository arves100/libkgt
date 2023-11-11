/**
\file gpu.h
\brief Small direct GPU access
\author Arves100
\date 09/08/2022
\project libkgt
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
Structure that contains all GPU accelerated operations
*/
typedef struct KGPU
{
#ifdef _WIN32
	/// Direct3D9 DLL
	HMODULE dll;

	/// Direct3D9 device
	IDirect3DDevice9* dev;

	/// Display presentation parameters
	D3DPRESENT_PARAMETERS pp;

	/// If using Direct3D9 Extended mode
	bool ex;
#endif // _WIN32
} KGPU;

/**
\param	ui	UI to use for initialization
\return true in case of init succeded, otherwise false

Initializes the GPU render with the provided UI
*/
bool gpu_init(struct KUI* ui);

/**
\param	width	Width of the offscreen render
\param	height	Height of the offscreen render
\param	gpu		GPU system to initialize
\return true in case of init succeded, otherwise false

Initializes the GPU render as an offscreen renderer
*/
bool gpu_init_offscreen(int width, int height, KGPU* gpu);

/**
\param	gpu	GPU system to delete

Frees the memory allocated by the GPU system
*/
void gpu_delete(KGPU* gpu);

/**
\param	gpu	GPU system
\param	bg	Clear color

Starts a rendering operation
*/
void gpu_begin(KGPU* gpu, struct nk_colorf bg);

/**
\param	gpu	GPU system

Ends a rendering operation
*/
void gpu_present(KGPU* gpu);

#ifdef __cplusplus
}
#endif
