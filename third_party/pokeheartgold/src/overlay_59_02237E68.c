#include "global.h"
#include "overlay_59_apricorn_app.h"
#include "heap.h"
#include "overlay_manager.h"
#include "menu_input_state.h"
#include "options.h"
#include "player_data.h"
#include "unk_02031B0C.h"

s32 ov59_02237F28(Ov59ApricornApp *);

s32 ov59_02237F74(Ov59ApricornApp *arg0);

s32 ov59_02237E68(OverlayManager *arg0);

s32 ov59_02237E68(OverlayManager *arg0) {
    Ov59ApricornApp *temp_r4;

    temp_r4 = OverlayManager_GetData(arg0);
    if (ov59_02237F74(temp_r4) == 0) {
        return 0;
    }
    ov59_02237F28(temp_r4);
    OverlayManager_FreeData(arg0);
    Heap_Destroy(HEAP_ID_134);
    return 1;
}

s32 ov59_02238768(Ov59ApricornApp *);

void ov59_02237E94(Ov59ApricornApp *arg0);

void ov59_02237E94(Ov59ApricornApp *arg0) {
    Options *temp_r0;

    arg0->menuInputState = MenuInputStateMgr_GetState(arg0->parent->args->menuInputStatePtr);
    temp_r0 = Save_PlayerData_GetOptionsAddr(arg0->parent->args->saveData);
    arg0->textFrameDelay = Options_GetTextFrameDelay(temp_r0);
    arg0->frame = (s8) Options_GetFrame(temp_r0);
    arg0->mode = (u8) arg0->parent->unk_18;
    arg0->unk_38 = (s32) arg0->parent->args->steps;
    arg0->apricornBox = Save_ApricornBox_Get(arg0->parent->args->saveData);
    ov59_02238768(arg0);
    if (arg0->mode == 3) {
        arg0->unk_49 = 2;
        arg0->unk_4A = 0;
        arg0->unk_4B = (u8) arg0->parent->unk_1A;
        arg0->unk_4C = (u8) arg0->parent->unk_1B;
        arg0->mode = 1U;
        arg0->parent->unk_18 = 1U;
    }
}
