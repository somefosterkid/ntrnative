#include "global.h"

#include "gf_gfx_loader.h"
#include "sprite.h"
#include "overlay_70_gtsapplication_state.h"

static s32 ov70_0224122C(s32 arg0);

static s32 ov70_0224122C(s32 arg0) {
    return (arg0 * 4) + 0xE;
}

void ov70_02241234(GTSApplicationState *arg0);

void ov70_02241234(GTSApplicationState *arg0) {
    s32 v1;
    u16 v0;

    v1 = 0;
    do {
        if (Sprite_GetDrawFlag(arg0->avatarSprites[v1 + 1]) != 0) {
            v0 = Sprite_GetAnimationNumber(arg0->avatarSprites[v1 + 1]);
            if (v0 != (ov70_0224122C(v1) + 1)) {
                Sprite_SetAnimCtrlSeq(arg0->avatarSprites[v1 + 1], ov70_0224122C(v1) + 1);
            }
        }
        v1 += 1;
    } while (v1 < 7);
}

void ov70_0224127C(GTSApplicationState *arg0);

void ov70_0224127C(GTSApplicationState *arg0) {
    arg0->avatarPaletteHeapPtr = GfGfxLoader_GetPlttData(NARC_application_record_record, 0xB, &arg0->avatarPaletteData, HEAP_ID_61);
    arg0->avatarCharDataHeapPtr = GfGfxLoader_GetCharData(NARC_application_record_record, 0xC, 1, &arg0->avatarCharData, HEAP_ID_61);
    DC_FlushRange(((void *)arg0->avatarCharData), 0x8000U);
}
