// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/battle/btlv_stage.h: the lines in use so
// far.
#ifndef BWPORT_BATTLE_BTLV_STAGE_H
#define BWPORT_BATTLE_BTLV_STAGE_H

// Names from swan (https://github.com/ds-pokemon-hacking/swan, GPL-3.0)

#include "types.h"
#include "nitro/fx/fx.h"
#include "struct_decls.h"
#include "battle/btlv_effect.h"
#include "gfl/g3d.h"

// pokebw2's BtlvStage_Create
// kind is the battle style, tblIdx the stage's id and fileIdx the season
BtlvStage *func_ov094_021f6560(s32 kind, s32 tblIdx, s32 fileIdx, HeapID heapId);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's BtlvStage_StartAnimation
void func_ov094_021f6aa8(BtlvStage *stage, u32 side, u32 anm, fx32 speed, s32 frames);
// pokebw2's BtlvStage_Delete
void func_ov094_021f6898(BtlvStage *stage);
// pokebw2's BtlvStage_Main
void func_ov094_021f6934(BtlvStage *stage);
// pokebw2's BtlvStage_Draw
void func_ov094_021f69b0(BtlvStage *stage);
// pokebw2's BtlvStage_StartPaletteFade
void func_ov094_021f69e8(BtlvStage *stage, u8 startEvy, u8 targetEvy, u8 wait, u16 color);
// pokebw2's BtlvStage_IsPaletteFading
BOOL func_ov094_021f6a18(BtlvStage *stage);
// pokebw2's BtlvStage_SetVanish
// side 0 or 1, or 2 for both
void func_ov094_021f6a28(BtlvStage *stage, u32 side, BOOL hide);
// pokebw2's BtlvStage_IsAnimating
BOOL func_ov094_021f6ad0(BtlvStage *stage);

// Black's layout as the 8 functions that reach it have it (tools/struct_infer.py), its size the end of the last
// field they reach
typedef struct BtlvStageResTable {
    GXRgb unk_00[2];
    u32 files[4][4];
} BtlvStageResTable;

typedef struct BtlvStageAnm {
    u32 mode;
    u32 active;
    fx32 speed;
    s32 frames;
} BtlvStageAnm;

struct BtlvStage {
    void *unk_00;
    s32 unk_04;
    BtlvStageAnm *unk_08[2];
    void **unk_10;
    void **unk_14[2];
    G3DModel *unk_1C[2];
    G3DActor *unk_24[2];
    SRTMatrix unk_2C[2];
    BtlvTexPaletteFade unk_A4;
    u32 hidden : 2;
    u32 : 30;
    HeapID heapId;
};

#endif // BWPORT_BATTLE_BTLV_STAGE_H
