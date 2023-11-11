/**
\file utls.c
\brief Utility functions
\author Arves100
\date 09/08/2022
\project libkgt
*/
#include "pch.h"
#include "utls.h"

#ifdef _WIN32
void u_openlinkn(void* window, const char* url)
{
	wchar_t* wurl = u_utf8_to_utf16(url);
	if (!wurl)
		return;

	(void)ShellExecuteW((HWND)window, L"open", wurl, NULL, NULL, SW_SHOWNORMAL);
	free(wurl);
}

wchar_t* u_utf8_to_utf16(const char* str)
{
	int len;
	wchar_t* buffer = NULL;
	int wlen;

	if (!str || str[0] == '\0')
		return NULL;

	len = (int)strlen(str);
	wlen = MultiByteToWideChar(CP_UTF8, 0, str, len, NULL, 0);

	buffer = (wchar_t*)malloc((wlen + 1) * sizeof(wchar_t));
	if (!buffer)
		return NULL;

	if (MultiByteToWideChar(CP_UTF8, 0, str, len, buffer, wlen) != wlen)
	{
		free(buffer);
		return NULL;
	}

	buffer[wlen] = L'\0';
	return buffer;
}

char* u_utf16_to_utf8(const wchar_t* str)
{
	int len;
	char* buffer = NULL;
	int wlen;
	BOOL uc = FALSE;
	CHAR dc = '?';

	if (!str || str[0] == L'\0')
		return NULL;

	wlen = (int)wcslen(str);
	len = WideCharToMultiByte(CP_UTF8, 0, str, wlen, NULL, 0, &dc, &uc);

	buffer = (char*)malloc(len + 1);
	if (!buffer)
		return NULL;

	if (WideCharToMultiByte(CP_UTF8, 0, str, wlen, buffer, len, &dc, &uc) != len)
	{
		free(buffer);
		return NULL;
	}

	buffer[len] = '\0';
	return buffer;
}

uint64_t u_getticks(void)
{
	return GetTickCount64();
}

#endif // _WIN32
