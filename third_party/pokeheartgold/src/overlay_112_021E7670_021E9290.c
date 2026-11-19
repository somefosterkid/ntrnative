#include "global.h"

#include "gf_gfx_loader.h"
#include "pokemon.h"
#include "unk_02013FDC.h"

s32 ov112_021E9290(BoxPokemon *arg0, void *arg1, PokepicTemplate *arg2, s32 arg3);

s32 ov112_021E9290(BoxPokemon *boxMon, void *pSrc, PokepicTemplate *pokepicTemplate, s32 arg3) {
    s32 v0;
    u32 pid;

    v0 = arg3;
    GetBoxmonSpriteCharAndPlttNarcIds(pokepicTemplate, boxMon, 2U, 0);
    pid = GetBoxMonData(boxMon, 0, NULL);
    sub_02014494((enum NarcId)pokepicTemplate->narcID, (s32)pokepicTemplate->charDataID, HEAP_ID_154, 0, 0, 0xA, 0xA, pSrc, pid, 0, 2, (s32)GetBoxMonData(boxMon, 5, NULL));
    DC_FlushRange(pSrc, 0xC80U);
    if (v0 == 0) {
        GX_LoadOBJ(pSrc, 0xB80U, 0xC80U);
        GfGfxLoader_GXLoadPal((enum NarcId)pokepicTemplate->narcID, (s32)pokepicTemplate->palDataID, GF_PAL_LOCATION_MAIN_OBJ, GF_PAL_SLOT_4_OFFSET, 0x20U, HEAP_ID_154);
    } else {
        GXS_LoadOBJ(pSrc, 0xC40U, 0xC80U);
        GfGfxLoader_GXLoadPal((enum NarcId)pokepicTemplate->narcID, (s32)pokepicTemplate->palDataID, GF_PAL_LOCATION_SUB_OBJ, GF_PAL_SLOT_1_OFFSET, 0x20U, HEAP_ID_154);
    }
    return 0;
}
