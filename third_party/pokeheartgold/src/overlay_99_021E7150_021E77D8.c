#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "poke_overlay.h"
#include "screen_fade.h"
#include "sprite_system.h"
#include "system.h"
#include "bg_window.h"
#include "overlay_99_pokeathlon_medals.h"
#include "overlay_98_ov98text_sys.h"

s32 ov99_021E7758(void *, u16);

void ov99_021E77D8(void *arg0);

void ov99_021E77D8(void *arg0) {
    s32 v0;

    ManagedSprite_SetDrawFlag(((PokeathlonMedals *)arg0)->sprites[3], 0);
    ov99_021E7758(arg0, 0);
    v0 = 0;
    do {
        ManagedSprite_SetDrawFlag(((PokeathlonMedals *)arg0)->sprites[0xF + v0], 0);
        v0 += 1;
    } while (v0 < 5);
    GfGfx_EngineBTogglePlanes(4U, 0U);
}

s32 ov98_0221F090(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov99_021E72C0(void *);
#else
s32 ov99_021E72C0(void *);
#endif

s32 ov99_021E7450(void *);

s32 ov99_021E7508(void *);

s32 ov99_021E7794(void *);

void ov99_021E7A54(void *);

s32 ov99_021E7A78(void *);

s32 ov99_021E7D30(void *);

s32 ov99_021E7DA4(void *);

s32 ov99_021E7EF0(void *);

s32 ov99_021E8224(void *);

FS_EXTERN_OVERLAY(OVY_98);

extern WindowTemplate ov99_021E9FDC[];

s32 PokeathlonMedals_Init(OverlayManager *man, s32 *state);

s32 PokeathlonMedals_Init(OverlayManager *man, s32 *state) {
    PokeathlonMedals *v0;
    void *v1;

    HandleLoadOverlay(FS_OVERLAY_ID(OVY_98), OVY_LOAD_ASYNC);
    v1 = OverlayManager_GetArgs(man);
    GfGfx_DisableEngineBPlanes();
    GfGfx_DisableEngineAPlanes();
    if (v1 == NULL) {
        GF_AssertFail();
    }
    Heap_Create(HEAP_ID_3, HEAP_ID_132, 0x30000U);
    v0 = OverlayManager_CreateAndGetData(man, sizeof(PokeathlonMedals), HEAP_ID_132);
    MI_CpuFill8(v0, 0U, sizeof(PokeathlonMedals));
    v0->heapId = HEAP_ID_132;
    v0->args = v1;
    ov98_0221F090();
    v0->narc = NARC_New(NARC_a_1_7_7, v0->heapId);
    v0->zukanNarc = NARC_New(NARC_application_zukanlist_zkn_data_zukan_data, v0->heapId);
    ov99_021E72C0(v0);
    ov99_021E7D30(v0);
    ov99_021E7DA4(v0);
    v0->textContext = ov98_0221EABC(v0->heapId, v0->bgConfig, 0xC, ov99_021E9FDC, 0x13A);
    ov99_021E7EF0(v0);
    ov99_021E7A78(v0);
    ov99_021E7508(v0);
    ov99_021E7450(v0);
    ov99_021E7794(v0);
    ov99_021E8224(v0);
    ResetVisibleHardwareWindows(PM_LCD_TOP);
    ResetVisibleHardwareWindows(PM_LCD_BOTTOM);
    Main_SetVBlankIntrCB(ov99_021E7A54, v0);
    return 1;
}

s32 ov98_0221E684(void *, void *, s32);

s32 ov98_0221F0EC(void);

s32 ov99_021E7AB8(void *);

s32 ov99_021E7EBC(void *);

s32 PokeathlonMedals_Exit(OverlayManager *man, s32 *state);

s32 PokeathlonMedals_Exit(OverlayManager *man, s32 *state) {
    void *v0;

    v0 = OverlayManager_GetData(man);
    Heap_Free(((void *)((PokeathlonMedals *)v0)->speciesOrder));
    ov99_021E7AB8(v0);
    ov98_0221E684(*((void **)&((PokeathlonMedals *)v0)->spriteContext), ((PokeathlonMedals *)v0)->sprites, 0x32);
    ov98_0221EB84(((PokeathlonMedals *)v0)->textContext, 0xC);
    ov99_021E7EBC(v0);
    ov98_0221F0EC();
    OverlayManager_FreeData(man);
    Heap_Destroy(HEAP_ID_132);
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_98));
    return 1;
}
