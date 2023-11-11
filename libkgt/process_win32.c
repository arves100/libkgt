/**
\file process_win32.c
\brief Process utilities for Windows NT
\author Arves100
\date 14/06/2023
\project libkgt
*/
#include "pch.h"
#include "process.h"
#include "utls.h"
#include "win32_internal.h"

bool proc_create(KPROCESS* kp)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	DWORD flags = 0;
	wchar_t* path;
	wchar_t* args;
	wchar_t* wdir;
	MODULEINFO mi;

	path = u_utf8_to_utf16(kp->path);
	if (!path)
		return false;

	args = u_utf8_to_utf16(kp->args);

	// args don't need to be checked
	// if it's null then we mean no args


	wdir = u_utf8_to_utf16(kp->wdir);
	// working dir doesn't have to be checked
	// if it's null then we eredidate it

	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	si.cb = sizeof(si);

	if (kp->flags & KPROCESS_CREATE_DEBUG)
		flags |= DEBUG_PROCESS;
	if (kp->flags & KPROCESS_CREATE_NO_WINDOW)
		flags |= CREATE_NO_WINDOW;
	if (kp->flags & KPROCESS_CREATE_SUSPEND)
		flags |= CREATE_SUSPENDED;

	if (!CreateProcessW(path, args, NULL, NULL, FALSE, flags, NULL, wdir, &si, &pi))
	{
		free(path);
		free(args);
		if (wdir)
			free(wdir);

		return false;
	}

	kp->hp = pi.hProcess;
	kp->ht = pi.hThread;
	kp->pid = pi.dwProcessId;
	kp->tid = pi.dwThreadId;

	if (wdir)
		free(wdir);

	if (args)
		free(args);

	free(path);

	if (!(kp->flags & KPROCESS_CREATE_SUSPEND))
	{
		if (GetModuleInformation(pi.hProcess, NULL, &mi, sizeof(mi)))
		{
			kp->addrSize = mi.SizeOfImage;
			kp->baseAddr = mi.lpBaseOfDll;
			kp->ep = mi.EntryPoint;
		}
		else
		{
			proc_kill(kp);
			proc_free(kp);
			return false;
		}
	}
	else
	{
		kp->addrSize = 0;
		kp->baseAddr = NULL;
		kp->ep = NULL;
	}

	return true;
}

void proc_free(KPROCESS* proc)
{
	CloseHandle(proc->ht);
	CloseHandle(proc->hp);
}

void proc_kill(KPROCESS* proc)
{
	TerminateProcess(proc->hp, 0);
}

bool proc_writeto(KPROCESS* kp, size_t offset, const void* data, size_t len)
{
	SIZE_T w;
	
	if (!WriteProcessMemory(kp->hp, (LPVOID)((size_t)kp->baseAddr + offset), data, len, &w))
		return false;

	return w == len;
}

bool proc_readfrom(KPROCESS* kp, size_t offset, void* data, size_t len)
{
	SIZE_T w;

	if (!ReadProcessMemory(kp->hp, (LPVOID)((size_t)kp->baseAddr + offset), data, len, &w))
		return false;

	return w == len;
}

void thread_resume(KTHREAD proc)
{
	ResumeThread(proc);
}

void thread_suspend(KTHREAD proc)
{
	SuspendThread(proc);
}
