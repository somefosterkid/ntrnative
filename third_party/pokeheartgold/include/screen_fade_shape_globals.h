#ifndef POKEHEARTGOLD_SCREEN_FADE_SHAPE_GLOBALS_H
#define POKEHEARTGOLD_SCREEN_FADE_SHAPE_GLOBALS_H

#include "global.h"

// One .data block at 0x0210F64C (asm/unk_0201010C.s, labels _0210F64C.._0210F6B4). Each shape fade function
// (FadeFunc_xx) stores the address of its shape table (.rodata _020F5Dxx, an array of UnkStruct_0200F898) into
// the matching record before handing the record to sub_0201289C / sub_02012B1C. mwcc addressed all of it from
// the first label, so m2c read the records as byte offsets from _0210F64C. On 64-bit the shape pointer makes a
// record 16 bytes, not 8: keep it one typed object and store through the fields.

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_0200FD8C;

typedef struct {
    UnkStruct_0200FD8C unk_00;
    UnkStruct_0200FD8C unk_04;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkStruct_0200F980;

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
} UnkStruct_0200F898;

typedef struct {
    const UnkStruct_0200F898 *shape; // 0x00: filled at run time by the FadeFunc that owns the record
    u16 count;                       // 0x04: number of UnkStruct_0200F898 in the table (sub_02012A90's loop bound)
    u16 unk_06;
} UnkStruct_0200FE6C;

typedef struct {
    UnkStruct_0200F980 unk_00;
    UnkStruct_0200FE6C unk_0C;
    fx32 unk_14;
} UnkStruct_0201006C;

typedef struct ScreenFadeShapeGlobals {
    // 0x00: _0210F64C.._0210F694, 8 bytes each on DS. Owner of each record: [0] FadeFunc_02 (still asm), [1] _08, [2] _03,
    // [3] _31, [4] _04, [5] _05, [6] _30, [7] _09, [8] _11, [9] _10
    UnkStruct_0200FE6C shapes[10];
    UnkStruct_0201006C windowFadeOut;    // 0x50: _0210F69C (FadeFunc_40), 0x18 bytes on DS
    UnkStruct_0201006C windowFadeIn;     // 0x68: _0210F6B4 (FadeFunc_41), 0x18 bytes on DS
} ScreenFadeShapeGlobals;

extern ScreenFadeShapeGlobals _0210F64C;

#endif // POKEHEARTGOLD_SCREEN_FADE_SHAPE_GLOBALS_H
