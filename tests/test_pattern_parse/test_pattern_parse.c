/**
\file test_pattern_parse.c
\brief Test pattern parsing functionality of libkgt
\author Arves100
\date 08/08/2022
*/
#include <libkgt/pch.h>
#include <libkgt/pattern.h>

int main()
{
    TXTPattern scan;
    struct TXTSegment* code;
    pattern_parse("?? !DF C3 !D1|E9 !CC|!B0 !CC|!BB|FF|?? 00 FF|FF", &scan);

    code = scan.start; // ??
    assert(code->start->match == TXTCODE_ANY);

    code = code->next; // !DF

    assert(code->start->match == TXTCODE_NO_MATCH);
    assert(code->start->code.bits.high == 0xD);
    assert(code->start->code.bits.low == 0xF);
    assert(code->start->code.data == 0xDF);

    code = code->next; // C3
    assert(code->start->code.bits.high == 0xC);
    assert(code->start->code.bits.low == 0x3);
    assert(code->start->match == TXTCODE_MATCH);
    assert(code->start->code.data == 0xC3);

    code = code->next; // !D1|E9
    assert(code->start->code.bits.high == 0xD);
    assert(code->start->code.bits.low == 0x1);
    assert(code->start->code.data == 0xD1);
    assert(code->start->match == TXTCODE_NO_MATCH);
    assert(code->start->next->code.bits.high == 0xE);
    assert(code->start->next->code.bits.low == 0x9);
    assert(code->start->next->match == TXTCODE_MATCH);
    assert(code->start->next->code.data == 0xE9);

    code = code->next; // !CC|!B0
    assert(code->start->code.bits.high == 0xC);
    assert(code->start->code.bits.low == 0xC);
    assert(code->start->code.data == 0xCC);
    assert(code->start->match == TXTCODE_NO_MATCH);
    assert(code->start->next->code.bits.high == 0xB);
    assert(code->start->next->code.bits.low == 0x0);
    assert(code->start->next->match == TXTCODE_NO_MATCH);
    assert(code->start->next->code.data == 0xB0);

    code = code->next; // !CC|!BB|FF|??
    assert(code->start->code.bits.high == 0xC);
    assert(code->start->code.bits.low == 0xC);
    assert(code->start->code.data == 0xCC);
    assert(code->start->match == TXTCODE_NO_MATCH);
    assert(code->start->next->code.bits.high == 0xB);
    assert(code->start->next->code.bits.low == 0xB);
    assert(code->start->next->code.data == 0xBB);
    assert(code->start->next->match == TXTCODE_NO_MATCH);
    assert(code->start->next->next->code.bits.high == 0xF);
    assert(code->start->next->next->code.bits.low == 0xF);
    assert(code->start->next->next->code.data == 0xFF);
    assert(code->start->next->next->match == TXTCODE_MATCH);
    assert(code->start->next->next->next->match == TXTCODE_ANY);

    code = code->next; // 00
    assert(code->start->code.bits.high == 0x0);
    assert(code->start->code.bits.low == 0x0);
    assert(code->start->code.data == 0x00);
    assert(code->start->match == TXTCODE_MATCH);

    code = code->next; // FF|FF
    assert(code->start->code.bits.high == 0xF);
    assert(code->start->code.bits.low == 0xF);
    assert(code->start->code.data == 0xFF);
    assert(code->start->match == TXTCODE_MATCH);
    assert(code->start->next->code.bits.high == 0xF);
    assert(code->start->next->code.bits.low == 0xF);
    assert(code->start->next->code.data == 0xFF);
    assert(code->start->next->match == TXTCODE_MATCH);

    assert(code->next == NULL);

    pattern_free(&scan);
}
