#include "global.h"

#include "gf_gfx_planes.h"
#include "sprite.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0203A3B0.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

void ov73_021E82A8(SpriteTemplate *, WifiBattleTowerAppState *, SpriteResourcesHeader *, NNS_G2D_VRAM_TYPE);

extern u32 ov73_021EA684;

void ov73_021E82D4(WifiBattleTowerAppState *param_1);

void ov73_021E82D4(WifiBattleTowerAppState *param_1) {
    Sprite *pSVar1;
    u16 *puVar2;
    s32 seq;
    SpriteTemplate SStack_44;

    CreateSpriteResourcesHeader(&param_1->arrowSpriteHeader, 0, 0, 0, 0, -1, -1, 0, 0, param_1->resourceCollection[0], param_1->resourceCollection[1], param_1->resourceCollection[2], param_1->resourceCollection[3], (GF_2DGfxResMan *)0x0, (GF_2DGfxResMan *)0x0);
    ov73_021E82A8(&SStack_44, param_1, &param_1->arrowSpriteHeader, NNS_G2D_VRAM_TYPE_2DMAIN);
    puVar2 = (u16 *)&ov73_021EA684;
    seq = 0;
    do {
        SStack_44.position.x = (u32)*puVar2 << 0xc;
        SStack_44.position.y = (u32)puVar2[1] << 0xc;
        pSVar1 = Sprite_CreateAffine(&SStack_44);
        param_1->selectionArrows[seq] = pSVar1;
        Sprite_SetAnimActiveFlag(param_1->selectionArrows[seq], 1);
        Sprite_SetAnimCtrlSeq(param_1->selectionArrows[seq], seq);
        Sprite_SetDrawFlag(param_1->selectionArrows[seq], 0);
        puVar2 = puVar2 + 2;
        seq = seq + 1;
    } while (seq < 2);
    GfGfx_EngineATogglePlanes(0x10, 1);
    GfGfx_EngineBTogglePlanes(0x10, 1);
    sub_0203A880();
    return;
}
