#include "global.h"

#include "heap.h"
#include "message_format.h"
#include "player_data.h"
#include "unk_0202C034.h"
#include "overlay_43_palpad.h"

void ov43_0222AB20(struct PalPadGfx *arg0, SaveData *arg1, s32 arg2, enum HeapID arg3);

void ov43_0222AC18(PalPadGfx *arg0, u32 arg1);

void ov43_0222AB20(PalPadGfx *arg0, SaveData *saveData, s32 a1, enum HeapID heapID) {
    PlayerProfile *playerProfile;
    WiFiList *a0;

    a0 = sub_0202C6F4(saveData);
    playerProfile = PlayerProfile_New(heapID);
    Save_Profile_PlayerName_Set(playerProfile, sub_0202C254(a0, a1));
    BufferPlayersName(arg0->msgFormat, 0U, playerProfile);
    Heap_Free(playerProfile);
}

void ov43_0222AB5C(struct PalPadGfx *arg0, SaveData *arg1, s32 arg2, enum HeapID arg3);

void ov43_0222AB5C(PalPadGfx *arg0, SaveData *saveData, s32 a1, enum HeapID heapID) {
    PlayerProfile *playerProfile;

    playerProfile = PlayerProfile_New(heapID);
    Save_Profile_PlayerName_Set(playerProfile, sub_0202C298(sub_0202C6F4(saveData), a1));
    BufferPlayersName(arg0->msgFormat, 0U, playerProfile);
    Heap_Free(playerProfile);
}

s32 ov43_0222AB94(struct PalPadGfx *arg0, SaveData *arg1, s32 arg2);

s32 ov43_0222AB94(PalPadGfx *arg0, SaveData *saveData, s32 a1) {
    WiFiList *a0;
    s32 num;
    s32 v0;

    a0 = sub_0202C6F4(saveData);
    v0 = 1;
    num = sub_0202C090(a0, a1, 6);
    if (num == 0) {
        v0 = 0;
    }
    BufferIntegerAsString(arg0->msgFormat, 2U, num, 2U, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(arg0->msgFormat, 0U, sub_0202C090(a0, a1, 4), 4U, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferMonthNameAbbr(arg0->msgFormat, 1U, (u32)sub_0202C090(a0, a1, 5));
    return v0;
}

void ov43_0222ABFC(PalPadGfx *arg0, s32 arg1);

void ov43_0222ABFC(PalPadGfx *arg0, s32 num) {
    BufferIntegerAsString(arg0->msgFormat, 0U, num, 4U, PRINTING_MODE_RIGHT_ALIGN, 1);
}

void ov43_0222AC18(PalPadGfx *arg0, u32 activityId) {
    BufferWiFiPlazaActivityName(arg0->msgFormat, 0U, activityId);
}
