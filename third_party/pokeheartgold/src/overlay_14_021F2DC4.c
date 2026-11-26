#include "global.h"

#include "gf_gfx_loader.h"
#include "pokemon_icon_idx.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_box_funcs.h"

void *ov14_021F2DC4(UnkStruct_ov14_boxsys *arg0, BoxPokemon *arg1, NNSG2dCharacterData **arg2);

void *ov14_021F2DC4(UnkStruct_ov14_boxsys *arg0, BoxPokemon *boxMon, NNSG2dCharacterData **ppCharData) {
    return GfGfxLoader_GetCharDataFromOpenNarc(arg0->unk_454, (s32)Boxmon_GetIconNaix(boxMon), 0, ppCharData, HEAP_ID_10);
}
