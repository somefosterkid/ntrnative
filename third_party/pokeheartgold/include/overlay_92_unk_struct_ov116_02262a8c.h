#ifndef POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02262A8C_H
#define POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02262A8C_H

#include "bg_window.h"
#include "field/overlay_01_021FB878.h"
#include "overlay_92_unk_struct_ov116_022649e4.h"
#include "overlay_92_unk_struct_ov116_02262d08.h"
#include "overlay_92_unk_struct_ov116_02265d84.h"

struct ManagedSprite;
struct SysTask;

// The ov116 (overlay 92) screen work: every task of the overlay carries a pointer to it (0x2BBC on DS, allocated by
// ov92_0225EB88, which still uses raw offsets). The unk_XXXX names are m2c offsets, not the compiled ones: sizeof is
// 0x2BBC and the offsets up to unk_DC (0x14, 0x1C, 0x28, 0xF4 in ov92_0225EDB4) are checked against the DS code, but
// the members after UnkStruct_ov116_022660A8 (unk_20C4 on) sit 0x18 or more away from the raw offsets ov92_0225EB88
// stores to (0x2228, 0x222C, 0x2AEC, 0x2B94): fix those inner types before typing that allocation.

typedef struct UnkStruct_ov116_02260490 {
    u32 unk_00;
} UnkStruct_ov116_02260490;

typedef struct UnkStruct_ov116_02262A8C_sub1 {
    u32 unk_00;
    u32 unk_04;
    u8 padding_08[4];
} UnkStruct_ov116_02262A8C_sub1;

typedef struct UnkStruct_ov116_02262A8C_sub2 {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    s16 unk_1C;
    s16 unk_1E;
    s16 unk_20[4];
    int unk_28[4];
    int unk_38;
    s16 unk_3C;
    s16 unk_3E;
    s16 unk_40;
    s16 unk_42;
    UnkStruct_ov116_0226469C unk_44;
    UnkStruct_ov116_0226469C unk_5C;
    UnkStruct_ov116_0226469C unk_74;
    UnkStruct_ov116_0226469C unk_8C;
    BOOL unk_A4;
    BOOL unk_A8;
    BOOL unk_AC;
    BOOL unk_B0;
    BOOL unk_B4;
    s16 unk_B8;
    s16 unk_BA;
} UnkStruct_ov116_02262A8C_sub2;

typedef union UnkUnion_ov116_02266FEC {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
    };
    f32 val2[4];
} UnkUnion_ov116_02266FEC;

typedef union UnkUnion_ov116_022671EC {
    struct {
        f32 val1_00;
        f32 val1_04;
        f32 val1_08;
        f32 val1_0C;
        f32 val1_10;
        f32 val1_14;
        f32 val1_18;
        f32 val1_1C;
        f32 val1_20;
        f32 val1_24;
        f32 val1_28;
        f32 val1_2C;
        f32 val1_30;
        f32 val1_34;
        f32 val1_38;
        f32 val1_3C;
    };
    f32 val2[4][4];
    f32 val3[16];
} UnkUnion_ov116_022671EC;

typedef struct UnkStruct_ov116_0226501C {
    Field3dObject unk_00;
    Field3dModel unk_78;
    Field3DModelAnimation unk_88[10];
    UnkUnion_ov116_022671EC unk_150;
    UnkUnion_ov116_02266FEC unk_190;
    UnkUnion_ov116_02266FEC unk_1A0;
    MtxFx43 unk_1B0;
    VecFx32 unk_1E0;
    BOOL unk_1EC;
    BOOL unk_1F0;
    int unk_1F4;
    int unk_1F8;
    f32 unk_1FC;
    f32 unk_200;
    f32 unk_204;
    f32 unk_208;
} UnkStruct_ov116_0226501C;

typedef struct UnkStruct_ov116_022647BC {
    BOOL unk_00;
    int unk_04;
    struct ManagedSprite *unk_08[3];
    UnkStruct_ov116_0226469C unk_14[3][2];
    UnkStruct_ov116_022649E4 *unk_A4;
} UnkStruct_ov116_022647BC;

typedef struct UnkStruct_ov116_02265470 {
    int unk_00;
    BOOL unk_04;
    void *unk_08[8];
    int unk_28[10];
    BOOL unk_50[10];
} UnkStruct_ov116_02265470;

typedef struct UnkStruct_ov116_0226534C {
    int unk_00;
    int *unk_04;
    UnkStruct_ov116_02265470 unk_08[5];
    struct SysTask *unk_80;
    UnkStruct_ov116_022649E4 *unk_84;
} UnkStruct_ov116_0226534C;

typedef struct UnkStruct_ov116_02265AA4 {
    UnkStruct_ov116_02265D84 unk_00[2];
    UnkStruct_ov116_02265D84 unk_A0[2];
    int unk_140;
    int unk_144;
    int unk_148;
    int unk_14C;
} UnkStruct_ov116_02265AA4;

typedef struct UnkStruct_ov116_022660A8 {
    UnkStruct_ov116_02265AA4 unk_00;
    u8 padding_04[4];
    struct SysTask *unk_08;
    int *unk_0C;
    int *unk_10;
    int *unk_14;
    int unk_18;
    int unk_1C;
    u8 padding_20[4];
    int unk_24;
    BOOL unk_28;
    BOOL unk_2C;
    UnkStruct_ov116_022649E4 *unk_30;
    UnkStruct_ov116_0226469C unk_34[5];
} UnkStruct_ov116_022660A8;

typedef struct UnkStruct_ov116_02262A8C {
    u32 unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    u8 padding_10[4];
    UnkStruct_ov116_022649E4 *unk_14;
    u8 padding_18[4];
    struct ManagedSprite *unk_1C[3];
    struct ManagedSprite *unk_20[3];
    struct ManagedSprite *unk_24[3];
    struct ManagedSprite *unk_28[36];
    struct ManagedSprite *unk_B8[4];
    struct ManagedSprite *unk_C8[2];
    struct ManagedSprite *unk_D0[2];
    struct ManagedSprite *unk_D8;
    u16 unk_DC[4];
    u8 padding_E4[12];
    int unk_F0;
    int unk_F4;
    int unk_F8;
    UnkStruct_ov116_0226501C unk_FC;
    UnkStruct_ov116_0226501C unk_308[3];
    UnkStruct_ov116_0226501C unk_92C;
    UnkStruct_ov116_0226501C unk_B38[8];
    UnkStruct_ov116_0226501C unk_1B98;
    UnkStruct_ov116_0226501C unk_1DA4;
    VecFx32 unk_1FB0;
    UnkStruct_ov116_02262A8C_sub1 unk_1FBC;
    Window unk_1FC8;
    Window unk_1FD8[4];
    UnkStruct_ov116_022660A8 unk_2018;
    UnkStruct_ov116_0226534C unk_20C4;
    UnkStruct_ov116_022647BC unk_214C[8];
    UnkStruct_ov116_02262D08 unk_268C[4];
    UnkStruct_ov116_02262A8C_sub2 unk_279C;
    UnkStruct_ov116_02260490 unk_2858[4];
    u8 padding_2868[4];
    u32 unk_286C;
    int unk_2870;
    BOOL unk_2874;
} UnkStruct_ov116_02262A8C; // 0x2BBC

#endif // POKEHEARTGOLD_OVERLAY_92_UNK_STRUCT_OV116_02262A8C_H
