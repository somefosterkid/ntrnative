#include "global.h"
struct UnkStruct_0208C06C;

#include "gf_gfx_loader.h"
#include "palette.h"
#include "pokemon_storage_system.h"
#include "pokepic.h"
#include "save_arrays.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

void ov40_02237548(struct UnkStruct_0208C06C *arg0, s32 arg1);

void ov40_02237548(UnkStruct_0208C06C *arg0, s32 value) {
    Pokepic *pokepic;

    pokepic = *(Pokepic **)((u8 *)(arg0->unk_860) + 0x32C);
    if (pokepic != NULL) {
        Pokepic_SetAttr(pokepic, 6, value);
    }
}

void ov40_02237564(struct UnkStruct_0208C06C *arg0);

void ov40_02237564(UnkStruct_0208C06C *arg0) {
    Pokepic *pokepic;

    pokepic = *(Pokepic **)((u8 *)(arg0->unk_860) + 0x32C);
    if (pokepic != NULL) {
        Pokepic_Delete(pokepic);
    }
}

s32 ov40_02237030(void *, s32);

s32 ov40_02237144(void *);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov40_022371E4(void *, s32);
#else
s32 ov40_022371E4(void *, s32);
#endif

s32 ov40_02237284(void *);

s32 ov40_02237474(void *);

void ov40_0223757C(void *arg0);

void ov40_0223757C(void *arg0) {
    u8 v0;
    void *v1;

    v1 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    SaveArray_PCStorage_Get(((SaveData *)((UnkStruct_0208C06C *)arg0)->saveData));
    v0 = *(u8 *)((u8 *)v1 + 0x17A);
    if (((u32)v0 >= 0x10U) && ((u32)v0 < 0x18U)) {
        *(u8 *)((u8 *)v1 + 0x17A) = 0U;
    }
    if ((u32) * (u8 *)((u8 *)v1 + 0x17A) >= 0x28U) {
        *(u8 *)((u8 *)v1 + 0x17A) = 0U;
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(((UnkStruct_0208C06C *)arg0)->unk_14.unk_00, (*(u8 *)((u8 *)v1 + 0x17A) * 3) + 0x8A, ((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(((UnkStruct_0208C06C *)arg0)->unk_14.unk_00, (*(u8 *)((u8 *)v1 + 0x17A) * 3) + 0x8C, ((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_109);
    PaletteData_LoadFromNarc(((PaletteData *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_14), NARC_a_1_9_1, (*(u8 *)((u8 *)v1 + 0x17A) * 3) + 0x8B, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x40U, 0xC0U, 0xC0U);
    ov40_02237474(arg0);
    ov40_02237284(arg0);
    ov40_022371E4(arg0, *(s32 *)((u8 *)v1 + 0x1B0));
    ov40_02237144(arg0);
    ov40_02237030(arg0, 0x10E);
}

void ov40_02237644(void *arg0);

void ov40_02237644(void *arg0) {
    PCStorage *storage;
    u8 v0;
    u8 v1;
    void *v2;

    v2 = ((UnkStruct_0208C06C *)arg0)->unk_860;
    storage = SaveArray_PCStorage_Get(((SaveData *)((UnkStruct_0208C06C *)arg0)->saveData));
    v1 = *(u8 *)((u8 *)v2 + 0x17A);
    if (((u32)v1 >= 0x10U) && ((u32)v1 < 0x18U)) {
        *(u8 *)((u8 *)v2 + 0x17A) = 0U;
    }
    v0 = *(u8 *)((u8 *)v2 + 0x17A);
    if (((u32)v0 >= 0x18U) && ((u32)v0 < 0x20U) && (PCStorage_IsBonusWallpaperUnlocked(storage, v0 - 0x18) == 0)) {
        *(u8 *)((u8 *)v2 + 0x17A) = 0U;
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(((UnkStruct_0208C06C *)arg0)->unk_14.unk_00, (*(u8 *)((u8 *)v2 + 0x17A) * 3) + 0x8A, ((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_109);
    GfGfxLoader_LoadScrnDataFromOpenNarc(((UnkStruct_0208C06C *)arg0)->unk_14.unk_00, (*(u8 *)((u8 *)v2 + 0x17A) * 3) + 0x8C, ((UnkStruct_0208C06C *)arg0)->unk_14.unk_10, GF_BG_LYR_MAIN_3, 0U, 0U, 0, HEAP_ID_109);
    PaletteData_LoadFromNarc(((PaletteData *)((UnkStruct_0208C06C *)arg0)->unk_14.unk_14), NARC_a_1_9_1, (*(u8 *)((u8 *)v2 + 0x17A) * 3) + 0x8B, HEAP_ID_109, PLTTBUF_MAIN_BG, 0x40U, 0xC0U, 0xC0U);
    ov40_02237474(arg0);
    ov40_02237284(arg0);
}
