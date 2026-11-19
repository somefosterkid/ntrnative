#include "global.h"

#include "battle/overlay_12_0225A2A0.h"

#include "battle/battle_controller.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "overlay_07.h"
#include "sprite_system.h"
#include "sys_task_api.h"

s32 ov07_02233EFC(UnkBallData *, s32);

s32 ov07_0223449C(UnkBallData *, s32 arg1);

s32 ov07_022344A8(UnkBallData *, s16, s16);

s32 ov07_022344C0(UnkBallData *, s32);

s32 ov07_022344D0(UnkBallData *, u16);

extern s32 ov12_0226D1E8;

extern s32 ov12_0226D1EA;

void ov12_0225D644(SysTask *arg0, void *arg1_);

void ov12_0225D644(SysTask *arg0, void *arg1_) {
    UnkStruct_ov12_0225A2A0 *arg1 = arg1_;
    s16 sp0[2];
    OpponentData *temp_r0_5;
    OpponentData *temp_r4_3;
    SpriteManager *temp_r4_2;
    SpriteManager *temp_r4_4;
    s32 temp_r3;
    s32 temp_r6;
    u16 temp_r0_4;
    u32 temp_r4;
    u8 temp_r0;
    void *temp_r0_2;
    OpponentData *temp_r0_3;
    temp_r4 = BattleSystem_GetBattleType(arg1->battleSystem);
    temp_r0 = arg1->unkA;
    switch (temp_r0) {
    case 0:
        temp_r0_2 = arg1->unk4;
        if ((arg1->unkB) == 2) {
            ManagedSprite_OffsetPositionXY((arg1->unk4->managedSprite), 5, 0);
            ManagedSprite_GetPositionXY((arg1->unk4->managedSprite), &sp0[1], &sp0[0]);
            if (((s32)(sp0[1])) < 0x128) {
                return;
            }
            temp_r4_2 = BattleSystem_GetSpriteManager(arg1->battleSystem);
            Sprite_DeleteAndFreeResources((arg1->unk4->managedSprite));
            (arg1->unk4->managedSprite) = 0L;
            SpriteManager_UnloadCharObjById(temp_r4_2, ((arg1->unk4->battlerType)) + 0x4E2F);
            SpriteManager_UnloadPlttObjById(temp_r4_2, ((arg1->unk4->battlerType)) + 0x4E2A);
            SpriteManager_UnloadCellObjById(temp_r4_2, ((arg1->unk4->battlerType)) + 0x4E27);
            SpriteManager_UnloadAnimObjById(temp_r4_2, ((arg1->unk4->battlerType)) + 0x4E27);
            arg1->unkA = 2U;
            return;
        }
        ManagedSprite_SetPriority((arg1->unk4->managedSprite), 1);
        ManagedSprite_SetAnimationFrame((arg1->unk4->managedSprite), 0U);
        ManagedSprite_SetAnim((arg1->unk4->managedSprite), 1);
        ManagedSprite_SetAnimateFlag((arg1->unk4->managedSprite), 1);
        arg1->unkA = 1U;
        return;

    case 1:
        ManagedSprite_OffsetPositionXY((arg1->unk4->managedSprite), -5, 0);
        ManagedSprite_GetPositionXY((arg1->unk4->managedSprite), &sp0[1], &sp0[0]);
        temp_r0_3 = arg1->unk4;
        if (((temp_r0_3->ballRotation != NULL) && (2 & temp_r4)) && (!(8 & temp_r4))) {
            temp_r0_4 = ManagedSprite_GetAnimationFrame(temp_r0_3->managedSprite);
            temp_r6 = temp_r0_4 * 4;
            if ((*((s16 *)(temp_r6 + (((u8 *)(&ov12_0226D1E8)) + ((arg1->unkC) * 0x18))))) != 0x7FFF) {
                temp_r0_5 = BattleSystem_GetOpponentData(arg1->battleSystem, BattleSystem_GetBattlerIdPartner(arg1->battleSystem, (s32)(arg1->unk9)));
                temp_r4_3 = temp_r0_5;
                if ((temp_r0_4 == 3) && (ov07_02233F20(temp_r0_5->ballRotation) != 0)) {
                    temp_r3 = (arg1->unkC) * 0x18;
                    ov07_022344A8(temp_r4_3->ballRotation, (s16)((sp0[1]) + (*((s16 *)(temp_r6 + (((u8 *)(&ov12_0226D1E8)) + temp_r3))))), (s16)((sp0[0]) + (*((s16 *)(temp_r6 + (((u8 *)(&ov12_0226D1EA)) + temp_r3))))));
                    ov07_0223449C(temp_r4_3->ballRotation, 1);
                    ov07_02233EFC(temp_r4_3->ballRotation, 0);
                    ov07_022344C0(temp_r4_3->ballRotation, 1);
                    ov07_022344D0(temp_r4_3->ballRotation, 1);
                }
            }
        }
        if (((s32)(sp0[1])) <= (-0x28)) {
            temp_r4_4 = BattleSystem_GetSpriteManager(arg1->battleSystem);
            Sprite_DeleteAndFreeResources((arg1->unk4->managedSprite));
            (arg1->unk4->managedSprite) = 0L;
            SpriteManager_UnloadCharObjById(temp_r4_4, ((arg1->unk4->battlerType)) + 0x4E2F);
            SpriteManager_UnloadPlttObjById(temp_r4_4, ((arg1->unk4->battlerType)) + 0x4E2A);
            SpriteManager_UnloadCellObjById(temp_r4_4, ((arg1->unk4->battlerType)) + 0x4E27);
            SpriteManager_UnloadAnimObjById(temp_r4_4, ((arg1->unk4->battlerType)) + 0x4E27);
            arg1->unkA = 2U;
            return;
        }
        break;

    case 2:
        BattleController_EmitClearCommand(arg1->battleSystem, (s32)(arg1->unk9), (s32)(arg1->unk8));
        Heap_Free(arg1);
        SysTask_Destroy(arg0);
        return;
    }
}
