#ifndef FIELD_HEIGHT_EX_H
#define FIELD_HEIGHT_EX_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/heap.h"

// height_ex.c: a list of areas of the map, each a rectangle of the grid with a height and flags (pokebw2's
// HeightExList), which the field's G3D mapper keeps (FieldG3DMapper.resortMap) and adds to the terrain at a position
typedef struct HeightExEntry {
    s32 x;
    s32 z;
    s32 width;
    s32 depth;
    s32 value;
    BOOL inUse;
    s32 flags;
} HeightExEntry;

struct HeightExList {
    s32 count;
    HeightExEntry *entries;
};

// 1 when grid position (x, z) is in the entry's area and the entry is in use
u32 func_ov021_021c0598(s32 x, s32 z, HeightExEntry *entry);
HeightExList *func_ov021_021c05d4(u32 count, u8 heapId);
void func_ov021_021c0628(u8 index, u32 x, u32 z, u32 width, u32 depth, u32 value, u32 flags, HeightExList *list);
void func_ov021_021c0674(HeightExList *list);
// func_ov021_021c0688 (the count), func_ov021_021c068c (whether a position is in an entry's area),
// func_ov021_021c06a8 (an entry's height) and func_ov021_021c06b4 (its flags) are declared in field.h
void func_ov021_021c06c0(u8 index, u32 height, HeightExList *list);

#endif
