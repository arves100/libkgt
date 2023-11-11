/**
\file win32_internal.h
\brief Internal NT functions
\author Arves100
\date 04/09/2023
\project libkgt
*/
#pragma once

#ifdef _WIN32

#ifdef __cplusplus
extern "C" {
#endif

/// NtQueryInformationProcess function definition
typedef NTSTATUS(NTAPI* NtQueryInformationProcessCallback)(
	_In_            HANDLE           ProcessHandle,
	_In_            PROCESSINFOCLASS ProcessInformationClass,
	_Out_           PVOID            ProcessInformation,
	_In_            ULONG            ProcessInformationLength,
	_In_opt_ PULONG           ReturnLength
);

/// RtlNtStatusToDosErrorCallback function definition
typedef ULONG (NTAPI* RtlNtStatusToDosErrorCallback)(
	_In_ NTSTATUS Status
);

/// NTDLL function
#define NTDLL_FUNC(x) x##Ptr

/// NTDLL function type
#define NTDLL_FUNC_TYPE(x) x##Callback

/// NTDLL function declaration
#define NTDLL_FUNC_DECL(x) NTDLL_FUNC_TYPE(x) NTDLL_FUNC(x)

/// NtQueryInformationProcess
extern NTDLL_FUNC_DECL(NtQueryInformationProcess);

/// RtlNtStatusToDosError
extern NTDLL_FUNC_DECL(RtlNtStatusToDosError);

#ifdef __cplusplus
}
#endif

#endif // _WIN32
