#ifndef POKEHEARTGOLD_UNK_020957B0_H
#define POKEHEARTGOLD_UNK_020957B0_H

#include "global.h"
#include "constants/heap.h"

// Cursor navigation over the areas of a screen (the PC storage screen, unk_020932E0): each area is a
// cols x rows grid of cursor slots; moving the cursor off the edge of one area hands over to a
// neighbour. The 0x10-byte (DS) object holds the area table and one cursor position per area.

typedef struct CursorNavAreaInfo {
    u8 cols;
    u8 rows;
    u8 count;
    u8 id;
} CursorNavAreaInfo;

typedef struct CursorNavPos {
    s8 x;
    s8 y;
    u8 index; // y * cols + x
    u8 unk_3;
} CursorNavPos;

typedef struct CursorNavEvent {
    s32 kind; // result of sub_02095BF0: 0 moved, 1..4 left the area, 2 out of range
    u8 value;
} CursorNavEvent;

typedef struct CursorNav CursorNav;

typedef struct CursorNavArea {
    const CursorNavAreaInfo *info;
    void (*move)(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
    void (*enter)(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
} CursorNavArea;

struct CursorNav {
    u8 area; // index into areas
    u8 areaCount;
    u8 mode;
    u8 unk_3;
    const u8 *screenMode; // the owner's unk_F: 0x12 hands the cursor over to area 6
    const CursorNavArea *areas;
    CursorNavPos *positions; // one per area
};

extern const CursorNavArea _02108434[]; // asm table, areaCount entries

void sub_020957B0(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_02095838(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
void sub_020958C0(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_02095920(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
void sub_0209592C(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_020959D8(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
void sub_02095A34(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_02095A78(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
void sub_02095A7C(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_02095AF8(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
void sub_02095B04(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_02095B88(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, const CursorNavEvent *event);
s32 sub_02095BF0(CursorNav *nav, CursorNavPos *pos, const CursorNavAreaInfo *info, s32 dx, s8 dy);
void sub_02095C90(CursorNav *nav, u32 areaId);
void sub_02095CB8(CursorNav *nav, u32 areaId, const CursorNavEvent *event);

CursorNav *sub_02095CE0(enum HeapID heapID, const u8 *screenMode);
void sub_02095D1C(CursorNav *nav, u8 mode);
void sub_02095D2C(CursorNav *nav);
void sub_02095D40(CursorNav *nav, u32 areaId, u32 index);
s32 sub_02095D88(CursorNav *nav, s32 dx, s8 dy);
u8 sub_02095DD8(CursorNav *nav);
u8 sub_02095DE8(CursorNav *nav);

#endif // POKEHEARTGOLD_UNK_020957B0_H
