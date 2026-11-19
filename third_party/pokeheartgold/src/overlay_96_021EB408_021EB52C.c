#include "global.h"

#include "error_handling.h"
#include "sprite.h"

void ov96_021EB52C(void *arg0, s32 arg1, s32 arg2);

void ov96_021EB52C(void *arg0, s32 arg1, s32 arg2) {
    Sprite *sprite;
    if ((*((u16 *)arg0)) != 0) {
        if (arg1 != 0) {
            *((s16 *)(((u8 *)arg0) + 2)) = 1;
            sprite = *((Sprite **)(((u8 *)arg0) + 4));
            if (arg2 != 0) {
                Sprite_SetDrawFlag(*((Sprite **)(((u8 *)arg0) + 4)), 1);
                return;
            }
            Sprite_SetDrawFlag(*((Sprite **)(((u8 *)arg0) + 4)), 0);
            return;
        }
        *((s16 *)(((u8 *)arg0) + 2)) = 0;
        Sprite_SetDrawFlag(*((Sprite **)(((u8 *)arg0) + 4)), 0);
        return;
    }
    GF_AssertFail();
}
