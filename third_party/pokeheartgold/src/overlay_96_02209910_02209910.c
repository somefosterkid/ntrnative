#include "global.h"

#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_0220_data.h"

Sprite *ov96_021EA374(struct UnkStruct_ov96_021E9A78 *, SpriteList *, s32, enum HeapID);

Sprite *ov96_021EA584(struct UnkStruct_ov96_021E9A78 *, SpriteList *, s32, enum HeapID);

ManagedSprite *ov96_0220D13C(SpriteSystem *, SpriteManager *, s16, u8, u16, u16);

void ov96_02209910(Ov96_0220Data *arg0);

void ov96_02209910(Ov96_0220Data *arg0) {
    s32 i;
    VecFx32 pos = { 0, 0, 0 };

    for (i = 0; i < 3; i++) {
        pos.x = i * 0x10000 + 0x8000;
        pos.y = 0xB0000;
        arg0->unk_2C[i] = ov96_021EA584(arg0->resSrc, SpriteManager_GetSpriteList(arg0->spriteManager), 0, arg0->heapId);
        Sprite_SetMatrix(arg0->unk_2C[i], &pos);
        Sprite_SetDrawPriority(arg0->unk_2C[i], 2);
        Sprite_SetAnimCtrlSeq(arg0->unk_2C[i], 1);
        Sprite_SetDrawFlag(arg0->unk_2C[i], 1);
    }
    arg0->unk_24[0] = ov96_0220D13C(arg0->spriteSystem, arg0->spriteManager, 0xC0, 0xB8, 0x19, 2);
    for (i = 0; i < 2; i++) {
        VecFx32 pos2 = { 0, 0, 0 };
        arg0->unk_38[i] = ov96_021EA374(arg0->resSrc, SpriteManager_GetSpriteList(arg0->spriteManager), 2, arg0->heapId);
        pos2.x = (i * 16 + 0xD4) << 12;
        pos2.y = 0xB8000;
        pos2.z = 0;
        Sprite_SetMatrix(arg0->unk_38[i], &pos2);
        Sprite_SetDrawFlag(arg0->unk_38[i], 1);
    }
    arg0->unk_24[1] = ov96_0220D13C(arg0->spriteSystem, arg0->spriteManager, 0x20, 0x10, 0x13, 2);
}
