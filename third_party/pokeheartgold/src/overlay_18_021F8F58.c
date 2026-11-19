#include "global.h"
struct PokedexEntryPopup;

#include "sys_task.h"
#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_18_pokedex_entry_popup.h"

void ObjCharTransfer_Destroy(void);



void ov18_021F8F58(void) {
    ObjCharTransfer_Destroy();
}

void ov18_021F8F60(PokedexEntryPopup *arg0) {
    s32 type;

    type = GF_GFX_RES_TYPE_CHAR;
    do {
        arg0->resMan[type] = Create2DGfxResObjMan(8, (GfGfxResType)type, arg0->heapId);
        type += 1;
    } while (type < 4);
}

void ov18_021F8F84(PokedexEntryPopup *arg0) {
    s32 v0;

    v0 = 0;
    do {
        Destroy2DGfxResObjMan(arg0->resMan[v0]);
        v0 += 1;
    } while (v0 < 4);
}

void ov18_021F8FA0(PokedexEntryPopup *arg0) {
    arg0->spriteList = G2dRenderer_Init(0x20, &arg0->renderer, arg0->heapId);
    ClearMainOAM(arg0->heapId);
    ov18_021F8F28(arg0);
    ov18_021F8F60(arg0);
    ov18_021F8FF8(arg0);
    ov18_021F9068(arg0);
    ov18_021F9150(arg0);
    ov18_021F94BC(arg0);
    ov18_021F9370(arg0);
    ov18_021F9518(arg0);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
}
