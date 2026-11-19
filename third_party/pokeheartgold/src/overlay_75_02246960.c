#include "global.h"

#include "heap.h"
#include "overlay_75.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "sound_02004A44.h"

typedef struct UnkStruct_ov75_02246960_OverlayManager_CreateAndGetData {
    u8 unk_0[0x4];
    SaveData *unk_4;
    Options *unk_8;
    u8 unk_C[0x7C];
    s32 unk_88;
    u8 unk_8C[0x84];
    String *unk_110;
    String *unk_114;
    u8 unk_118[0x4];
} UnkStruct_ov75_02246960_OverlayManager_CreateAndGetData;

s32 ov75_02246960(OverlayManager *man) {
    SaveData *saveData;
    UnkStruct_ov75_02246960_OverlayManager_CreateAndGetData *dest;

    Heap_Create(HEAP_ID_3, HEAP_ID_115, 0x28000U);
    Heap_Create(HEAP_ID_DEFAULT, HEAP_ID_89, 0x570U);
    dest = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov75_02246960_OverlayManager_CreateAndGetData), HEAP_ID_115);
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov75_02246960_OverlayManager_CreateAndGetData));
    saveData = *(SaveData **)((u8 *)(OverlayManager_GetArgs(man)) + 8);
    dest->unk_4 = saveData;
    dest->unk_8 = Save_PlayerData_GetOptionsAddr(saveData);
    dest->unk_110 = String_New(0x64U, HEAP_ID_115);
    dest->unk_114 = String_New(0x64U, HEAP_ID_115);
    Sound_SetSceneAndPlayBGM(0x11U, 0x47DU, 1);
    dest->unk_88 = 0;
    return 1;
}
