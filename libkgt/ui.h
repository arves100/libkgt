/**
\file ui.h
\brief Small native UI system
\author Arves100
\date 08/08/2022
\project libkgt
*/
#pragma once

#include "nuklear.h" // C++ import fix

#ifdef __cplusplus
extern "C" {
#endif

#include "rez.h"
#include "gpu.h"

/// Max numbers of GUI elemtns allowed
#define KUI_MAX_GUI_ELEMENTS 0x10

/// Max default buffer size
#define KUI_MAX_BUFFER 100

/// Default max memory size
#define KUI_NK_MEMORY_SIZE 16*1024

#ifdef _WIN32
/// Native UI instance
typedef HINSTANCE KUIINSTANCE;
#endif

/**
Types of UI styles
*/
enum KUIStyle
{
	/// The window must have an internal image as a background
	KUI_STYLE_IMAGE_BG = 1 << 0,

	/// The window does not have a border
	KUI_STYLE_NO_BORDER = 1 << 1,
};

/**
IDs of the native elements
*/
enum KUIElementsID
{
	/// Reserved
	KUI_ID_NONE = 0,

	/// The element is a label
	KUI_ID_LABEL = 1,

	/// The element is an editbox
	KUI_ID_EDITBOX = 2,

	/// The element is an image
	KUI_ID_IMAGE = 3,
};

/**
An UI element
*/
struct KUIELEM
{

#ifdef _WIN32
	/// Element handle
	HWND handle;
#endif

	/// Element Type
	uint8_t type;
};

/**
Simple structure for containing the size of an element
*/
typedef struct KUISIZE
{
	/// Width of the element
	int width;

	/// Height of the element
	int height;
} KUISIZE;

/**
Main container of an application UI
*/
typedef struct KUI
{
	/// App style
	uint32_t style;

	/// Initial movement position
	KUISIZE firstPos;

	/// Number of used GUI elements
	uint32_t used_gui;

	/// GPU context
	KGPU gpu;

	/// Check if the gui window is moving (only used when the style is without borders)
	bool isMoving;

	/// Check if we can or canot render Nuklear
	bool no_gpu;

	/// Nuklear context
	struct nk_context* ctx;

#ifdef _WIN32
	/// WIN32 App instance
	HINSTANCE hInst;

	/// GUI Class
	ATOM aClass;

	/// Main window handle
	HWND hMain;

	/// Background handle
	HWND hBackground;

	/// Elements handlers
	struct KUIELEM* hElements[KUI_MAX_GUI_ELEMENTS];

	/// Default font handle
	HFONT hFont;

	/// Default font brush color
	HBRUSH hFontColor;
#endif

} KUI;

/**
UI creation parameters (required for creating a new UI)
*/
typedef struct KUICREATEPARAM
{
	/// UI intance
	KUIINSTANCE inst;

	/// App title
	char title[KUI_MAX_BUFFER];

	/// App style
	uint32_t style;

	/// Do not create a GPU context
	bool no_gpu;

	/// Window width
	int width;

	/// Window height
	int height;

	/// Background image
	KBITMAP* bg_img;

#if EXP_NATIVE
	/// Default font family name
	char font_family[KUI_MAX_BUFFER];

	/// Default font size
	uint32_t font_size;

	/// Font foreground color
	uint32_t font_fg_color;

	/// Font background color
	uint32_t font_bg_color;
#endif
} KUICREATEPARAM;

/**
\param ui UI system

Frees the memory allocated for the UI
*/
extern void ui_free(KUI* ui);

/**
\param ui UI system
\param params Creation parameters

Creates a new window UI
*/
extern bool ui_create(KUI* ui, KUICREATEPARAM* params);

extern bool ui_change_title(KUI* ui, const char* title);

/**
\param ui UI system
\return true if the application should continue processing UI message, otherwise false

Processes the UI system messages
*/
extern bool ui_loop(KUI* ui);

/**
\param ui UI system

Istantaneously show the UI
*/
extern void ui_instant_show(KUI* ui);

/**
\param ui UI system
\param sz Output size
\return true in case the get was successfull, otherwise false

Gets the size of the UI
*/
extern bool ui_getsize(KUI* ui, KUISIZE* sz);

/**
\param ui UI system
\note Must be called after the GPU begin scene and before the GPU end scene

Renders the GPU-based GUI components (Nuklear)
*/
extern void ui_render(KUI* ui);

#if EXP_NATIVE // TODO
typedef void(*KUIEditCb)(const char* text);

extern bool ui_add_label(KUI* ui, int x, int y, const char* text);
extern bool ui_add_edit(KUI* ui, int x, int y, int max, KUIEditCb cb);
extern bool ui_add_img(KUI* ui, int x, int y, int w, int h, void* img);
#endif

#ifdef __cplusplus
}
#endif
