/**
\file gpu_d3d9.c
\brief Direct3D9 GPU access
\author Arves100
\date 09/08/2022
*/
#include "pch.h"
#include "gpu.h"
#include "ui.h"

typedef HRESULT WINAPI Direct3DCreate9ExPtr(UINT, IDirect3D9Ex**);
typedef HRESULT WINAPI Direct3DCreate9Ptr(UINT, IDirect3D9**);

/**
\param	width	Width of the backbuffer
\param	height	Height of the backbuffer
\param	hwnd	Window handle
\param	gpu		GPU system
\return true in case of init succeeded, otherwise false

Real function that initializes the GPU system
*/
static bool gpu_init_real(int width, int height, HWND hwnd, KGPU* gpu)
{
	HRESULT hr;
	IDirect3D9Ex* d3d9ex = NULL;
	IDirect3D9* d3d9 = NULL;

	memset(&gpu->pp, 0, sizeof(gpu->pp));

	gpu->pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	gpu->pp.BackBufferWidth = width;
	gpu->pp.BackBufferHeight = height;
	gpu->pp.BackBufferCount = 1;
	gpu->pp.BackBufferFormat = D3DFMT_X8R8G8B8;
	gpu->pp.MultiSampleType = D3DMULTISAMPLE_NONE;
	gpu->pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	gpu->pp.EnableAutoDepthStencil = TRUE;
	gpu->pp.AutoDepthStencilFormat = D3DFMT_D24S8;
	gpu->pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	gpu->pp.Windowed = TRUE;
	gpu->pp.hDeviceWindow = hwnd;

	gpu->dll = LoadLibraryW(L"d3d9.dll");

	if (!gpu->dll)
		return false;

	Direct3DCreate9ExPtr* Direct3DCreate9Ex = (Direct3DCreate9ExPtr*)GetProcAddress(gpu->dll, "Direct3DCreate9Ex");

	if (!Direct3DCreate9Ex)
	{
		// System does not support D3D9Ex (Windows Vista or lesser)

		Direct3DCreate9Ptr* Direct3DCreate9 = (Direct3DCreate9Ptr*)GetProcAddress(gpu->dll, "Direct3DCreate9");
		if (!Direct3DCreate9)
		{
			FreeLibrary(gpu->dll);
			return false;
		}

		hr = Direct3DCreate9(D3D_SDK_VERSION, &d3d9);

		if (FAILED(hr))
		{
			FreeLibrary(gpu->dll);
			return false;
		}

		gpu->ex = false;
	}
	else
	{
		// System does support D3D9Ex (Windows 7+)

		hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9ex);

		if (FAILED(hr))
		{
			FreeLibrary(gpu->dll);
			return false;
		}

		gpu->ex = true;
	}

	if (gpu->ex)
	{
		// Extended device creation

		IDirect3DDevice9Ex* devex;

		hr = IDirect3D9Ex_CreateDeviceEx(d3d9ex, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_DISABLE_PRINTSCREEN | D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE, &gpu->pp, NULL, &devex);

		if (FAILED(hr))
		{
			// Software render
			hr = IDirect3D9Ex_CreateDeviceEx(d3d9ex, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_DISABLE_PRINTSCREEN | D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE, &gpu->pp, NULL, &devex);
		}

		gpu->dev = (IDirect3DDevice9*)devex;

		IDirect3D9Ex_Release(d3d9ex);
	}
	else
	{
		// Normal device creation

		hr = IDirect3D9_CreateDevice(d3d9, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE, &gpu->pp, &gpu->dev);

		if (FAILED(hr))
		{
			// Software render
			hr = IDirect3D9_CreateDevice(d3d9, D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE | D3DCREATE_FPU_PRESERVE, &gpu->pp, &gpu->dev);
		}

		IDirect3D9_Release(d3d9);
	}

	
	if (FAILED(hr))
	{
		FreeLibrary(gpu->dll);
		return false;
	}

	return true;
}

bool gpu_init_offscreen(int width, int height, KGPU* gpu)
{
	return gpu_init_real(width, height, NULL, gpu);
}

bool gpu_init(struct KUI* ui)
{
	KUISIZE sz;

	if (!ui_getsize(ui, &sz))
		return false;

	return gpu_init_real(sz.width, sz.height, ui->hMain, &ui->gpu);
}

void gpu_delete(KGPU* gpu)
{
	if (gpu->dev)
		IDirect3DDevice9Ex_Release(gpu->dev);

	if (gpu->dll)
		FreeLibrary(gpu->dll);

	gpu->dll = NULL;
	gpu->dev = NULL;
}

void gpu_present(KGPU* gpu)
{
	IDirect3DDevice9Ex_EndScene(gpu->dev);
	IDirect3DDevice9Ex_Present(gpu->dev, NULL, NULL, NULL, 0);
}

void gpu_begin(KGPU* gpu, struct nk_colorf bg)
{
	IDirect3DDevice9_Clear(gpu->dev, 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(bg.r, bg.g, bg.b, bg.a), 0.0f, 0);
	IDirect3DDevice9_BeginScene(gpu->dev);
}
