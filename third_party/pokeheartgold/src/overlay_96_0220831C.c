#include "global.h"

#include "overlay_96_ov96hurdle_hud.h"
#include "sprite.h"

Sprite *ov96_021EB5B8(void *param_1);

void ov96_0220831C(Ov96HurdleHud *hud, s32 param_2, s32 param_3);

void ov96_0220831C(Ov96HurdleHud *hud, s32 param_2, s32 param_3) {
    Sprite *sprite;
    s32 iVar2;

    sprite = ov96_021EB5B8(hud->players[param_2].unk_0C);
    iVar2 = param_3 + param_2 * 3;
    DC_FlushRange(hud->iconChar[iVar2]->pRawData, 0x200);
    GXS_LoadOBJ(hud->iconChar[iVar2]->pRawData, param_2 * 0x200 + 0x1800, 0x200);
    Sprite_SetPaletteOverride(sprite, (u32)hud->iconPal[iVar2]);
    return;
}
