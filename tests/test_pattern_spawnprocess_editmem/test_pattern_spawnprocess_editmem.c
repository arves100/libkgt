/**
\file test_pattern_spawnprocess_editmem.c
\brief Tests the memory modification abilities of a process with libkgt
\author Arves100
\date 14/06/2023
*/
#define KGT_NO_MAIN_DEF 1
#include <libkgt/pch.h>
#include <libkgt/pattern.h>
#include <libkgt/process.h>
#include <libkgt/main.h>
#include <stdio.h>

#define TARGET_STRING "yoooigothaxxed"

int main(int argc, char** argv)
{
	KPROCESS proc;
	TXTPattern ptn;
	size_t pos;
	char target[20];

	if (argc < 2)
	{
		(void)printf("Usage: %s [app]\n", argv[0]);
		return -1;
	}

	if (!kgt_delay_init())
	{
		(void)printf("Cannot init libkgt\n");
		return -1;
	}

	atexit(kgt_delay_free);

	ptn.start = NULL;

	if (!pattern_parse("69 6D 6D 61 62 61 73 69 63 32 30 30 30 30", &ptn))
	{
		pattern_free(&ptn);
		proc_free(&proc);
		(void)printf("Cannot parse TXT pattern\n");
		return -1;
	}

	assert(ptn.start->start->match == TXTCODE_MATCH);
	assert(ptn.start->start->code.data == 0x69);

	memset(&proc, 0, sizeof(proc));
	strcpy(proc.path, argv[1]);
	
	if (!proc_create(&proc))
	{
		pattern_free(&ptn);
		(void)printf("Cannot create process\n");
		return -1;
	}

	thread_suspend(proc.ht);

	pos = pattern_find_inside_process(&ptn, &proc);

	if (pos == PATTERN_NOT_FOUND)
	{
		proc_kill(&proc);
		pattern_free(&ptn);
		proc_free(&proc);
		(void)printf("Cannot find pattern in process\n");
		return -1;
	}

	if (!proc_writeto(&proc, pos, TARGET_STRING, 14))
	{
		proc_kill(&proc);
		pattern_free(&ptn);
		proc_free(&proc);
		(void)printf("Cannot write to memory\n");
		return -1;
	}

	if (!proc_readfrom(&proc, pos, target, 14))
	{
		proc_kill(&proc);
		pattern_free(&ptn);
		proc_free(&proc);
		(void)printf("Cannot read to memory\n");
		return -1;
	}

	(void)printf("ok\n");

	thread_resume(proc.ht);

	pattern_free(&ptn);
	proc_free(&proc);
	return 0;
}
