#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "overlay_80_02237A70.h"


void ov83_0224773C(void *, u32, u32);

void ov83_02247988(u16 *, u16 *, u32);

typedef struct {
    u16 v[6];
} UnkStruct_ov83_022469E4_Box;

typedef struct {
    u16 sA[2];
    UnkStruct_ov83_022469E4_Box sB;
    u32 uStack_2c;
    u32 uStack_28;
    u8 uStack_1c;
    u8 pad[3];
    UnkStruct_ov83_022469E4_Box sC;
} UnkStruct_ov83_022469E4;

u32 ov83_02247CCC(void *, UnkStruct_ov83_022469E4_Box *, u32, u32, u8);

extern const UnkStruct_ov83_022469E4_Box ov83_02248018;

void ov83_022469E4(u8 *param_1, u8 param_2, u32 param_3, u32 param_4, u8 param_5);

void ov83_022469E4(u8 *param_1, u8 param_2, u32 param_3, u32 param_4, u8 param_5) {
    u32 uVar1;
    u16 x;
    UnkStruct_ov83_022469E4 s;

    ov83_02247988(&s.sA[1], s.sA, param_3);
    s.sC = ov83_02248018;
    s.sC.v[3] = s.sA[1];
    x = s.sA[0];
    s.sC.v[4] = s.sA[1] + x - 0x1b;
    s.sC.v[5] = x;
    s.sB = s.sC;
    s.uStack_2c = ((BattleCastleOpponentApp *)param_1)->unk_5FC;
    s.uStack_28 = *((u32 *)&((BattleCastleOpponentApp *)param_1)->bgConfig);
    s.uStack_1c = param_2;
    uVar1 = ov83_02247CCC(*((void **)&((BattleCastleOpponentApp *)param_1)->unk_5F4), &s.sB, param_3, param_4, param_5);
    ((BattleCastleOpponentApp *)param_1)->unk_5F8 = uVar1;
    *(u8 *)(param_1 + 0xf) = *(u8 *)(param_1 + 0xf) | 4;
    uVar1 = ov80_02237B24(*(u8 *)(param_1 + 9), 1);
    ov83_0224773C(param_1 + 0x4e4, uVar1, 1);
    ov83_0224773C(param_1 + 0x4f4, uVar1, 1);
}
