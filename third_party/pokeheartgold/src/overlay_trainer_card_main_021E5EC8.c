#include "global.h"

#include "heap.h"
#include "menu_input_state.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "sound_02004A44.h"
#include "system.h"
#include "trainer_card_main.h"
#include "unk_020210A0.h"

s32 ov51_021E6644(BgConfig *);

s32 ov51_021E6C00(void);

s32 TrainerCardMainApp_Exit(OverlayManager *manager, s32 *state);

s32 TrainerCardMainApp_Exit(OverlayManager *manager, s32 *state) {
    TrainerCardMain *v0;

    v0 = OverlayManager_GetData(manager);
    ov51_021E6C00();
    ov51_021E6038(v0);
    ov51_021E7CA4(&v0->sprites);
    Heap_Free(v0->frontCharFile);
    Heap_Free(v0->badgeFrameScreenFile);
    Heap_Free(v0->frontScreenFile);
    ov51_021E6EF0(v0);
    ov51_021E6644(v0->bgConfig);
    DisableTouchPad();
    TextFlags_SetCanTouchSpeedUpPrint(0);
    TextFlags_SetCanABSpeedUpPrint(0);
    MenuInputStateMgr_SetState(v0->args->settings.menuInputStatePtr, (enum MenuInputState)v0->inTouchMode);
#if defined(SDK_PORT)
    Main_SetVBlankIntrCB(NULL, NULL);
    OverlayManager_FreeData(manager);
#else
    OverlayManager_FreeData(manager);
    Main_SetVBlankIntrCB(NULL, NULL);
#endif
    Heap_Destroy(HEAP_ID_25);
    GF_SndHandleSetPlayerVolume(1U, 0x7FU);
    return 1;
}
