#include "global.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_022447E0(void *, void *, u32, u32, u32, u32, u8, u8, u8, u8);

void ov83_02246CC0(u8 *param_1, u32 param_2, u32 param_3);

void ov83_02246CC0(u8 *param_1, u32 param_2, u32 param_3) {
    u32 uVar1;
    uVar1 = (u32)(*((u8 *)((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5F8)) + 0x24)));
    if ((((BattleCastleOpponentApp *)param_1)->unk_600) != uVar1) {
        uVar1 = *((u32 *)(((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5FC)) + (uVar1 * 8)) + 4));
        switch (uVar1) {
        case 0:
            param_3 = 0xc;
            break;
        case 1:
            param_3 = 0xd;
            break;
        case 2:
            param_3 = 0xe;
            break;
        case 0xfffffffe:
            param_3 = 0xf;
            break;
        }
        ov83_022447E0(param_1, param_1 + 0xc0, param_3, 1, 1, 0xff, 1, 2, 0xf, 1);
        ((BattleCastleOpponentApp *)param_1)->unk_600 = (u32)(*((u8 *)((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5F8)) + 0x24)));
    }
}

u8 ov83_0224777C(void *, u8, u8, u32);

extern u32 ov83_02248024;

extern u32 ov83_02248028;

extern u32 ov83_0224802C;

extern u32 ov83_02248030;

void ov83_02246D40(u8 *param_1, u32 param_2, u32 param_3, u32 param_4);

void ov83_02246D40(u8 *param_1, u32 param_2, u32 param_3, u32 param_4) {
    s32 iVar1;
    u32 uVar2;
    u32 uVar3;
    u32 unaff_r4;
    if ((((BattleCastleOpponentApp *)param_1)->unk_600) != ((u32)(*((u8 *)((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5F8)) + 0x24))))) {
        iVar1 = ov83_0224777C(((void *)((BattleCastleOpponentApp *)param_1)->saveData), ((BattleCastleOpponentApp *)param_1)->challengeType, 2, param_4);
        uVar2 = (u32)(iVar1 != 1);
        uVar3 = *((u32 *)(((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5FC)) + (((u32)(*((u8 *)((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5F8)) + 0x24)))) * 8)) + 4));
        switch (uVar3) {
        case 3:
            unaff_r4 = *((u16 *)(((u8 *)(&ov83_02248024)) + (uVar2 * 2)));
            break;
        case 4:
            unaff_r4 = (u32)(*((u16 *)(((u8 *)(&ov83_02248028)) + (uVar2 * 2))));
            break;
        case 5:
            unaff_r4 = (u32)(*((u16 *)(((u8 *)(&ov83_0224802C)) + (uVar2 * 2))));
            break;
        case 0xfffffffe:
            unaff_r4 = (u32)(*((u16 *)(((u8 *)(&ov83_02248030)) + (uVar2 * 2))));
            break;
        }
        ov83_022447E0(param_1, param_1 + 0xc0, unaff_r4, 1, 1, 0xff, 1, 2, 0xf, 1);
        ((BattleCastleOpponentApp *)param_1)->unk_600 = (u32)(*((u8 *)((*((u8 **)&((BattleCastleOpponentApp *)param_1)->unk_5F8)) + 0x24)));
    }
    return;
}
