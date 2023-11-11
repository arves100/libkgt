/**
\file ui_win32.c
\brief UI implementation for Windows NT
\author Arves100
\date 08/08/2022
\project libkgt
*/
#include "pch.h"
#include "ui.h"
#include "utls.h"
#include "nuklear_d3d9.h"

/**
\param hWnd Window handle
\param Msg Message ID
\param wParam First parameter
\param lParam Second parameter
\return Message result (0 means handled by this function)

Windows message procedure handler for the main UI
*/
static LRESULT WINAPI ui_wndproc(_In_ HWND hWnd, _In_ UINT Msg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	KUI* ui = (KUI*)GetWindowLongPtrW(hWnd, GWLP_USERDATA);

	switch (Msg)
	{
	case WM_CREATE:
	{ 
		// Set the userdata to KUI*
		CREATESTRUCTW* cs = (CREATESTRUCTW*)lParam;
		(void)SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
		return 0;
	}

	case WM_LBUTTONDOWN:
		// Move the window if we don't have a border
		if (ui->style & KUI_STYLE_NO_BORDER)
		{
			ui->firstPos.width = GET_X_LPARAM(lParam);
			ui->firstPos.height = GET_Y_LPARAM(lParam);
			ui->isMoving = true;
		}
		break;

	case WM_LBUTTONUP:
	case WM_KILLFOCUS:
		// Release the moving handle
		ui->isMoving = false;
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

#if EXP_NATIVE
	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;

		SetTextColor(hdc, ui->font_color);
		SetBkColor(hdc, RGB(0, 0, 0));

		return (LRESULT)ui->hFontColor;
	}
#endif

	default:
		break;
	}

	if (ui && !ui->no_gpu && nk_d3d9_handle_event(hWnd, Msg, wParam, lParam))
		return 0;

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}

/**
\param ui UI system
\param w New width
\param h New height

Fixes the UI window position based from the border sizes
*/
static void ui_fix_pos(KUI* ui, int w, int h)
{
	if (!(ui->style & KUI_STYLE_NO_BORDER))
	{
		RECT r, rw;

		(void)GetClientRect(ui->hMain, &r);
		(void)GetWindowRect(ui->hMain, &rw);

		w += (rw.right - rw.left) - r.right;
		h += (rw.bottom - rw.top) - r.bottom;
	}

	(void)SetWindowPos(ui->hMain, HWND_TOPMOST, GetSystemMetrics(SM_CXSCREEN) / 2 - w / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - h / 2, w, h, SWP_NOZORDER);
}

bool ui_create(KUI* ui, KUICREATEPARAM* params)
{
	WNDCLASSEXW wcex;
	wchar_t* title;
	DWORD hwFlags = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER;
	struct nk_font_atlas* atlas;

#if EXP_NATIVE
	wchar_t* wfont;
#endif

	if (!params)
		return false;

	if (params->style & KUI_STYLE_IMAGE_BG && !params->bg_img)
		return false;

	ui->style = params->style;
	ui->hInst = params->inst;
	ui->no_gpu = params->no_gpu;

	wcex.cbSize = sizeof(wcex);
	wcex.hInstance = ui->hInst;
	wcex.lpszClassName = L"KGT_APPE";
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = GetSysColorBrush(BLACK_BRUSH);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpfnWndProc = ui_wndproc;
	wcex.lpszMenuName = NULL;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	ui->aClass = RegisterClassExW(&wcex);

	if (!ui->aClass)
	{
		return false;
	}

	if (ui->style & KUI_STYLE_NO_BORDER)
		hwFlags = 0;

	title = u_utf8_to_utf16(params->title);
	if (!title)
		return false;

	ui->hMain = CreateWindowExW(0U, MAKEINTATOM(ui->aClass), title, WS_MINIMIZEBOX | WS_BORDER | WS_SYSMENU | WS_CAPTION, 0, 0, 0, 0, NULL, NULL, ui->hInst, (LPVOID)ui);
	free(title);

	if (!ui->hMain)
	{
		return false;
	}

	if (ui->style & KUI_STYLE_NO_BORDER)
	{
		hwFlags = GetWindowLong(ui->hMain, GWL_STYLE);
		hwFlags &= ~WS_SYSMENU & ~WS_CAPTION;
		(void)SetWindowLong(ui->hMain, GWL_STYLE, hwFlags);
	}

	(void)ShowWindow(ui->hMain, SW_HIDE);
	(void)UpdateWindow(ui->hMain);

	ui_fix_pos(ui, params->width, params->height);

	if (!params->no_gpu)
	{
		if (!gpu_init(ui))
			return false;

		ui->ctx = nk_d3d9_init(ui->gpu.dev, ui->gpu.pp.BackBufferWidth, ui->gpu.pp.BackBufferHeight);

		if (!ui->ctx)
			return false;

		nk_d3d9_font_stash_begin(&atlas);
		nk_d3d9_font_stash_end();
	}

#if EXP_NATIVE
	wfont = u_utf8_to_utf16(params->font_family);
	if (!wfont)
		return false;

	ui->hFont = CreateFontW(ui->font_size, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, wfont);
	free(wfont);

	if (!ui->hFont)
		return false;

	if (ui->style & KUI_STYLE_IMAGE_BG)
	{
		ui->hBackground = CreateWindowExW(0U, L"STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, 0, 0, 0, 0, ui->hMain, NULL, ui->hInst, NULL);

		if (!ui->hBackground)
			return false;

		SendMessage(ui->hBackground, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)img);
	}

	ui->hFontColor = CreateSolidBrush(ui->font_color);
	if (!ui->hFontColor)
		return false;
#endif

	return true;
}

#if EXP_NATIVE
bool ui_add_img(KUI* ui, int x, int y, int w, int h, void* img)
{
	if (ui->used_gui >= KUI_MAX_GUI_ELEMENTS)
		return false;

	ui->hElements[ui->used_gui] = CreateWindowExW(0U, L"STATIC", NULL, SS_BITMAP | WS_VISIBLE | WS_CHILD, x, y, w, h, ui->hMain, NULL, ui->hInst, NULL);
	if (!ui->hElements[ui->used_gui])
		return false;

	SendMessage(ui->hElements[ui->used_gui], STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)img);

	ui->used_gui++;
	
	return true;
}

bool ui_add_label(KUI* ui, int x, int y, const char* text)
{
	wchar_t* wtext = u_utf8_to_utf16(text);
	int wlen;

	if (!wtext)
		return false;

	wlen = (int)wcslen(wtext);

	ui->hElements[ui->used_gui] = CreateWindowExW(0U, L"STATIC", NULL, WS_VISIBLE | WS_CHILD, x, y, wlen * ui->font_size, ui->font_size, ui->hMain, NULL, ui->hInst, NULL);
	if (!ui->hElements[ui->used_gui])
	{
		free(wtext);
		return false;
	}

	SetWindowTextW(ui->hElements[ui->used_gui], wtext);
	free(wtext);

	SendMessage(ui->hElements[ui->used_gui], WM_SETFONT, (WPARAM)ui->hFont, (LPARAM)TRUE);

	ui->used_gui++;
	return true;
}

bool ui_add_edit(KUI* ui, int x, int y, int max, KUIEditCb cb)
{
	ui->hElements[ui->used_gui] = CreateWindowExW(0U, L"EDIT", NULL, WS_VISIBLE | WS_CHILD, x, y, max * ui->font_size, ui->font_size, ui->hMain, NULL, ui->hInst, NULL);
	if (!ui->hElements[ui->used_gui])
	{
		return false;
	}

	SendMessage(ui->hElements[ui->used_gui], WM_SETFONT, (WPARAM)ui->hFont, (LPARAM)TRUE);

	ui->used_gui++;
	return true;
}
#endif

void ui_free(KUI* ui)
{
	for (int i = 0; i < KUI_MAX_GUI_ELEMENTS; i++)
	{
		if (ui->hElements[i])
		{
			DestroyWindow(ui->hElements[i]->handle);
			free(ui->hElements[i]);
		}
	}

	if (ui->hBackground)
		DestroyWindow(ui->hBackground);

	if (ui->hFont)
		DeleteObject(ui->hFont);

	if (!ui->no_gpu)
	{
		nk_d3d9_shutdown();
		gpu_delete(&ui->gpu);
	}

	if (ui->hMain)
		(void)DestroyWindow(ui->hMain);

	if (ui->aClass)
		(void)UnregisterClassW(MAKEINTATOM(ui->aClass), ui->hInst);

	memset(ui->hElements, 0, sizeof(ui->hElements));
	ui->hBackground = NULL;
	ui->hFont = NULL;
	ui->hInst = NULL;
	ui->hMain = NULL;
	ui->aClass = 0;
}

bool ui_change_title(KUI* ui, const char* title)
{
	wchar_t* wtitle = u_utf8_to_utf16(title);

	if (!wtitle)
		return false;

	(void)SetWindowTextW(ui->hMain, wtitle);
	free(wtitle);
	return true;
}

bool ui_loop(KUI* ui)
{
	MSG msg = { 0 };

	// nuklear input
	if (!ui->no_gpu)
		nk_input_begin(ui->ctx);

	if (GetMessage(&msg, NULL, 0, 0))
	{
		(void)TranslateMessage(&msg);
		(void)DispatchMessageW(&msg);
	}

	if (!ui->no_gpu)
		nk_input_end(ui->ctx);

	if (ui->isMoving) // move the window manually if we have no borders
	{
		POINT p;
		if (GetCursorPos(&p))
		{
			(void)SetWindowPos(ui->hMain, NULL, p.x - ui->firstPos.width, p.y - ui->firstPos.height, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
	}

	return msg.message != WM_QUIT;
}

void ui_instant_show(KUI* ui)
{
	(void)ShowWindow(ui->hMain, SW_SHOW);
}

bool ui_getsize(KUI* ui, KUISIZE* sz)
{
	RECT r;

	if (!GetWindowRect(ui->hMain, &r))
	{
		return false;
	}

	sz->width = r.right - r.left;
	sz->height = r.bottom - r.top;
	return true;
}

void ui_render(KUI* ui)
{
	if (ui->no_gpu)
		return;

	nk_d3d9_render(NK_ANTI_ALIASING_ON);
}
