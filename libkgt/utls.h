/**
\file utls.h
\brief Utility functions
\author Arves100
\date 08/08/2022
\project libkgt
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
\param str String to convert
\return Converted string or NULL
\note The returned buffer MUST BE MANUALLY FREED by the programmer

Converts an UTF-8 string into an UTF-16 string
*/
extern wchar_t* u_utf8_to_utf16(const char* str);

/**
\param str String to convert
\return Converted string or NULL
\note The returned buffer MUST BE MANUALLY FREED by the programmer

Converts an UTF-16 string into an UTF-8 string
*/
extern char* u_utf16_to_utf8(const wchar_t* str);

/**
\param window Window handle
\param url URL to open

Opens a new URL with the default browser
*/
extern void u_openlinkn(void* window, const char* url);

/**
\return Current ticks

Get the current ticks of the system
*/
extern uint64_t u_getticks(void);

#include "utls.inl" // inline

#ifdef __cplusplus
}
#endif
