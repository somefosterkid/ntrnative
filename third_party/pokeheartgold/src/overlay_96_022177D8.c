#include "global.h"

#include "error_handling.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_96_ov96_rd_work.h"

void ov96_022177D8(Ov96RdSprites *sprites);

void ov96_022177D8(Ov96RdSprites *sprites) {
    s32 i;
    s32 j;
    Ov96RdSlot *slots;

    if (sprites->spriteSystem == NULL) {
        GF_AssertFail();
    }
    if (sprites->spriteManager == NULL) {
        GF_AssertFail();
    }
    if (sprites->sprite == NULL) {
        GF_AssertFail();
    }
    Sprite_DeleteAndFreeResources(sprites->sprite);
    slots = sprites->slots;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            Sprite_DeleteAndFreeResources(slots[i].sprites[j]);
        }
    }
}

void ov96_02217820(Ov96RdSprites *sprites);

void ov96_02217820(Ov96RdSprites *sprites) {
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            ManagedSprite_SetAnimateFlag(sprites->slots[i].sprites[j], 0);
        }
    }
    ManagedSprite_SetAnimateFlag(sprites->sprite, 0);
}

s32 ov96_0221785C(s32 arg0);

s32 ov96_0221785C(s32 arg0) {
    switch (arg0) {
    case 0xB:
        return 0;
    }
    return 1;
}

s32 ov96_022186CC(void *);

void ov96_02217868(Ov96RdObj *obj, VecFx32 *dir);

void ov96_02217868(Ov96RdObj *obj, VecFx32 *dir) {
    s32 state;

    state = obj->state;
    if ((state != 6) && (state != 0xB)) {
        obj->f16 = ov96_022186CC(dir);
    }
}

s32 ov96_02217890(s32 arg0);

s32 ov96_02217890(s32 arg0) {
    if ((arg0 == 6) || (arg0 == 0xB)) {
        return 1;
    }
    return 0;
}
