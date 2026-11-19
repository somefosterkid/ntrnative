#include "global.h"

#include "sprite_transfer.h"
#include "unk_0200A090.h"
#include "filesystem.h"
#include "palette.h"
#include "overlay_18_pokedex_entry_popup.h"

void ov18_021F92DC(void *, void *);


enum NarcId ov18_021E5900(void);
s32 ov18_021E5908(void);

void ov18_021F9108(PokedexEntryPopup *param_1) {
    ov18_021F92DC(param_1->objs[1].res, param_1->resMan);
    SpriteTransfer_DeleteCharTransferTask(param_1->objs[2].res[0]);
    DestroySingle2DGfxResObj(param_1->resMan[0], param_1->objs[2].res[0]);
    DestroySingle2DGfxResObj(param_1->resMan[2], param_1->objs[2].res[2]);
    DestroySingle2DGfxResObj(param_1->resMan[3], param_1->objs[2].res[3]);
    return;
}

void ov18_021F9150(PokedexEntryPopup *arg0) {
    NARC *narc;

    narc = NARC_New(ov18_021E5900(), arg0->heapId);
    ov18_021F922C(arg0->objs[3].res, arg0->resMan, arg0->heapId, arg0->gfxNarc, 0x4D, -1, 0x4E, 0x4F, 1, 0xC61B, 0xC61B, 0xC61B, 0xC61B);
    arg0->objs[3].res[1] = ov18_021F92AC(arg0->resMan[1], arg0->heapId, narc, ov18_021E5908(), 1, 0xC61B);
    PaletteData_LoadPaletteSlotFromHardware(arg0->paletteData, PLTTBUF_MAIN_OBJ, (u16)(SpriteTransfer_GetPlttOffset(arg0->objs[3].res[1], NNS_G2D_VRAM_TYPE_2DMAIN) * 0x10), 0x20U);
    NARC_Delete(narc);
}
