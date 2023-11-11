/**
\file process.h
\brief Process utilities
\author Arves100
\date 14/06/2023
\project libkgt
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
typedef HANDLE KTHREAD;
#endif

/**
Application process
*/
typedef struct KPROCESS
{
	/// Executable path
	char path[MAX_PATH + 1];

	/// Arguments that started the app
	char args[MAX_PATH + 1];

	/// Working directory
	char wdir[MAX_PATH + 1];

	// 1 byte padding... TODO:fix

	/// Process flags (eg: cretion)
	uint32_t flags;

	/// Process ID
	uint32_t pid;

	/// Thread ID
	uint32_t tid;

	/// Base address
	void* baseAddr;

	/// Address size
	size_t addrSize;

	/// Entry point
	void* ep;

	/// Handle to the thread
	KTHREAD ht;

#ifdef _WIN32
	/// Handle to the process
	HANDLE hp;
#endif
} KPROCESS;

/*
KProcess creation flags
*/
enum KPROCESS_CREATEFLAGS
{
	/// Creates a suspended process
	KPROCESS_CREATE_SUSPEND = 1 << 0,

	/// Creates a process with debug flag set to true
	KPROCESS_CREATE_DEBUG = 1 << 1,

	/// Do not create the window of the process
	KPROCESS_CREATE_NO_WINDOW = 1 << 2,
};

/**
\param kp Input process structure
\return true if the process was created, otherwise false

Creates a new process
*/
extern bool proc_create(KPROCESS* kp);

/**
\param kp Process structure
\note This function DOES NOT KILL the associated process, it just releases it's handles

Frees the process structure
*/
extern void proc_free(KPROCESS* kp);

/**
\param kp Process structure

Kills the associated process
*/
extern void proc_kill(KPROCESS* kp);

/**
\param kp Process structure
\param offset Start position to write
\param data Data to write
\param len Length of the data to write
\return true if the write was successfull, otherwise false

Writes data inside a process
*/
extern bool proc_writeto(KPROCESS* kp, size_t offset, const void* data, size_t len);

/**
\param kp Process structure
\param offset Start position to read
\param data Data that will be readed
\param len Length of the data to read
\return true if the read was successfull, otherwise false

Reads data inside a process
*/
extern bool proc_readfrom(KPROCESS* kp, size_t offset, void* data, size_t len);

/**
\param kp Thread pointer

Resumes a thread
*/
extern void thread_resume(KTHREAD* kp);

/**
\param kp Thread pointer

Suspends a thread
*/
extern void thread_suspend(KTHREAD* kp);

#ifdef __cplusplus
}
#endif
