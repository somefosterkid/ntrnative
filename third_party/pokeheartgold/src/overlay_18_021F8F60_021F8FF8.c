#include "global.h"

#include "palette.h"
#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "overlay_18_pokedex_entry_popup.h"



void ov18_021F8FF8(PokedexEntryPopup *param_1) {
    u32 uVar1;
    ov18_021F922C(param_1->objs[0].res, param_1->resMan, param_1->heapId, param_1->gfxNarc, 0x1d, 0x20, 0x1e, 0x1f, 2, 0xc618, 0xc618, 0xc618, 0xc618);
    uVar1 = SpriteTransfer_GetPlttOffset(param_1->objs[0].res[1], NNS_G2D_VRAM_TYPE_2DMAIN);
    PaletteData_LoadPaletteSlotFromHardware(param_1->paletteData, PLTTBUF_MAIN_OBJ, (u16)(uVar1 << 4), 0x40);
}
