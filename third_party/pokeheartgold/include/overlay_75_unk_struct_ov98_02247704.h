#ifndef POKEHEARTGOLD_OVERLAY_75_UNK_STRUCT_OV98_02247704_H
#define POKEHEARTGOLD_OVERLAY_75_UNK_STRUCT_OV98_02247704_H

// UnkStruct_ov98_02247704: merged by tools/hg/promote.py from 18 m2c local copies (1 variants, base
// src/overlay_75_02246BE0_02246F0C_02247118.c) and 51 accesses; fields at every accessed offset, pads elsewhere.

#include "bg_window.h"

struct BgConfig;
struct ListMenuCursor;
struct ListMenu;
struct YesNoPrompt;
struct MsgData;
struct String;
struct ListMenuItem;
struct MessageFormat;
struct SysTask;
struct UnkStruct_ov98_02246E88;

// The vblank palette-fade task work (ov75_022476E8); one word wider than Platinum's UnkStruct_ov61_0222C3B0 (unk_32C = fade mode).
typedef struct UnkStruct_ov75_022476E8 {
    struct SysTask *unk_00;
    BOOL unk_04;
    u16 unk_08[4][16];
    u16 unk_88[21][16];
    s16 unk_328;
    s8 unk_32A;
    u8 unk_32B;
    u32 unk_32C;
} UnkStruct_ov75_022476E8;

typedef struct UnkStruct_ov98_02247704 {
    struct UnkStruct_ov98_02246E88 *unk_00;
    struct BgConfig *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    int unk_1C;
    struct MessageFormat *unk_20;
    struct MsgData *unk_24;
    struct MsgData *unk_28;
    struct MsgData *unk_2C;
    struct MsgData *unk_30;
    struct MsgData *unk_34;
    struct String *unk_38;
    struct String *unk_3C;
    struct String *unk_40;
    int unk_44;
    Window unk_48;
    Window unk_58;
    Window unk_68;
    Window unk_78;
    struct YesNoPrompt *unk_88;
    void *unk_8C;
    int unk_90;
    int unk_94;
    int unk_98;
    int unk_9C;
    struct ListMenuItem *unk_A0;
    struct ListMenu *unk_A4;
    int unk_A8;
    int unk_AC;
    int unk_B0;
    int unk_B4;
    int unk_B8;
    int unk_BC;
    int unk_C0;
    Window unk_C4;
    Window unk_D4;
    struct ListMenuCursor *unk_E4;
    s32 unk_E8;
    UnkStruct_ov75_022476E8 unk_EC;
} UnkStruct_ov98_02247704;

// sizeof(UnkStruct_ov98_02247704) == 0x41c (the heap object of ov75_02246F0C: OverlayManager_CreateAndGetData(sizeof(...)))

#endif // POKEHEARTGOLD_OVERLAY_75_UNK_STRUCT_OV98_02247704_H
