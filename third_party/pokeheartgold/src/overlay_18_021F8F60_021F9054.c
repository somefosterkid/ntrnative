#include "global.h"

#include "palette.h"
#include "sprite_transfer.h"
#include "overlay_18_pokedex_entry_popup.h"

s32 ov18_021F92DC(void *, void *);


void ov18_021F9054(PokedexEntryPopup *arg0) {
    ov18_021F92DC(arg0->objs[0].res, arg0->resMan);
}


s32 ov18_021F967C(s32);


void ov18_021F9068(PokedexEntryPopup *arg0) {
    ov18_021F922C(arg0->objs[1].res, arg0->resMan, arg0->heapId, arg0->gfxNarc, ov18_021F967C(arg0->type1), 0x23, 0x21, 0x22, 4, 0xC619, 0xC619, 0xC619, 0xC619);
    ov18_021F922C(arg0->objs[2].res, arg0->resMan, arg0->heapId, arg0->gfxNarc, ov18_021F967C(arg0->type2), -1, 0x21, 0x22, 4, 0xC61A, 0xC61A, 0xC61A, 0xC61A);
    PaletteData_LoadPaletteSlotFromHardware(arg0->paletteData, PLTTBUF_MAIN_OBJ, (u16)(SpriteTransfer_GetPlttOffset(arg0->objs[1].res[1], NNS_G2D_VRAM_TYPE_2DMAIN) * 0x10), 0x80U);
}
