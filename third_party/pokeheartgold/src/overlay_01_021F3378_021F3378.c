#include "global.h"

#include "constants/sndseq.h"

#include "field_system.h"
#include "heap.h"
#include "map_object.h"
#include "overlay_01.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "sys_task_api.h"
#include "unk_02005D10.h"

typedef struct UnkStruct_ov01_021F3378 {
    FieldSystem *fieldSystem;
    BOOL *unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
} UnkStruct_ov01_021F3378;

static void ov01_021F3378(UnkStruct_ov01_021F3378 *param0);

static void ov01_021F3378(UnkStruct_ov01_021F3378 *param0) {
    switch (param0->unk_10) {
    case 0:
        param0->unk_10 = 2;
        break;
    case 2:
        param0->unk_10 = 1;
        break;
    case 1:
        param0->unk_10 = 3;
        break;
    case 3:
        param0->unk_10 = 0;
        break;
    }

    PlayerAvatar_SetFacingDirection(param0->fieldSystem->playerAvatar, param0->unk_10);
}

static void ov01_021F33B8(SysTask *param0, void *param1) {
    UnkStruct_ov01_021F3378 *v0 = param1;
    LocalMapObject *v1 = PlayerAvatar_GetMapObject(v0->fieldSystem->playerAvatar);
    VecFx32 v2;

    switch (v0->unk_08) {
    case 0:
        v0->unk_08 = 1;
        PlaySE(SEQ_SE_DP_TELE2);
    case 1:
        if (v0->unk_0C % 2) {
            ov01_021F3378(v0);
        }

        sub_0205F990(v1, &v2);
        v2.y = ((FX32_ONE * 2.2) + ((FX32_ONE / 2) * v0->unk_0C)) * v0->unk_0C;

        sub_0205F9A0(v1, &v2);
        v0->unk_0C++;

        if (v0->unk_0C == 20) {
            BeginNormalPaletteFade(FADE_SUB_THEN_MAIN, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
        } else if ((v0->unk_0C > 20) && IsPaletteFadeFinished()) {
            *v0->unk_04 = 1;
            Heap_Free(v0);
            SysTask_Destroy(param0);
        } else {
            break;
        }
    }
}

extern void ov01_021F92A0(LocalMapObject *object);

static void ov01_021F348C(SysTask *param0, void *param1) {
    UnkStruct_ov01_021F3378 *v0 = param1;
    LocalMapObject *v1 = PlayerAvatar_GetMapObject(v0->fieldSystem->playerAvatar);
    VecFx32 v2;
    int v3;

    switch (v0->unk_08) {
    case 0: {
        MapObject_UnpauseMovement(v1);
        sub_0205F990(v1, &v2);
        v3 = (20 - v0->unk_0C);
        v2.y = ((FX32_ONE * 2.2) + ((FX32_ONE / 2) * v3)) * v3;
        sub_0205F9A0(v1, &v2);
        ov01_021F92A0(v1);
    }

        PlaySE(SEQ_SE_DP_TELE2);
        v0->unk_08 = 1;
    case 1:
        if (v0->unk_0C % 2) {
            ov01_021F3378(v0);
        }

        sub_0205F990(v1, &v2);
        v3 = (20 - v0->unk_0C);
        v2.y = ((FX32_ONE * 2.2) + ((FX32_ONE / 2) * v3)) * v3;
        sub_0205F9A0(v1, &v2);
        v0->unk_0C++;

        if (v0->unk_0C == 2) {
            BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, HEAP_ID_FIELD1);
        }

        if (v0->unk_0C > 20) {
            v0->unk_08 = 2;
        }
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            PlayerAvatar_SetFacingDirection(v0->fieldSystem->playerAvatar, 1);
            *v0->unk_04 = 1;
            Heap_Free(v0);
            SysTask_Destroy(param0);
        }
        break;
    }
}

void ov01_021F35C4(FieldSystem *fieldSystem, int a1, int *a2) {
    UnkStruct_ov01_021F3378 *v0 = Heap_AllocAtEnd(HEAP_ID_FIELD1, sizeof(UnkStruct_ov01_021F3378));

    MI_CpuClear8(v0, sizeof(UnkStruct_ov01_021F3378));

    v0->fieldSystem = fieldSystem;
    v0->unk_04 = a2;
    v0->unk_10 = PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);

    if (a1) {
        SysTask_CreateOnMainQueue(ov01_021F33B8, v0, 100);
    } else {
        SysTask_CreateOnMainQueue(ov01_021F348C, v0, 100);
    }
}
