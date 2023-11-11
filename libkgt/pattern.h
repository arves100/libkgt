/**
\file pattern.h
\brief String pattern tokenizer
\author Arves100
\date 08/08/2022
\project libkgt
*/
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
Token types to be parsed by the tokenizer
*/
enum TXTToken
{
	/// Invalid token
	TXTTOKEN_UNK,

	/// Hex number
	TXTTOKEN_HEX,

	/// Any number
	TXTTOKEN_ANY,

	/// Next sequence
	TXTTOKEN_NEXT,

	/// Or another number
	TXTTOKEN_OR,

	/// Not a number
	TXTTOKEN_NOT,
};

/**
Type of match codes
*/
enum TXTMatchCode
{
	/// Must match the sequence
	TXTCODE_MATCH,

	/// Skip the character
	TXTCODE_ANY,

	/// Must not match the character
	TXTCODE_NO_MATCH,
};

/**
A structure that holds low/high bytes for TXT parser
*/
union TXTHex
{
	/// One byte
	uint8_t data;

	struct TXTBits
	{
#if __BIG_ENDIAN__
		/// High bits
		uint8_t high : 4;

		/// Low bits
		uint8_t low : 4;
#else
		/// Low bits
		uint8_t low : 4;

		/// High bits
		uint8_t high : 4;
#endif
	} bits;
};

/**
Holds a single character code format
*/
struct TXTCode
{
	/// Previous TXT code in the OR flow
	struct TXTCode* prev;

	/// Next TXT code in the OR flow
	struct TXTCode* next;

	/// Type of match
	uint8_t match; 	// ! -> false, "" -> true

	/// Byte codes
	union TXTHex code;
};

/**
Holds a number of segments for TXT pattern match
*/
struct TXTSegment
{
	/// Start of the code match
	struct TXTCode* start;

	/// Next segment of match
	struct TXTSegment* next;

	/// Previous segment of match
	struct TXTSegment* prev;
};

/**
Main structure of TXT pattern matching
*/
typedef struct TXTPattern
{
	/// Start of the match segments
	struct TXTSegment* start;
} TXTPattern;

/**
Pattern read function
*/
typedef uint8_t(*PatternReadFunc)(void* context, size_t offset);

/**
\param pattern The string to parse
\param data Output data
\return true in case of parsing succeeded, otherwise false

Parses a string into a TXT pattern
*/
extern bool pattern_parse(const char* pattern, TXTPattern* data);

/**
\param data TXT data

Clear the memory of the allocated TXT structure
*/
extern void pattern_free(TXTPattern* data);

/**
Pattern not found or invalid parameters
*/
#define PATTERN_NOT_FOUND ((size_t)-1)

/**
\param pattern Pattern to run the search
\param start Start offset
\param len Length of the pattern
\param func Pattern read function
\param ctx User context for pattern read
\return Position where the pattern is found or "PATTERN_NOT_FOUND" in case it's not found

Search a pattern inside a chunk of memory
*/
extern size_t pattern_find(TXTPattern* pattern, size_t start, size_t len, PatternReadFunc func, void* ctx);

/**
\param pattern Pattern to run the search
\param data Memory data to search
\param len Length of the pattern
\return Position where the pattern is found or "PATTERN_NOT_FOUND" in case it's not found

Search a pattern inside specified memory data
*/
extern size_t pattern_find_inside_memory(TXTPattern* pattern, void* data, size_t len);

/**
\param pattern Pattern to run the search
\param func Pattern read function
\param proc Process info
\return Position where the pattern is found or "PATTERN_NOT_FOUND" in case it's not found

Search a pattern inside a process
*/
extern size_t pattern_find_inside_process(TXTPattern* pattern, struct KPROCESS* proc);

#ifdef __cplusplus
}
#endif
