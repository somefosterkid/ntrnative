#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "heap.h"
#include "overlay_manager.h"
#include "pokepic.h"
#include "sprite_system.h"
#include "system.h"
#include "unk_020210A0.h"
#include "vram_transfer_manager.h"
#include "pokemon_summary_app.h"
#include "unk_02088288.h"
#include "unk_0208B1AC.h"
#include "unk_0208C3E4.h"
#include "unk_0208DE40.h"

BOOL PokemonSummary_Exit(OverlayManager *man, int *state) {
    PokemonSummary *v0;

    v0 = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, NULL);
    sub_0208DEFC(v0);
    sub_0208B258(v0);
    sub_0208C560(v0);
    sub_0208877C(v0->bgConfig);
    DisableTouchPad();
    GF_DestroyVramTransferManager();
    sub_02088AF8(v0);
    sub_0208895C(v0);
    FontID_Release(FONT_UNK_4);
    NARC_Delete(v0->pokepicAnimNarc);
    FontID_SetAccessLazy(FONT_SYSTEM);
    reg_G2_BLDCNT = 0;
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_19);
    return 1;
}

void sub_020885DC(void *arg);

void sub_020885DC(void *arg) {
    PokemonSummary *summary = arg;

    DoScheduledBgGpuUpdates(summary->bgConfig);
    PokepicManager_HandleLoadImgAndOrPltt(summary->pokepicManager);
    GF_RunVramTransferTasks();
    SpriteSystem_TransferOam();
#if defined(SDK_PORT) || defined(NONMATCHING)
    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
#else
    *(s32 *)((u8 *)OS_IRQTable + 0x3FF8) = (s32)(1 | *(s32 *)((u8 *)OS_IRQTable + 0x3FF8));
#endif
}

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/unk_02088288_02088610_021039B8.s keeps the DS copy (three .word SYMBOL then the literal overlay id), which the 64-bit
// port emits as 8-byte pointers + a 4-byte word (wordmix.py): the PC build uses this C definition instead.
const OverlayManagerTemplate gOverlayTemplate_PokemonSummary = {
    PokemonSummary_Init,
    PokemonSummary_Main,
    PokemonSummary_Exit,
    FS_OVERLAY_ID_NONE,
};
#endif
