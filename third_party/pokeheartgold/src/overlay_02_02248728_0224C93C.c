#include "global.h"
struct FieldSystem;

#include "constants/heap.h"

#include "field_types_def.h"
#include "field_system.h"
#include "player_avatar.h"
#include "screen_fade.h"
#include "unk_02062108.h"
#include "field_warp_tasks.h"
#include "save_local_field_data.h"
#include "overlay_01_021FCD2C.h"
#include "overlay_02.h"

extern u32 ov02_02253794;

u32 ov02_0224C93C(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

s32 ov02_0224CA38(TaskManager *arg0, FieldSystem *arg1, FieldMoveTaskEnvironment *arg2);

extern void ov02_0224CA58(u8 arr[], u8 length, u8 iterations);

struct PasswordWordBank;
typedef struct PasswordWordBank PasswordWordBank;
u32 WallpaperPasswordBank_GetCount(const PasswordWordBank *wordBank);
s16 WallpaperPasswordBank_GetIndexOfWord(const PasswordWordBank *wordBank, s32 word);
int ov02_0224CAB8(const PasswordWordBank *wordBank, u16 trainerID, u16 word1, u16 word2, u16 word3, u16 word4);

u32 ov02_0224C93C(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;
    s32 iVar3;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->playerMovement);
    pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
    param_3->playerMovement = pEVar2;
    if (param_3->followMonActive != 0) {
        EventObjectMovementMan_Delete(param_3->followMonMovement);
        pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject, (MovementScriptCommand *)&ov02_02253794);
        param_3->followMonMovement = pEVar2;
    }
    iVar3 = param_3->frame;
    param_3->frame = iVar3 + 1;
    if (iVar3 + 1 < 8) {
        return 0;
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0x7fff, 6, 1, HEAP_ID_FIELD1);
    param_3->state = param_3->state + 1;
    return 0;
}

u32 ov02_0224C9B8(u32 param_1, struct FieldSystem *param_2, FieldMoveTaskEnvironment *param_3);

u32 ov02_0224C9B8(u32 param_1, FieldSystem *param_2, FieldMoveTaskEnvironment *param_3) {
    BOOL BVar1;
    EventObjectMovementMan *pEVar2;
    s32 sVar3;

    BVar1 = EventObjectMovementMan_IsFinish(param_3->playerMovement);
    if (BVar1 == 1) {
        EventObjectMovementMan_Delete(param_3->playerMovement);
        pEVar2 = EventObjectMovementMan_Create(param_3->mapObject, (MovementScriptCommand *)&ov02_02253794);
        param_3->playerMovement = pEVar2;
        if (param_3->followMonActive != 0) {
            EventObjectMovementMan_Delete(param_3->followMonMovement);
            pEVar2 = EventObjectMovementMan_Create(param_2->followMon.mapObject,
                (MovementScriptCommand *)&ov02_02253794);
            param_3->followMonMovement = pEVar2;
        }
    }
    BVar1 = IsPaletteFadeFinished();
    if (BVar1 == 0) {
        return 0;
    }
    EventObjectMovementMan_Delete(param_3->playerMovement);
    sVar3 = PlayerAvatar_GetState(param_2->playerAvatar);
    if (sVar3 != 2) {
        ov02_0224DE08(param_3->unk_2C);
        param_3->unk_2C = 0;
    }
    if (param_3->followMonActive != 0) {
        EventObjectMovementMan_Delete(param_3->followMonMovement);
    }
    ov01_021FCD78(param_3->cameraTask);
    param_3->state = param_3->state + 1;
    return 1;
}

s32 ov02_0224CA38(TaskManager *taskManager, FieldSystem *arg1, FieldMoveTaskEnvironment *arg2) {
    sub_02053B04(taskManager, LocalFieldData_GetSpecialSpawnWarpPtr(Save_LocalFieldData_Get(arg1->saveData)), arg2->type);
    return 2;
}

void ov02_0224CA58(u8 arr[], u8 length, u8 iterations) {
    while (iterations--) {
        u8 bit = arr[length - 1] & 1;
        int i;

        for (i = length - 1; i > 0; i--) {
            arr[i] >>= 1;
            arr[i] |= ((arr[i - 1] & 1) << 7);
        }

        arr[i] >>= 1;
        arr[i] |= bit << 7;
    }
}

int ov02_0224CAB8(const PasswordWordBank *wordBank, u16 trainerID, u16 word1, u16 word2, u16 word3, u16 word4) {
    int wordBankLength = WallpaperPasswordBank_GetCount(wordBank);

    s16 wordIndices[4];
    wordIndices[0] = WallpaperPasswordBank_GetIndexOfWord(wordBank, word1);
    wordIndices[1] = WallpaperPasswordBank_GetIndexOfWord(wordBank, word2);
    wordIndices[2] = WallpaperPasswordBank_GetIndexOfWord(wordBank, word3);
    wordIndices[3] = WallpaperPasswordBank_GetIndexOfWord(wordBank, word4);

    u8 bits[4];

    int i;

    for (i = 0; i < 4; i++) {
        if (wordIndices[i] < 0) {
            return -1;
        }

        if (i > 0) {
            if (wordIndices[i] >= wordIndices[i - 1]) {
                int diff = wordIndices[i] - wordIndices[i - 1];

                if (diff > 0xff) {
                    return -1;
                }

                bits[i] = diff;
            } else {
                int diff = wordBankLength - (wordIndices[i - 1] - wordIndices[i]);

                if (diff > 0xff) {
                    return -1;
                }

                bits[i] = diff;
            }
        } else {
            if (wordIndices[0] > 0xff) {
                return -1;
            }

            bits[0] = wordIndices[0];
        }
    }

    ov02_0224CA58(bits, (sizeof(bits) / sizeof(*(bits))), 5);

    for (i = 0; i < 3; i++) {
        bits[i] ^= ((bits[3] >> 4) | (bits[3] & 0xF0));
    }

    ov02_0224CA58(bits, 3, bits[3] & 0xF);

    if ((bits[0] & 0xF) >= 8) {
        return -1;
    }

    bits[1] ^= bits[0];
    bits[2] ^= bits[0];

    if (((bits[1] << 8) | bits[2]) == trainerID && ((bits[0] & 0xF0) >> 4) == 6 && bits[3] == (((bits[0] + bits[1]) * bits[2]) & 0xFF)) {
        return bits[0] & 0xF;
    }

    return -1;
}
