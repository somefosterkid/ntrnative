#ifndef POKEHEARTGOLD_OVERLAY_85_OV109_021D5140_H
#define POKEHEARTGOLD_OVERLAY_85_OV109_021D5140_H

// UnkStruct_ov109_021D5140: overlay 85's app object (OverlayManager data of ov85_021E8B08's app, HEAP_ID_102,
// sizeof checked by the matching alloc + memset). Hoisted from the 30 identical m2c local copies (hk229).

#include "global.h"
#include "bg_window.h"
#include "pm_string.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_struct_0209_bdf8.h"

struct ListMenu2D;
struct MessageFormat;
struct MsgData;
struct PlayerProfile;
struct UnkStruct_0209C194_t;

typedef struct UnkStruct_ov109_021D5140_sub1 {
    int unk_00;
    int unk_04;
    int unk_08;
    void *unk_0C;
    NNSG2dPaletteData *unk_10;
} UnkStruct_ov109_021D5140_sub1;

typedef struct UnkStruct_ov109_021D5140_sub2 {
    u8 unk_00[3000];
    u32 unk_BB8;
    u32 unk_BBC;
} UnkStruct_ov109_021D5140_sub2;

typedef union UnkStruct_ov109_021D5140_sub3 {
    u64 val1;
    struct {
        u32 unk_00;
        u32 unk_04;
    } val2;
} UnkStruct_ov109_021D5140_sub3;

typedef struct UnkStruct_ov109_021D5140_t {
    int unk_00;
    int unk_04;
    int unk_08;
    struct UnkStruct_0209C194_t *unk_0C;
    UnkStruct_0209BDF8 *unk_10;
    BgConfig *unk_14;
    BOOL unk_18;
    UnkStruct_ov109_021D5140_sub1 unk_1C;
    SysTask *unk_30;
    struct MessageFormat *unk_34;
    struct MsgData *unk_38;
    String *unk_3C[5];
    String *unk_50;
    String *unk_54;
    String *unk_58;
    int unk_5C;
    SpriteList *unk_60;
    G2dRenderer unk_64;
    GF_2DGfxResMan *unk_1F0[4];
    SpriteResource *unk_200[3][4];
    SpriteResourcesHeader unk_230;
    SpriteResourcesHeader unk_254;
    SpriteResourcesHeader unk_278;
    Sprite *unk_29C[14];
    Sprite *unk_2D4[14];
    Window unk_30C[5];
    Window unk_35C;
    Window unk_36C;
    Window unk_37C;
    Window *unk_38C[2];
    struct ListMenu2D *unk_394;
    void *unk_398[2];
    NNSG2dCharacterData *unk_3A0[2];
    void *unk_3A8[2];
    NNSG2dPaletteData *unk_3B0[2];
    int unk_3B8;
    int unk_3BC;
    int unk_3C0;
    int unk_3C4;
    u8 unk_3C8[8][2];
    struct PlayerProfile *unk_3D8[5][2];
    int unk_400[5];
    int unk_414;
    u16 unk_418;
    u16 *unk_41C;
    u8 unk_420;
    u8 unk_421;
    UnkStruct_ov109_021D5140_sub2 unk_424;
    UnkStruct_ov109_021D5140_sub2 unk_FE4[5];
    u8 unk_4AA4;
    volatile int unk_4AA8;
    int unk_4AAC;
    u32 unk_4AB0;
    u8 unk_4AB4;
    s8 unk_4AB5;
    u8 unk_4AB6;
    u8 unk_4AB7;
    int unk_4AB8;
    s32 unk_4ABC;
    s16 unk_4AC0;
    UnkStruct_ov109_021D5140_sub3 unk_4AC4[5][2];
} UnkStruct_ov109_021D5140;

#endif // POKEHEARTGOLD_OVERLAY_85_OV109_021D5140_H
