#include "global.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_07.h"
#include "pokemon.h"
#include "pokepic.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"
#include "unk_02016EDC.h"
#include "overlay_12_mon_show_data.h"
#include "overlay_12_battle_controller_opponent.h"

void *ov07_0221BEDC(s32);

s32 ov07_0221BFE0(void *);

s32 ov07_0221C394(void *);

s32 ov07_0221C3B0(void *);

s32 ov07_0221C3C0(void *);

void *ov07_0221FDFC(BattleSystem *, s32);

void ov07_0221FE08(struct BattleMonOBJData *btlMonObjData);

Pokepic *ov12_022612A4(BattleSystem *, PokepicManager *, void *, int, s32, s32, s32, s32, s32, s32, s32, PokepicAnimScript *, void *);

s32 ov12_02261B80(BattleSystem *, void *, void *, MoveAnimation *);


s32 sub_02005B58(u8);

extern const s16 ov07_022377DC[][2];

extern const s16 ov07_022377F4[][3];

extern void *ov12_0226D15A;

void ov12_0225B960(SysTask *arg0, void *arg1);

void ov12_0225B960(SysTask *arg0, void *arg1) {
    MoveAnimation sp58;
#if !defined(SDK_PORT) && !defined(NONMATCHING)
    u32 sp4c[3];
#endif
    u32 sp24[10];
    BallCapsuleSealEffect *temp_r0_3;
    PokepicManager *temp_r5;
    u8 temp_r0;
    u8 temp_r1_2;
    u8 temp_r3;

    OpponentData *temp_r3_2;
    OpponentData *temp_r3_3;

    temp_r0 = ((MonShowData *)arg1)->state;
    switch ((u32)temp_r0) {
    case 0:
        ((MonShowData *)arg1)->delay = 0U;
        *((s32 *)&((MonShowData *)arg1)->btlMonObjData) = 0;
        if (BattleSystem_GetBattleType(*(BattleSystem **)arg1) & 8) {
            if (!(BattleSystem_GetBattleSpecial(*(BattleSystem **)arg1) & 0x20) && (((MonShowData *)arg1)->battlerType == 2)) {
                *((void **)&((MonShowData *)arg1)->btlMonObjData) = ov07_0221FDFC(*(BattleSystem **)arg1, 5);
            }
        } else if (!(BattleSystem_GetBattleSpecial(*(BattleSystem **)arg1) & 0x20)) {
            if ((BattleSystem_IsInitialized(*(BattleSystem **)arg1) == 1) && (((MonShowData *)arg1)->battlerType == 2)) {
                *((void **)&((MonShowData *)arg1)->btlMonObjData) = ov07_0221FDFC(*(BattleSystem **)arg1, 5);
            } else if (((MonShowData *)arg1)->battlerType == 0) {
                *((void **)&((MonShowData *)arg1)->btlMonObjData) = ov07_0221FDFC(*(BattleSystem **)arg1, 5);
            }
        }
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        break;
    case 1: {
#if defined(SDK_PORT) || defined(NONMATCHING)
        BallCapsuleConfig ballCapCfg;
        ballCapCfg.battlerType = ((MonShowData *)arg1)->battlerType;
        ballCapCfg.ballTypeOverride = 0;
        ballCapCfg.mon = BattleSystem_GetPartyMon(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (s32) ((MonShowData *)arg1)->selectedPartySlot);
        temp_r0_3 = ov07_02232694(HEAP_ID_BATTLE, &ballCapCfg);
#else
        u32 *temp_p = (u32 *)&sp4c;
        temp_p[0] = 0;
        temp_p[1] = 0;
        temp_p[2] = 0;
        sp4c[0] = ((MonShowData *)arg1)->battlerType;
        sp4c[2] = (u32)BattleSystem_GetPartyMon(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (s32) ((MonShowData *)arg1)->selectedPartySlot);
        temp_r0_3 = ov07_02232694(HEAP_ID_BATTLE, (BallCapsuleConfig *)sp4c);
#endif
        *((BallCapsuleSealEffect **)&((MonShowData *)arg1)->ballCapsuleSealEffect) = temp_r0_3;
        ov07_022329B0(temp_r0_3);
        ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        break;
    }
    case 2:
        if ((ov07_02233F20((((MonShowData *)arg1)->battlerData)->ballRotation) == 0) && (ov07_02232A04(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect)) == 1) && (ov07_02233EA0((((MonShowData *)arg1)->battlerData)->ballRotation) == 1)) {
            if (((MonShowData *)arg1)->battlerType == 4) {
                ((MonShowData *)arg1)->delay = (u8)(((MonShowData *)arg1)->delay + 1);
                if ((u32) ((MonShowData *)arg1)->delay < 0xCU) {

                } else {
                    ((MonShowData *)arg1)->delay = 0U;
                    goto block_22;
                }
            } else {
block_22:
                temp_r5 = BattleSystem_GetPokepicManager(*(BattleSystem **)arg1);
                NARC_ReadPokepicAnimScript((((MonShowData *)arg1)->battlerData)->narc, ((PokepicAnimScript *)sp24), ((MonShowData *)arg1)->species, (u16) ((MonShowData *)arg1)->battlerType);
                temp_r3 = ((MonShowData *)arg1)->battlerType;
                (((MonShowData *)arg1)->battlerData)->monSprite = ov12_022612A4(*(BattleSystem **)arg1, temp_r5, &((MonShowData *)arg1)->spriteTemplate, ov07_022377DC[temp_r3][0], ov07_022377F4[temp_r3][1], ov07_022377F4[temp_r3][2], (s32) ((MonShowData *)arg1)->yOffset, (s32) ((MonShowData *)arg1)->height, (s32) ((MonShowData *)arg1)->shadowXOffset, (s32) ((MonShowData *)arg1)->shadowSize, (s32) ((MonShowData *)arg1)->battler, ((PokepicAnimScript *)sp24), 0);
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC, 0);
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xD, 0);
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0x2C, 0);
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 6, 1);
                Pokepic_StartPaletteFade((((MonShowData *)arg1)->battlerData)->monSprite, 0x10, 0x10, 0, (s32) * (u16 *)(((u8 *)&ov12_0226D15A) + (((MonShowData *)arg1)->capturedBall * 2)));
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 6, 0);
                ov07_02232A44(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect));
                if (((MonShowData *)arg1)->face == 2) {
                    sub_0200602C(0x706U, 0x75);
                } else {
                    sub_0200602C(0x706U, -0x75);
                }
                if (((MonShowData *)arg1)->btlMonObjData != NULL) {
                    ov07_0221FE08(((MonShowData *)arg1)->btlMonObjData);
                    *((s32 *)&((MonShowData *)arg1)->btlMonObjData) = 0;
                }
                ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
                break;
            }
        } else {
            break;
        }
        break;
    case 3:
        if (ov07_02233E88((((MonShowData *)arg1)->battlerData)->ballRotation) != 1) {
            ((MonShowData *)arg1)->state = (u8)(((MonShowData *)arg1)->state + 1);
        }
    case 4:
        if ((Pokepic_GetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC) == 0x100) && (ov07_02232A54(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect)) == 0)) {
            if (((MonShowData *)arg1)->face == 2) {
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0x2D, 0);
            }
            temp_r3_2 = ((MonShowData *)arg1)->battlerData;
            ov12_02261F38(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battler, ((MonShowData *)arg1)->battlerType, temp_r3_2->monSprite, temp_r3_2->narc, (s32) ((MonShowData *)arg1)->species, (s32) ((MonShowData *)arg1)->formNum, (s32) ((MonShowData *)arg1)->face, *((s32 *)&((MonShowData *)arg1)->cryMod));
            Pokepic_StartPaletteFade((((MonShowData *)arg1)->battlerData)->monSprite, 0x10, 0, 0, (s32) * (u16 *)(((u8 *)&ov12_0226D15A) + (((MonShowData *)arg1)->capturedBall * 2)));
            ((MonShowData *)arg1)->state = 5U;
            break;
        }
        if (Pokepic_GetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC) >= 0x100) {
            Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC, 0x100);
            Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xD, 0x100);
            if (((MonShowData *)arg1)->face == 2) {
                Pokepic_SetAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0x2D, 0);
            }
            temp_r3_3 = ((MonShowData *)arg1)->battlerData;
            ov12_02261F38(*(BattleSystem **)arg1, ((MonShowData *)arg1)->battler, ((MonShowData *)arg1)->battlerType, temp_r3_3->monSprite, temp_r3_3->narc, (s32) ((MonShowData *)arg1)->species, (s32) ((MonShowData *)arg1)->formNum, (s32) ((MonShowData *)arg1)->face, *((s32 *)&((MonShowData *)arg1)->cryMod));
            Pokepic_StartPaletteFade((((MonShowData *)arg1)->battlerData)->monSprite, 0x10, 0, 1, (s32) * (u16 *)(((u8 *)&ov12_0226D15A) + (((MonShowData *)arg1)->capturedBall * 2)));
            ((MonShowData *)arg1)->state = 5U;
            break;
        }
        Pokepic_AddAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xC, 0x20);
        Pokepic_AddAttr((((MonShowData *)arg1)->battlerData)->monSprite, 0xD, 0x20);
        sub_0200914C((((MonShowData *)arg1)->battlerData)->monSprite, (s32) ((MonShowData *)arg1)->height);
        break;
    case 5:
        if (ov07_02232A54(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect)) == 0) {
            ((MonShowData *)arg1)->state = 6U;
            break;
        }
        break;
    case 6:
        if ((sub_02017068(ov12_0223B750(*(BattleSystem **)arg1), (s32) ((MonShowData *)arg1)->battler) == 1) && (Pokepic_IsAnimActive((((MonShowData *)arg1)->battlerData)->monSprite) == 0)) {
            ov07_02233ECC((((MonShowData *)arg1)->battlerData)->ballRotation);
            (((MonShowData *)arg1)->battlerData)->ballRotation = NULL;
            ov07_02232AB8(((BallCapsuleSealEffect *)((MonShowData *)arg1)->ballCapsuleSealEffect));
            if (((MonShowData *)arg1)->isShiny != 0) {
                *((void **)&((MonShowData *)arg1)->battleAnimSys) = ov07_0221BEDC(5);
                temp_r1_2 = ((MonShowData *)arg1)->battler;
                BattleController_SetMoveAnimation(*(BattleSystem **)arg1, NULL, &sp58, 1, 0xB, (s32)temp_r1_2, (s32)temp_r1_2, 0U);
                ov12_02261B80(*(BattleSystem **)arg1, ((void *)((MonShowData *)arg1)->battlerData), *((void **)&((MonShowData *)arg1)->battleAnimSys), &sp58);
                ((MonShowData *)arg1)->state = 7U;
                break;
            }
            ((MonShowData *)arg1)->state = 0xFFU;
            break;
        }
        break;
    case 7:
        ov07_0221C394(*((void **)&((MonShowData *)arg1)->battleAnimSys));
        if (ov07_0221C3B0(*((void **)&((MonShowData *)arg1)->battleAnimSys)) == 0) {
            ov07_0221C3C0(*((void **)&((MonShowData *)arg1)->battleAnimSys));
            ov07_0221BFE0(*((void **)&((MonShowData *)arg1)->battleAnimSys));
            ((MonShowData *)arg1)->state = 0xFFU;
            break;
        }
        break;
    default:
        sub_02005B58(0);
        BattleController_EmitClearCommand(*(BattleSystem **)arg1, (s32) ((MonShowData *)arg1)->battler, (s32) ((MonShowData *)arg1)->command);
        Heap_Free(arg1);
        SysTask_Destroy(arg0);
        break;
    }
}
