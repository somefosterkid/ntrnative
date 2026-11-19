#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "heap.h"
#include "options.h"
#include "overlay_manager.h"
#include "player_data.h"
#include "save.h"
#include "sound_02004A44.h"

#include "overlay_87.h"
#include "overlay_87_scratch_off_card_app.h"

void ov87_021E68A4(void);

void ov87_021E68DC(ScratchOffCardApp *app);

void ov87_021E7334(ScratchOffCardApp *app);

BOOL ScratchOffCards_Init(OverlayManager *man, int *state) {
    ScratchOffCardApp *app;
    ScratchOffCardsArgs *args;
    s32 i;

    ov87_021E68A4();
    Heap_Create(HEAP_ID_3, HEAP_ID_122, 0x48000);
    app = OverlayManager_CreateAndGetData(man, sizeof(ScratchOffCardApp), HEAP_ID_122);
    memset(app, 0, sizeof(ScratchOffCardApp));
    app->bgConfig = BgConfig_Alloc(HEAP_ID_122);
    app->appMan = man;
    args = OverlayManager_GetArgs(man);
    app->saveData = args->saveData;
    app->dummy = args->unk_04;
    app->dummy4 = &args->unk_14;
    app->options = Save_PlayerData_GetOptionsAddr(app->saveData);
    app->wonItems = args->unk_08;
    app->wonItemsCount = args->unk_0E;
    ov87_021E7334(app);
    for (i = 0; i < 3; i++) {
        app->wonItems[i] = 0;
        app->wonItemsCount[i] = 0;
    }
    app->highlightPairPalette = 0;
    ov87_021E68DC(app);
    *state = 0;
    Sound_SetSceneAndPlayBGM(0x42, 0, 0);
    return 1;
}
