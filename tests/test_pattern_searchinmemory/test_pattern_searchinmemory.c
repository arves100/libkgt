/**
\file test_pattern_searchinmemory.c
\brief Tests the pattern search ability of libkgt
\author Arves100
\date 14/06/2023
*/

#include <libkgt/pch.h>
#include <libkgt/pattern.h>

int main()
{
	TXTPattern p1;
	size_t o1;
	char mem1[] = { 0x00, 0xEF, 0xAC, 0xAB, 0xAA, 0xBB, 0xAA, 0xCC, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xAA };

	assert(pattern_parse("AA BB CC", &p1));

	o1 = pattern_find(&p1, mem1, 14);
	assert(o1 == 8);
	assert(memcmp(mem1 + o1, "\xAA\xBB\xCC", 3) == 0);

	return 0;
}
