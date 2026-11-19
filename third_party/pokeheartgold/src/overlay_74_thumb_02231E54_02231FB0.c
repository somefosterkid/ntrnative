#include "global.h"

#include "sprite.h"

extern void *ov74_0223D338;

void ov74_02231FB0(void);

void ov74_02231FB0(void) {
    s32 v0;
    void *v1;

    v1 = ov74_0223D338;
    v0 = 0;
    do {
        if (*(Sprite **)((u8 *)v1 + 8) != NULL) {
            DC_FlushRange(v1 + 0xC, 0x200U);
            GX_LoadOBJ(v1 + 0xC, *(u32 *)v1, 0x200U);
            Sprite_SetPaletteOverride(*(Sprite **)((u8 *)v1 + 8), *(s32 *)((u8 *)v1 + 4));
        }
        v0 += 1;
        v1 += 0x20C;
    } while (v0 < 0x1E);
}
