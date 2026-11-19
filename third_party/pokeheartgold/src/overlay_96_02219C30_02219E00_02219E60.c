#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_digit_work.h"

struct UnkStruct_ov96_021E9A78;

Sprite *ov96_021EA374(struct UnkStruct_ov96_021E9A78 *, SpriteList *, s32, enum HeapID);

ManagedSprite *ov96_02219E00(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);

void ov96_02219E60(Ov96DigitWork *work);

void ov96_02219E60(Ov96DigitWork *work) {
    ManagedSprite *ms;
    s32 i;

    ms = ov96_02219E00(work->spriteSystem, work->spriteManager, 0x72, 0xB4, 0x14, 1);
    work->sprites[1] = ms;
    ManagedSprite_SetPaletteOverride(ms, 0xE);
    for (i = 0; i < 2; i++) {
        VecFx32 pos = { 0, 0, 0 };
        work->unk_A8[i] = ov96_021EA374(work->resSrc, SpriteManager_GetSpriteList(work->spriteManager), 1, work->heapId);
        pos.x = (134 + i * 16) << 12;
        pos.y = 180 << 12;
        pos.z = 0;
        Sprite_SetMatrix(work->unk_A8[i], &pos);
        Sprite_SetDrawFlag(work->unk_A8[i], 1);
    }
}
