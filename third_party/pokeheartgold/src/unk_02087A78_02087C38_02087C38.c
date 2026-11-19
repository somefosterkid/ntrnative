#include "global.h"

#include "constants/heap.h"

#include "battle/battle_setup.h"

#include "bag_cursor.h"
#include "field_bgm.h"
#include "field_types_def.h"
#include "game_stats.h"
#include "heap.h"
#include "launch_application.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "save.h"
#include "sound_02004A44.h"
#include "trainer_data.h"
#include "unk_0202FBCC.h"
#include "unk_020915B0.h"
#include "unk_struct_0208bc3c.h"

s32 Sound_SetFieldBGM(u16 param_1);

s32 ov40_02244920(void *, BattleSetup *, enum HeapID);

s32 sub_02005B68(u8);

void *sub_0202FC5C(void);

s32 sub_0202FC90(SaveData *, enum HeapID, s32 *, BattleSetup *, s32);

s32 sub_020304F0(BattleSetup *, SaveData *);

s32 sub_02087E34(int);

FS_EXTERN_OVERLAY(OVY_39);

FS_EXTERN_OVERLAY(OVY_40);

s32 sub_02087C38(void *arg0, enum HeapID arg1);

s32 sub_02087C38(void *arg0, enum HeapID arg1) {
    s32 sp4;
    FieldSystem *temp_r0_2;
    FieldSystem *temp_r0_3;
    s32 temp_r0;
    u16 temp_r4;
    void *temp_r2;

    temp_r0 = *(s32 *)arg0;
    switch (temp_r0) {
    case 0:
        if (*(s32 *)(((UnkStruct_0208BC3C *)arg0)->unk_14) != 0) {
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_39));
            UnloadOVY38();
        }
        *(s32 *)arg0 = (s32)(*(s32 *)arg0 + 1);
        break;
    case 1:
        *((BattleSetup **)&((UnkStruct_0208BC3C *)arg0)->unk_10) = BattleSetup_New(arg1, 0U);
        if (sub_0202FC48() == 0) {
            sub_0202FC90(((UnkStruct_0208BC3C *)arg0)->saveData, arg1, &sp4, ((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10), *(s32 *)((u8 *)(((UnkStruct_0208BC3C *)arg0)->unk_14) + 0x86C));
        } else {
            sub_020304F0(((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10), ((UnkStruct_0208BC3C *)arg0)->saveData);
            sp4 = 1;
        }
        (((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10))->bagCursor = BagCursor_New(arg1);
        (((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10))->gameStats = Save_GameStats_Get(((UnkStruct_0208BC3C *)arg0)->saveData);
        if (HandleLoadOverlay(FS_OVERLAY_ID(OVY_40), OVY_LOAD_ASYNC) == 1) {
            ov40_02244920(sub_0202FC5C(), ((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10), arg1);
            UnloadOverlayByID(FS_OVERLAY_ID(OVY_40));
        }
        *(s32 *)((u8 *)(((UnkStruct_0208BC3C *)arg0)->unk_14) + 0x874) = 1;
        if (sp4 != 1) {
            Heap_Free((((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10))->bagCursor);
            BattleSetup_Delete(((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10));
            *(s32 *)arg0 = 0;
            return 1;
        }
        *(s32 *)arg0 = (s32)(*(s32 *)arg0 + 1);
        break;
    case 2:
        GF_SndHandleSetPlayerVolume(1U, 0x7FU);
        GF_SndHandleSetPlayerVolume(7U, 0x7FU);
        sub_02005B68(1);
        if (sub_02087E34((((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10))->trainer[1].data.trainerClass) == 1) {
            Sound_SetSceneAndPlayBGM(5U, 0x47BU, 1);
        } else {
            Sound_SetSceneAndPlayBGM(5U, 0x45DU, 1);
        }
        *((OverlayManager **)&((UnkStruct_0208BC3C *)arg0)->appMan) = OverlayManager_New(&gOverlayTemplate_Battle, ((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10), arg1);
        *(s32 *)arg0 = (s32)(*(s32 *)arg0 + 1);
        break;
    default:
        if (OverlayManager_Run(((OverlayManager *)((UnkStruct_0208BC3C *)arg0)->appMan)) != 0) {
            *(s32 *)((u8 *)(((UnkStruct_0208BC3C *)arg0)->unk_14) + 0x874) = (s32)(((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10))->unk1C4;
            temp_r2 = ((UnkStruct_0208BC3C *)arg0)->unk_14;
            if ((*(s32 *)temp_r2 != 0) && (*(s32 *)((u8 *)temp_r2 + 0x874) == 0)) {
                **(s8 **)((u8 *)temp_r2 + 0x878) = 1;
            }
            Heap_Free((((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10))->bagCursor);
            BattleSetup_Delete(((BattleSetup *)((UnkStruct_0208BC3C *)arg0)->unk_10));
            OverlayManager_Delete(((OverlayManager *)((UnkStruct_0208BC3C *)arg0)->appMan));
            sub_02005B68(0);
            Sound_SetScene(0);
            temp_r0_2 = ((FieldSystem *)((UnkStruct_0208BC3C *)arg0)->fieldSystem);
            temp_r4 = FieldBGM_GetEffective(temp_r0_2, temp_r0_2->location->mapId);
            temp_r0_3 = ((FieldSystem *)((UnkStruct_0208BC3C *)arg0)->fieldSystem);
            Sound_SetFieldBGM(FieldBGM_GetForMapHeader(temp_r0_3, temp_r0_3->location->mapId));
            sub_02055198(NULL, temp_r4);
            *(s32 *)arg0 = 0;
            if (*(s32 *)(((UnkStruct_0208BC3C *)arg0)->unk_14) != 0) {
                LoadOVY38();
                HandleLoadOverlay(FS_OVERLAY_ID(OVY_39), OVY_LOAD_ASYNC);
            }
            return 1;
        }
        break;
    }
    return 0;
}
