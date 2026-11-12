#ifndef LOCAL_TVT_CHARA_H
#define LOCAL_TVT_CHARA_H

#include "types.h"
#include "gfl/heap.h"

#include "ov174_021b6100.h"
#include "gfl/bg_sys.h"
#include "gfl/gx_layers.h"
#include "gfl/bmpwin.h"
#include "gfl/clact.h"
#include "nnsys/g2d.h"

struct LocalTvtFace {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    s32 unk_08;
    NNSG2dCharacterData *unk_0c;
    void *unk_10;
    u32 unk_14;
    BmpWin *unk_18;
    u32 unk_1c;
    u32 unk_20;
    u32 unk_24;
    ClActor *unk_28;
    ClActor *unk_2c;
    u16 unk_30;
    s32 unk_34;
    u16 unk_38;
}; // 0x3c

void func_ov174_021b7004(Ov174Work *work, LocalTvtFace *face);
void func_ov174_021b6f3c(Ov174Work *work, LocalTvtFace *face);
void func_ov174_021b71b8(Ov174Work *work, LocalTvtFace *face);

u8 func_ov174_021b7360(LocalTvtFace *face);
void func_ov174_021b6d00(Ov174Work *work, LocalTvtFace *face);

// Where the names of the four faces are, in tiles
typedef struct {
    u8 x;
    u8 y;
} LocalTvtNamePos;

extern const LocalTvtNamePos data_ov174_021b7380[4];
extern const BGSysLCDConfig data_ov174_021b7388;
extern const BGSysVRAMConfig data_ov174_021b7450;
extern const BGSetup data_ov174_021b73b0;
extern const BGSetup data_ov174_021b73d0;
extern const BGSetup data_ov174_021b73f0;
extern const BGSetup data_ov174_021b7410;
extern const BGSetup data_ov174_021b7430;
void func_ov174_021b7364(LocalTvtFace *face, s32 speaking);
LocalTvtFace *func_ov174_021b6c70(Ov174Work *work, u32 idx);
u32 func_ov174_021b6f2c(Ov174Work *work, LocalTvtFace *face);
void func_ov174_021b6f24(Ov174Work *work, LocalTvtFace *face, u8 param_3);
void func_ov174_021b7350(Ov174Work *work, LocalTvtFace *face);
void func_ov174_021b7378(LocalTvtFace *face);
void func_ov174_021b6d3c(Ov174Work *work, LocalTvtFace *face);
void func_ov174_021b7180(Ov174Work *work, LocalTvtFace *face, u32 fileId);
void func_ov174_021b71a0(Ov174Work *work, LocalTvtFace *face, u32 srcOffset, u32 dstOffset, u32 size);

#endif
