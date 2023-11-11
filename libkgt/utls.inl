/**
\file utls.inl
\brief Inline Utility functions
\author Arves100
\date 10/08/2022
\project libkgt
*/
#pragma once

#ifdef _WIN32
/**
\return The current module instance

Gets the native instance
*/
static KGT_FORCE_INLINE HINSTANCE u_getinstance(void)
{
	return GetModuleHandleW(NULL);
}

/**
\param millis Milliseconds to sleep

Sleeps for some milliseconds
*/
static KGT_FORCE_INLINE void u_sleep(uint32_t millis)
{
	Sleep(millis);
}
#endif // _WIN32

/**
\param url URL to open

Opens a new URL with the default browser
*/
static KGT_FORCE_INLINE void u_openlink(const char* url)
{
	u_openlinkn(NULL, url);
}
