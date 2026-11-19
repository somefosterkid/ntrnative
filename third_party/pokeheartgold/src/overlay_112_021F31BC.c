#include "global.h"
#include "math_util.h"
#include "party.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "save_trainer_house.h"

s32 ov112_021F31BC(void *arg0);

void ov112_021F336C(u32 *param_1, SaveData *param_2);
void ov112_021F33D8(u16 *param_1, Party *param_2);
void ov112_021F3244(SaveData *param_1, TrainerHouseSet *param_2);

s32 ov112_021F31BC(void *arg0) {
    s32 v1;
    void *v0;

    v0 = arg0;
    v1 = 0;
loop_1:
    if (*(u8 *)((u8 *)v0 + 0x84) == 0x1D) {
        return 1;
    }
    v1 += 1;
    v0 += 0x88;
    if (v1 >= 0x18) {
        return 0;
    }
    goto loop_1;
}

s32 ov112_021F31D8(void *arg0);

s32 ov112_021F31D8(void *arg0) {
    s32 v3;
    u16 v1;
    u8 v0;
    void *v2;

    v2 = arg0;
    v3 = 0;
loop_1:
    v0 = *(u8 *)((u8 *)v2 + 0x84);
    if (v0 != 0) {
        if ((u32)v0 >= 0x1EU) {
            return 1;
        }
        v1 = *(u16 *)((u8 *)v2 + 0xA);
        if (v1 == 0) {
            return 1;
        }
        if (((u32)(u16)(*(u16 *)((u8 *)v2 + 8) + 0xFFF9) <= 1U) && (((u32)v1 > 0x1EDU) || ((u32) * (u16 *)((u8 *)v2 + 0xC) > 0x1EDU))) {
            return 1;
        }
        if ((u32) * (u8 *)((u8 *)v2 + 0x76) >= 8U) {
            return 1;
        }
        goto block_12;
    }
block_12:
    v3 += 1;
    v2 += 0x88;
    if (v3 >= 0x18) {
        return 0;
    }
    goto loop_1;
}

void ov112_021F3244(SaveData *param_1, TrainerHouseSet *param_2) {
    u16 uVar1;
    Party *pPVar2;

    Save_PlayerData_GetProfile(param_1);
    pPVar2 = SaveArray_Party_Get(param_1);
    MIi_CpuClearFast(0, param_2, 500);
    TrainerHouseSet_SetZero(param_2);
    ov112_021F336C((u32 *)param_2, param_1);
    ov112_021F33D8((u16 *)param_2->party, pPVar2);
    uVar1 = GF_CalcCRC16(param_2, 0x1f2);
    param_2[1].party[1].moves[3] = uVar1;
    return;
}
