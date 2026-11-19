#include "global.h"
struct UnkStruct_ov96_021EF610;

#include "pokeathlon/pokeathlon.h"
#include "pokeathlon/pokeathlon_save.h"

#include "gf_gfx_loader.h"
#include "save.h"
#include "bg_window.h"
#include "overlay_96_unk_struct_ov96_021_ef610.h"
#include "overlay_98_ov98text_sys.h"

s32 ov96_021EF3A8(void *);

s32 ov96_021EF610(void *);

extern WindowTemplate ov96_0221BB38[];

void ov96_021EF54C(struct UnkStruct_ov96_021EF610 *arg0);

void ov96_021EF54C(UnkStruct_ov96_021EF610 *arg0) {
    GfGfxLoader_GXLoadPalFromOpenNarc(*((NARC * *)&arg0->unk_8), 4, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, *((enum HeapID *)&arg0->unk_C));
    GfGfxLoader_LoadCharDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 5, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
    GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 6, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_MAIN_FIRST, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
    if (*((s32 *)&arg0->unk_24) == 0) {
        GfGfxLoader_LoadCharDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 5, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_SUB_0_F, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
        GfGfxLoader_LoadScrnDataFromOpenNarc(*((NARC * *)&arg0->unk_8), 8, *((BgConfig * *)&arg0->unk_4), GF_BG_LYR_SUB_0_F, 0U, 0U, 1, *((enum HeapID *)&arg0->unk_C));
    }
    ov96_021EF3A8(arg0);
    arg0->textSys = ov98_0221EABC(*((enum HeapID *)&arg0->unk_C), *((BgConfig * *)&arg0->unk_4), 0xC, ov96_0221BB38, 0x135);
    ov96_021EF610(arg0);
}

u16 ov96_021EF5F4(u32 *param_1, s32 param_2);

u16 ov96_021EF5F4(u32 *param_1, s32 param_2) {
    SaveData *saveData;
    PokeathlonSave *pokeathlonSave;
    Pokeathlon_RecordsSolo *pPVar1;

    saveData = PokeathlonCourse_GetSaveData((PokeathlonCourseData *)*param_1);
    pokeathlonSave = Save_Pokeathlon_Get(saveData);
    pPVar1 = PokeathlonSave_GetRecordsSolo2(pokeathlonSave);
    return pPVar1[param_2].entries[0].unk_0;
}
