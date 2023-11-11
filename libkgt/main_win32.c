/**
\file main_win32.c
\brief Main wrapper for NT an delay load
\author Arves100
\date 04/09/2023
\project libkgt
*/
#include "pch.h"
#include "main.h"
#include "win32_internal.h"

NTDLL_FUNC_DECL(NtQueryInformationProcess) = NULL;
NTDLL_FUNC_DECL(RtlNtStatusToDosError) = NULL;

/// NTDLL.DLL
static HMODULE ntdll = NULL;

/// Assign NTDLL function
#define NTDLL_FUNC_ASSIGN(x) NTDLL_FUNC(x) = (NTDLL_FUNC_TYPE(x))(GetProcAddress(ntdll, #x))

bool kgt_delay_init(void)
{
	ntdll = LoadLibraryW(L"ntdll.dll");

	if (!ntdll)
		return false;

	// load NTDLL.DLL functions

	NTDLL_FUNC_ASSIGN(NtQueryInformationProcess);
	NTDLL_FUNC_ASSIGN(RtlNtStatusToDosError);

	return true;
}

void kgt_delay_free(void)
{
	if (ntdll)
		FreeLibrary(ntdll);

	ntdll = NULL;
}
