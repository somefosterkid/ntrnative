#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "main.h"
#include "overlay_74_app_templates.h"
#include "overlay_manager.h"
#include "title_screen.h"

BOOL ov74_02229450(OverlayManager *man, int *state) {
    Heap_Destroy(HEAP_ID_89);
    RegisterMainOverlay(FS_OVERLAY_ID(intro_title), &gApplication_TitleScreen);
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_83);
    return 1;
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/overlay_74_thumb.s keeps the DS copy (three .word SYMBOL then the literal overlay id), which the 64-bit port emits as
// 8-byte pointers + a 4-byte word (wordmix.py): the PC build uses this C definition instead.
const OverlayManagerTemplate gApp_MainMenu_SelectOption_ConnectToRanger = {
    ov74_02229200,
    ov74_02229294,
    ov74_02229450,
    FS_OVERLAY_ID_NONE,
};
#endif
