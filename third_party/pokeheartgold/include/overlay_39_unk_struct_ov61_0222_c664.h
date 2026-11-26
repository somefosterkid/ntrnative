#ifndef POKEHEARTGOLD_OVERLAY_39_UNK_STRUCT_OV61_0222_C664_H
#define POKEHEARTGOLD_OVERLAY_39_UNK_STRUCT_OV61_0222_C664_H

// UnkStruct_ov61_0222C664: merged by tools/hg/promote.py from 5 m2c local copies (2 variants, base
// src/overlay_39_thumb_02227FEC_022285CC_022285CC.c) and 32 accesses; fields at every accessed offset, pads elsewhere.

#include "bg_window.h"

struct BgConfig;
struct ListMenu;
struct Menu;
struct MsgData;
struct String;
struct StringList;
struct StringTemplate;
struct SysTask;

typedef struct UnkStruct_ov61_0222C664 {
    void *unk_00;
    struct BgConfig *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    struct StringTemplate *unk_20;
    struct MsgData *unk_24;
    struct MsgData *unk_28;
    struct MsgData *unk_2C;
    struct MsgData *unk_30;
    struct String *unk_34;
    struct String *unk_38;
    struct String *unk_3C;
    int unk_40;
    Window unk_44;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_54;
    Window unk_64;
    struct { struct BgConfig *bgConfig; u8 bgLayer; u8 tilemapLeft; u8 tilemapTop; u8 width; u8 height; u8 palette; u16 baseTile : 15; u16 colorMode : 1; void *pixels; } unk_74;
    struct Menu *unk_84;
    void *unk_88;
    int unk_8C;
    int unk_90;
    int unk_94;
    int unk_98;
    struct StringList *unk_9C;
    struct ListMenu *unk_A0;
    u8 pad_A4[4];
    struct { struct SysTask *unk_00; BOOL unk_04; u16 unk_08[16][4]; u16 unk_88[16][21]; s16 unk_328; s8 unk_32A; u8 unk_32B; int unk_32C; } unk_A4;
} UnkStruct_ov61_0222C664;

// sizeof(UnkStruct_ov61_0222C664) == 0x3d8

#endif // POKEHEARTGOLD_OVERLAY_39_UNK_STRUCT_OV61_0222_C664_H
