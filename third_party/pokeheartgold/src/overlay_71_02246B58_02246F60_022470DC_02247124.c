#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "pokemon.h"
#include "pokemon_types_def.h"
#include "unk_02013FDC.h"
#include "wild_encounter.h"
#include "overlay_70_gtsapplication_state.h"
#include "overlay_71_trade_sequence_data.h"

extern UnkStruct_02014E30 ov71_0224BBDC;

void ov71_02247124(TradeSequenceData *param_1, s32 param_2, u32 param_3, s32 param_4, s32 param_5);

void ov71_02247124(TradeSequenceData *param_1, s32 param_2, u32 param_3, s32 param_4, s32 param_5) {
    void *dest;
    BOOL decrypt_result;
    u32 personality;
    u32 uVar1;
    enum GFPalLoadLocation location;
    BoxPokemon *boxMon;
    u32 species;
    u32 size;
    PokepicTemplate PStack_28;
    UnkStruct_02014E30 UStack_38;

    if (param_5 != 0) {
        size = 0x1900;
    } else {
        size = 0xc80;
    }
    dest = Heap_AllocAtEnd(HEAP_ID_56, size);
    if (dest != NULL) {
        UStack_38 = ov71_0224BBDC;
        if (param_2 == 0) {
            boxMon = param_1->animationTemplate->sendingPokemon;
        } else {
            boxMon = param_1->animationTemplate->receivingPokemon;
        }
        decrypt_result = AcquireBoxMonLock(boxMon);
        GetBoxmonSpriteCharAndPlttNarcIds(&PStack_28, boxMon, 2, 0);
        personality = GetBoxMonData(boxMon, 0, (void *)0x0);
        uVar1 = GetBoxMonData(boxMon, 5, (void *)0x0);
        species = uVar1 & 0xffff;
        if (param_5 != 0) {
            UStack_38.w = UStack_38.w << 1;
        }
        uVar1 = GetBoxMonData(boxMon, 0x4c, (void *)0x0);
        if ((uVar1 == 1) && (species == 0x147)) {
            species = 0x1ee;
        }
        sub_02014510((enum NarcId)PStack_28.narcID, (s32)PStack_28.charDataID, HEAP_ID_56, &UStack_38, dest, personality, param_5, 2, species);
        DC_FlushRange(dest, size);
        BG_LoadCharTilesData(param_1->bgConfig, (u8)param_3, dest, size, 0);
        ReleaseBoxMonLock(boxMon, decrypt_result);
        Heap_Free(dest);
    }
    if (param_3 >= 4) {
        location = GF_PAL_LOCATION_SUB_BG;
    } else {
        location = GF_PAL_LOCATION_MAIN_BG;
    }
    GfGfxLoader_GXLoadPal((enum NarcId)PStack_28.narcID, PStack_28.palDataID, location, (enum GFPalSlotOffset)(param_4 << 5), 0x20, HEAP_ID_56);
    return;
}
