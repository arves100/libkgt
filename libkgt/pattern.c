/**
\file pattern.c
\brief String pattern tokenizer
\author Arves100
\date 08/08/2022
\project libkgt
*/
#include "pch.h"
#include "pattern.h"
#include "process.h"

/**
\param pattern Character rapresentation of the token
\return The token id

Parses a character into a token
*/
static uint8_t pattern_gettok(char pattern)
{
	switch (pattern)
	{
	case '|':
		return TXTTOKEN_OR;
	case '?':
		return TXTTOKEN_ANY;
	case '!':
		return TXTTOKEN_NOT;
	case ' ':
		return TXTTOKEN_NEXT;
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return TXTTOKEN_HEX;
	default:
		break;
	}

	return TXTTOKEN_UNK;
}

/**
\param c The text char to conver to number
\return The converted char or 0xFF in case of an error

Converts a ASCII character to it's number
*/
static uint8_t pattern_tohex(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'Z')
		return 0xA + (c - 'A');

	assert(false);
	return 0xFF;
}

/**
\param current The last code which the new segment will be appended to
\return A newly created code

Creates a new text code
*/
static struct TXTCode* pattern_newcode(struct TXTCode* current)
{
	struct TXTCode* ret = (struct TXTCode*)malloc(sizeof(struct TXTCode));
	if (!ret)
		return NULL;

	if (current)
	{
		ret->prev = current;
		current->next = ret;
	}
	else
		ret->prev = NULL;

	ret->next = NULL;
	ret->match = TXTCODE_MATCH;
	ret->code.bits.low = 0;
	ret->code.bits.high = 0;

	return ret;
}

/**
\param current The last segment which the new segment will be appended to
\return The newly created segment or NULL

Creates a new TXT segment
*/
static struct TXTSegment* pattern_newseg(struct TXTSegment* current)
{
	struct TXTSegment* ret = (struct TXTSegment*)malloc(sizeof(struct TXTSegment));
	if (!ret)
		return NULL;

	ret->start = pattern_newcode(NULL);

	if (!ret->start)
	{
		free(ret);
		return NULL;
	}

	if (current)
	{
		ret->prev = current;
		current->next = ret;
	}
	else
		ret->prev = NULL;

	ret->next = NULL;
	return ret;
}


bool pattern_parse(const char* pattern, TXTPattern* data)
{
	size_t offset = 0;
	char c = pattern[offset];
	struct TXTSegment* currentSeg = pattern_newseg(NULL);
	struct TXTCode* currentCode;
	bool lovalue = false;
	uint8_t except = TXTTOKEN_UNK;

	if (!currentSeg)
		return false;

	data->start = currentSeg;
	currentCode = currentSeg->start;

	while (c != '\0')
	{
		uint8_t token = pattern_gettok(c);

		if (except != TXTTOKEN_UNK && except != token)
		{
			assert(false);
			return false;
		}

		switch (token)
		{
		case TXTTOKEN_NEXT:
			currentSeg = pattern_newseg(currentSeg);
			if (!currentSeg)
			{
				assert(false);
				return false;
			}

			lovalue = false;
			currentCode = currentSeg->start;
			break;

		case TXTTOKEN_HEX:
			if (!lovalue)
			{
				currentCode->code.bits.high = pattern_tohex(c) & 0xF;
				lovalue = true;
				except = TXTTOKEN_HEX; // must see another value
			}
			else
			{
				currentCode->code.bits.low = pattern_tohex(c) & 0xF;
				lovalue = false;
				except = TXTTOKEN_UNK;
			}
			break;

		case TXTTOKEN_ANY:
			currentCode->match = TXTCODE_ANY;

			// ?? pattern fix
			if (except != TXTTOKEN_ANY)
				except = TXTTOKEN_ANY;
			else
				except = TXTTOKEN_UNK;
			break;

		case TXTTOKEN_NOT:
			currentCode->match = TXTCODE_NO_MATCH;
			except = TXTTOKEN_HEX;
			break;

		case TXTTOKEN_OR:
			currentCode = pattern_newcode(currentCode);
			if (!currentCode)
			{
				assert(false);
				return false;
			}
			break;

		default:
			// invalid token
			assert(false);
			return false;
		}

		c = pattern[++offset];
	};

	return true;
}

void pattern_free(TXTPattern* data)
{
	for (struct TXTSegment* seg = data->start, *segself = seg; seg != NULL; segself = seg)
	{
		for (struct TXTCode* code = seg->start, *self = code; code != NULL; self = code)
		{
			code = self->next;
			free(self);
		}

		seg = segself->next;
		free(segself);
	}

	data->start = NULL;
}

size_t pattern_find(TXTPattern* pattern, size_t start_offset, size_t len, PatternReadFunc func, void* ctx)
{
	struct TXTSegment* current = pattern->start;
	bool next = false;
	size_t start;

	if (!pattern || !current || len == (size_t)-1 || !func)
		return PATTERN_NOT_FOUND;

	for (size_t i = 0; i < len; i++, start_offset++)
	{
		for (struct TXTCode* code = current->start; code != NULL; code = code->next)
		{
			// just OR...
			uint8_t cdata = func(ctx, start_offset);

			if (code->match == TXTCODE_ANY)
			{
				next = true;
				break;
			}
			else if (code->match == TXTCODE_MATCH && cdata == code->code.data)
			{
				next = true;
				break;
			}
			else if (code->match == TXTCODE_NO_MATCH && cdata != code->code.data)
			{
				next = true;
				break;
			}
		}

		if (next)
		{
			if (current == pattern->start)
				start = i;

			// TXTCode passed, go to next pattern
			next = false;
			current = current->next;

			if (!current) // end of match! We found what we were looking for
				return start;
		}
		else if (current != pattern->start)
		{
			// reset to start
			current = pattern->start;

			// we should NOT exclude the first unmatching byte as it might contain the pattern
			i--;
			start_offset--;
		}
	}

	return PATTERN_NOT_FOUND; // not found...
}

/**
\param ctx Empty ctx
\param offset memory position
\return Character at the position in memory

Sample function that searches inside the block specified in offset
*/
static uint8_t pattern_find_memory_operator(void* ctx, size_t offset)
{
	return *((uint8_t*)offset);
}

size_t pattern_find_inside_memory(TXTPattern* pattern, void* data, size_t len)
{
	return pattern_find(pattern, (size_t)data, len, pattern_find_memory_operator, NULL);
}

/**
\param ctx KPROCESS handle
\param offset offset to query the memory
\return Character at the position in memory

Sample function that searches inside the specified memory
*/
static uint8_t pattern_find_inside_process_operator(void* ctx, size_t offset)
{
	uint8_t rb;
	
	if (!proc_readfrom((KPROCESS*)ctx, offset, &rb, sizeof(rb)))
		return 0x00;

	return rb;
}

size_t pattern_find_inside_process(TXTPattern* pattern, struct KPROCESS* proc)
{
	return pattern_find(pattern, (size_t)proc->baseAddr, proc->addrSize, pattern_find_inside_process_operator, proc);
}
