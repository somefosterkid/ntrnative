#include "global.h"

#include "bg_window.h"
#include "heap.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "sound_02004A44.h"
#include "yes_no_prompt.h"
#include "main.h"

typedef struct UnkStruct_ov74_02233230_OverlayManager_CreateAndGetData {
    u8 unk_0[0x10];
    SaveData *unk_10;
    PlayerProfile *unk_14;
    Options *unk_18;
    u32 unk_1C;
    BgConfig *unk_20;
    u8 unk_24[0xE868];
    YesNoPrompt *unk_E88C;
    u8 unk_E890[0xC];
    u8 unk_E89C;
    u8 unk_E89D[0x3D6B];
    String *unk_12608;
    String *unk_1260C;
} UnkStruct_ov74_02233230_OverlayManager_CreateAndGetData;

s32 ov74_02236074(void);

extern void *ov74_0223D338;

s32 ov74_02233230(OverlayManager *arg0);

s32 ov74_02233230(OverlayManager *man) {
    Options *options;
    SaveData *saveData;
    UnkStruct_ov74_02233230_OverlayManager_CreateAndGetData *v0;

    Heap_Create(HEAP_ID_3, HEAP_ID_76, 0x38000U);
    v0 = OverlayManager_CreateAndGetData(man, sizeof(UnkStruct_ov74_02233230_OverlayManager_CreateAndGetData), HEAP_ID_76);
    memset(v0, 0, sizeof(UnkStruct_ov74_02233230_OverlayManager_CreateAndGetData));
    v0->unk_20 = BgConfig_Alloc(HEAP_ID_76);
    v0->unk_E88C = YesNoPrompt_Create(HEAP_ID_76);
    sub_0200FBF4(PM_LCD_TOP, 0U);
    sub_0200FBF4(PM_LCD_BOTTOM, 0U);
    saveData = ((UnkStruct_02111868_sub *)OverlayManager_GetArgs(man))->saveData;
    v0->unk_10 = saveData;
    v0->unk_14 = Save_PlayerData_GetProfile(saveData);
    options = Save_PlayerData_GetOptionsAddr(v0->unk_10);
    v0->unk_18 = options;
    v0->unk_1C = Options_GetFrame(options);
    v0->unk_12608 = String_New(0x180U, HEAP_ID_76);
    v0->unk_1260C = String_New(0x180U, HEAP_ID_76);
    Sound_SetSceneAndPlayBGM(9U, 0x47EU, 1);
    if (OS_IsTickAvailable() == 0) {
        OS_InitTick();
    }
    ov74_02236074();
    ov74_0223D338 = (void *)&v0->unk_E89C;
    return 1;
}
