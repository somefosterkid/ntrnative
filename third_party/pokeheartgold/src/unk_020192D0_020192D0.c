#include "global.h"

#include "gf_gfx_planes.h"
#include "heap.h"
#include "overlay_manager.h"
#include "sound_02004A44.h"
#include "system.h"

s32 sub_020192D0(OverlayManager *man, s32 *state);

s32 sub_020192D0(OverlayManager *man, s32 *state) {
    void **dest;

    Main_SetVBlankIntrCB(NULL, NULL);
    HBlankInterruptDisable();
    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFFE0FF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFFE0FF & reg_GXS_DB_DISPCNT);
    reg_GX_DISPCNT = (s32)(reg_GX_DISPCNT & 0xFFFF1FFF);
    reg_GXS_DB_DISPCNT = (s32)(0xFFFF1FFF & reg_GXS_DB_DISPCNT);
    reg_G2_BLDCNT = 0;
    reg_G2S_DB_BLDCNT = 0;
    Heap_Create(HEAP_ID_3, HEAP_ID_123, 0x28000U);
    dest = OverlayManager_CreateAndGetData(man, 0x80U, HEAP_ID_123);
    MI_CpuFill8(dest, 0U, 0x80U);
    *dest = OverlayManager_GetArgs(man);
    Sound_SetSceneAndPlayBGM(0xBU, 0x47DU, 1);
    return 1;
}
