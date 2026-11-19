#include "global.h"

#include "battle/overlay_12_0225A2A0.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_07.h"
#include "sprite_system.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

s32 ov07_02233EFC(UnkBallData *, s32);

s32 ov07_0223449C(UnkBallData *, s32);

s32 ov07_022344A8(UnkBallData *, s16, s16);

s32 ov07_022344C0(UnkBallData *, s32);

s32 ov07_022344C4(UnkBallData *, u16);

s32 ov07_022344D0(UnkBallData *, u16);

s32 ov07_022344DC(UnkBallData *, s32);

extern s32 ov12_0226D1E8;

extern s32 ov12_0226D1EA;

void ov12_0225D138(SysTask *arg0, void *arg1);

void ov12_0225D138(SysTask *arg0, void *arg1_) {
    UnkStruct_ov12_0225A2A0 *arg1 = arg1_;
    BallThrow sp8;
    s16 sp4[2];
    OpponentData *temp_r0_4;
    OpponentData *temp_r0_6;
    SpriteManager *temp_r5;
    SpriteManager *temp_r5_3;
    UnkBallData *temp_r0_8;
    s16 temp_r1_2;
    s32 temp_r0_5;
    s32 temp_r0_9;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 temp_r6_2;
    s32 temp_r5_2;
    s32 temp_r5_4;
    s32 temp_r5_5;
    u16 temp_r0_7;
    u32 temp_r0_2;
    u32 temp_r6;
    u8 temp_r0;
    OpponentData *temp_r1;

    temp_r6 = BattleSystem_GetBattleType(arg1->battleSystem);
    temp_r0 = arg1->unkA;
    switch ((u32)temp_r0) {
    case 0:
        temp_r0_2 = arg1->unk10;
        if (temp_r0_2 == 0) {
            arg1->unkA = 1U;
            break;
        }
        switch (temp_r0_2) {
        default:
        case 0:
        case 1:
            (sp8.type) = 0xF;
            arg1->unk18 = 3;
            (sp8.ball) = 5;
            break;
        case 2:
            (sp8.type) = 0xC;
            arg1->unk18 = 0;
            (sp8.ball) = 0x401;
            break;
        case 3:
            (sp8.type) = 0xD;
            arg1->unk18 = 0;
            (sp8.ball) = 0x402;
            break;
        case 4:
            (sp8.type) = 0xF;
            arg1->unk18 = 3;
            (sp8.ball) = 0x400;
            break;
        }
        sp8.heapID = HEAP_ID_BATTLE;
        (sp8.mode) = 4;
        (sp8.target) = (s32) arg1->unk9;
        sp8.spriteSystem = BattleSystem_GetSpriteSystem(arg1->battleSystem);
        sp8.paletteData = BattleSystem_GetPaletteData(arg1->battleSystem);
        (sp8.surface) = 0;
        sp8.battleSystem = arg1->battleSystem;
        (arg1->unk4->ballRotation) = ov07_02233DB8((BallThrow *)&sp8);
        ov07_022344C4((arg1->unk4->ballRotation), 0x64);
        ov07_022344D0((arg1->unk4->ballRotation), 2);
        ov07_022344C0((arg1->unk4->ballRotation), 0);
        ManagedSprite_SetAnimationFrame((arg1->unk4->managedSprite), 0U);
        ManagedSprite_SetAnim((arg1->unk4->managedSprite), 1);
        ManagedSprite_SetAnimateFlag((arg1->unk4->managedSprite), 1);
        arg1->unkA = 3U;
        break;
    case 1:
        if (arg1->unkB == 2) {
            ManagedSprite_OffsetPositionXY((arg1->unk4->managedSprite), 5, 0);
            ManagedSprite_GetPositionXY((arg1->unk4->managedSprite), &sp4[1], &sp4[0]);
            if (((s32) sp4[1] >= 0xA0) && (ov07_02233F20((arg1->unk4->ballRotation)) != 0)) {
                ov07_0223449C((arg1->unk4->ballRotation), 1);
                ov07_02233EFC((arg1->unk4->ballRotation), 0);
                if ((2 & temp_r6) != 0 && (8 & temp_r6) == 0) {
                    temp_r0_4 = BattleSystem_GetOpponentData(arg1->battleSystem, BattleSystem_GetBattlerIdPartner(arg1->battleSystem, (s32) arg1->unk9));
                    ov07_0223449C(temp_r0_4->ballRotation, 1);
                    ov07_02233EFC(temp_r0_4->ballRotation, 0);
                    ov07_022344DC(temp_r0_4->ballRotation, 0xC);
                    break;
                }
            } else {
                if ((s32) sp4[1] < 0x128) {
                    break;
                }
                temp_r5 = BattleSystem_GetSpriteManager(arg1->battleSystem);
                Sprite_DeleteAndFreeResources((arg1->unk4->managedSprite));
                (arg1->unk4->managedSprite) = NULL;
                SpriteManager_UnloadCharObjById(temp_r5, (arg1->unk4->battlerType) + 0x4E2F);
                SpriteManager_UnloadPlttObjById(temp_r5, (arg1->unk4->battlerType) + 0x4E2A);
                SpriteManager_UnloadCellObjById(temp_r5, (arg1->unk4->battlerType) + 0x4E27);
                SpriteManager_UnloadAnimObjById(temp_r5, (arg1->unk4->battlerType) + 0x4E27);
                arg1->unkA = 6U;
                break;
            }
        } else {
            ManagedSprite_SetAnimationFrame((arg1->unk4->managedSprite), 0U);
            ManagedSprite_SetAnim((arg1->unk4->managedSprite), 1);
            ManagedSprite_SetAnimateFlag((arg1->unk4->managedSprite), 1);
            arg1->unkA = 2U;
            break;
        }
        break;
    case 2:
        ManagedSprite_OffsetPositionXY((arg1->unk4->managedSprite), -5, 0);
        ManagedSprite_GetPositionXY((arg1->unk4->managedSprite), &sp4[1], &sp4[0]);
        temp_r1 = arg1->unk4;
        if (temp_r1->ballRotation != NULL) {
            temp_r0_5 = (s32)ManagedSprite_GetAnimationFrame(temp_r1->managedSprite);
            temp_r5_2 = temp_r0_5 * 4;
            if (*(s16 *)(temp_r5_2 + (((u8 *)&ov12_0226D1E8) + (arg1->unkC * 0x18))) != 0x7FFF) {
                ov07_0223449C((arg1->unk4->ballRotation), 1);
                temp_r3 = arg1->unkC * 0x18;
                ov07_022344A8((arg1->unk4->ballRotation), (s16)(sp4[1] + *(s16 *)(temp_r5_2 + (((u8 *)&ov12_0226D1E8) + temp_r3))), (s16)(sp4[0] + *(s16 *)(temp_r5_2 + (((u8 *)&ov12_0226D1EA) + temp_r3))));
                if ((temp_r0_5 == 3) && (ov07_02233F20((arg1->unk4->ballRotation)) != 0)) {
                    ov07_02233EFC((arg1->unk4->ballRotation), 0);
                    ov07_022344C0((arg1->unk4->ballRotation), 1);
                    if ((2 & temp_r6) != 0 && (8 & temp_r6) == 0) {
                        temp_r0_6 = BattleSystem_GetOpponentData(arg1->battleSystem, BattleSystem_GetBattlerIdPartner(arg1->battleSystem, (s32) arg1->unk9));
                        temp_r3_2 = arg1->unkC * 0x18;
                        ov07_022344A8(temp_r0_6->ballRotation, (s16)(sp4[1] + *(s16 *)(temp_r5_2 + (((u8 *)&ov12_0226D1E8) + temp_r3_2))), (s16)(sp4[0] + *(s16 *)(temp_r5_2 + (((u8 *)&ov12_0226D1EA) + temp_r3_2))));
                        ov07_0223449C(temp_r0_6->ballRotation, 1);
                        ov07_02233EFC(temp_r0_6->ballRotation, 0);
                        ov07_022344C0(temp_r0_6->ballRotation, 1);
                    }
                }
            }
        }
        if ((s32) sp4[1] > -0x28) {
            break;
        }
        temp_r5_3 = BattleSystem_GetSpriteManager(arg1->battleSystem);
        Sprite_DeleteAndFreeResources((arg1->unk4->managedSprite));
        (arg1->unk4->managedSprite) = NULL;
        SpriteManager_UnloadCharObjById(temp_r5_3, (arg1->unk4->battlerType) + 0x4E2F);
        SpriteManager_UnloadPlttObjById(temp_r5_3, (arg1->unk4->battlerType) + 0x4E2A);
        SpriteManager_UnloadCellObjById(temp_r5_3, (arg1->unk4->battlerType) + 0x4E27);
        SpriteManager_UnloadAnimObjById(temp_r5_3, (arg1->unk4->battlerType) + 0x4E27);
        arg1->unkA = 6U;
        break;
    case 3:
        ManagedSprite_GetPositionXY((arg1->unk4->managedSprite), &sp4[1], &sp4[0]);
        temp_r0_7 = ManagedSprite_GetAnimationFrame((arg1->unk4->managedSprite));
        if (temp_r0_7 == 4) {
            arg1->unk14 = 8;
            arg1->unkA = 4U;
            break;
        }
        temp_r0_8 = (arg1->unk4->ballRotation);
        if (temp_r0_8 != NULL) {
            temp_r6_2 = temp_r0_7 * 4;
            temp_r5_4 = arg1->unkC * 0x18;
            temp_r1_2 = *(s16 *)(temp_r6_2 + (((u8 *)&ov12_0226D1E8) + temp_r5_4));
            if (temp_r1_2 != 0x7FFF) {
                ov07_022344A8(temp_r0_8, (s16)(sp4[1] + temp_r1_2), (s16)(sp4[0] + *(s16 *)(temp_r6_2 + (((u8 *)&ov12_0226D1EA) + temp_r5_4))));
                if (temp_r0_7 == 3) {
                    temp_r5_5 = arg1->unk18;
                    if (temp_r5_5 != ov07_02233F20((arg1->unk4->ballRotation))) {
                        ov07_02233EFC((arg1->unk4->ballRotation), temp_r5_5);
                        ov07_022344D0((arg1->unk4->ballRotation), 1);
                        ov07_022344C0((arg1->unk4->ballRotation), 1);
                        if (arg1->unk18 != 3) {
                            PlaySE(0x70AU);
                            break;
                        }
                    }
                }
            }
        }
        break;
    case 4:
        temp_r0_9 = arg1->unk14 - 1;
        arg1->unk14 = temp_r0_9;
        if (temp_r0_9 == 0) {
            if (arg1->unk18 == 3) {
                arg1->unkA = 6U;
                break;
            }
            arg1->unkA = 5U;
            break;
        }
        break;
    case 5:
        if (ov07_02233E88((arg1->unk4->ballRotation)) != 1) {
            ov07_02233ECC((arg1->unk4->ballRotation));
            (arg1->unk4->ballRotation) = NULL;
            arg1->unkA = 6U;
            break;
        }
        break;
    case 6:
        BattleController_EmitClearCommand(arg1->battleSystem, (s32) arg1->unk9, (s32) arg1->unk8);
        Heap_Free(arg1);
        SysTask_Destroy(arg0);
        break;
    }
}
