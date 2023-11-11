/**
\file test_app_to_memedit.c
\brief Simple app that should be modified by libkgt
\author Arves100
\date 14/06/2023
*/
#include <stdio.h>

#define BASE_STRING "immabasic20000"

static const char* g_changeself = BASE_STRING;

int main()
{
	(void)printf("Hallo! The string is %s\n", g_changeself);

	(void)printf("ps: press enter to exit\n");
	(void)getchar();
	return 0;
}
