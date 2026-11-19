#include "global.h"

#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "hall_of_fame.h"
#include "heap.h"
#include "overlay_manager.h"
#include "system.h"
#include "unk_020210A0.h"
#include "overlay_64_hall_of_fame_showcase.h"

void ov64_021E5A88(void *);

s32 ov64_021E5B00(void);

s32 ov64_021E5B10(HallOfFameShowcase *);

s32 ov64_021E5CD0(HallOfFameShowcase *);

s32 ov64_021E607C(void *);

s32 ov64_021E6170(HallOfFameShowcase *);

s32 ov64_021E622C(void *);

s32 ov64_021E652C(void *, s32);

s32 HallOfFameShowcase_Init(OverlayManager *man, s32 *state);

s32 HallOfFameShowcase_Init(OverlayManager *man, s32 *state) {
    HallOfFame *hallOfFame;
    HallOfFameShowcase *showcase;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_EngineASetPlanes(0U);
    GfGfx_EngineBSetPlanes(0U);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    EnableTouchPad();
    InitializeTouchPad(4);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    Heap_Create(HEAP_ID_3, HEAP_ID_59, 0x40000U);
    showcase = OverlayManager_CreateAndGetData(man, sizeof(HallOfFameShowcase), HEAP_ID_59);
    MI_CpuFill8(showcase, 0U, sizeof(HallOfFameShowcase));
    hallOfFame = OverlayManager_GetArgs(man);
    showcase->hallOfFame = hallOfFame;
    showcase->numRecords = Save_HOF_GetNumRecords(hallOfFame);
    showcase->pokegraNarc = NARC_New(NARC_a_1_8_0, HEAP_ID_59);
    ov64_021E5B00();
    ov64_021E5B10(showcase);
    ov64_021E5CD0(showcase);
    ov64_021E607C(showcase);
    ov64_021E6170(showcase);
    ov64_021E622C(showcase);
    ov64_021E652C(showcase, 0);
    Main_SetVBlankIntrCB(ov64_021E5A88, showcase);
    return 1;
}

s32 ov64_021E5CA4(HallOfFameShowcase *);

s32 ov64_021E602C(HallOfFameShowcase *);

s32 ov64_021E6118(HallOfFameShowcase *);

s32 ov64_021E620C(HallOfFameShowcase *);

s32 ov64_021E6260(HallOfFameShowcase *);

s32 HallOfFameShowcase_Exit(OverlayManager *man, s32 *state);

s32 HallOfFameShowcase_Exit(OverlayManager *man, s32 *state) {
    HallOfFameShowcase *arg;

    arg = OverlayManager_GetData(man);
    Main_SetVBlankIntrCB(NULL, arg);
    ov64_021E6260(arg);
    ov64_021E620C(arg);
    ov64_021E6118(arg);
    ov64_021E602C(arg);
    ov64_021E5CA4(arg);
    NARC_Delete(arg->pokegraNarc);
    DisableTouchPad();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_59);
    return 1;
}
