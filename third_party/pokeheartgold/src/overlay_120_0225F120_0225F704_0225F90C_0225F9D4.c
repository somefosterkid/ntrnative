#include "global.h"

#include "error_handling.h"
#include "filesystem.h"
#include "overlay_01_021EFB38.h"
#include "overlay_120.h"
#include "sprite.h"

extern const s32 ov120_022601F4[][4];

#if defined(SDK_PORT)
// On PC the overlay's asm rodata (asm/overlay_120.s) is not linked; this is a copy of it (2 rows used).
const s32 ov120_022601F4[][4] = {
    { 0x0A, 0x0B, 0x0C, 0x01 },
    { 0x07, 0x08, 0x09, 0x00 },
};
#endif

void ov120_0225F9D4(NARC *narc, Sprite **spriteOut, UnkStruct_ov5_021DE47C *env, UnkStruct_ov5_021DE5A4 *res, s32 which) {
    Sprite *sprite;

    if (which == 2) {
        GF_AssertFail();
    }
    ov01_021F0614(narc, env, res, ov120_022601F4[which][3], 1, ov120_022601F4[which][0], ov120_022601F4[which][2], ov120_022601F4[which][1], 600000);
    sprite = ov01_021F0718(env, res, 0x80000, 0x60000, 0, 0);
    *spriteOut = sprite;
    Sprite_SetDrawFlag(sprite, 1);
    Sprite_SetDrawPriority(*spriteOut, 0x10);
    Sprite_SetPriority(*spriteOut, 1);
}
