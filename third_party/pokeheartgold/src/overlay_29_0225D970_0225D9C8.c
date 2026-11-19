#include "global.h"

#include "sprite.h"
#include "overlay_29_number_entry_data.h"

s32 ov29_0225D880(SpriteResource **, GF_2DGfxResMan **, NARC *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 ov29_0225D910(SpriteResource **, GF_2DGfxResMan **, s32 *, s32);

s32 ov29_0225D970(NumberEntryData *, s32, s32 *, s32 *);

extern s32 ov29_0225E21C;

extern s32 ov29_0225E220;

extern s32 ov29_0225E224;

extern s32 ov29_0225E228;

extern s32 ov29_0225E22C;

extern s32 ov29_0225E230;

extern s32 ov29_0225E234;

extern s32 ov29_0225E238;

extern s32 ov29_0225E23C;

extern s32 ov29_0225E240;

extern s32 ov29_0225E244;

void ov29_0225D9C8(NumberEntryData *arg0, void *arg1);

void ov29_0225D9C8(NumberEntryData *arg0, void *arg1) {
    s32 sp20[9];
    u32 var_r4;

    ov29_0225D880(arg0->padResObjs, arg0->resMans, arg1, 5, 4, 6, 7, 2, 0x399, 0x399, 0x399, 0x399);
    ov29_0225D910(arg0->padResObjs, arg0->resMans, sp20, 1);
    ov29_0225D970(arg0, 0, sp20, &ov29_0225E21C);
    ov29_0225D970(arg0, 1, sp20, &ov29_0225E220);
    ov29_0225D970(arg0, 2, sp20, &ov29_0225E224);
    ov29_0225D970(arg0, 3, sp20, &ov29_0225E228);
    ov29_0225D970(arg0, 4, sp20, &ov29_0225E22C);
    ov29_0225D880(arg0->digitResObjs, arg0->resMans, arg1, 1, 0, 2, 3, 2, 0x39A, 0x39A, 0x39A, 0x39A);
    ov29_0225D910(arg0->digitResObjs, arg0->resMans, sp20, 1);
    ov29_0225D970(arg0, 5, sp20, &ov29_0225E230);
    ov29_0225D970(arg0, 6, sp20, &ov29_0225E234);
    ov29_0225D970(arg0, 7, sp20, &ov29_0225E238);
    ov29_0225D970(arg0, 8, sp20, &ov29_0225E23C);
    ov29_0225D970(arg0, 9, sp20, &ov29_0225E240);
    ov29_0225D970(arg0, 0xA, sp20, &ov29_0225E244);
    var_r4 = 0;
    do {
        if (var_r4 < (u32) arg0->numDigits) {
            Sprite_SetDrawFlag(arg0->sprites[5 + var_r4], 1);
        } else {
            Sprite_SetDrawFlag(arg0->sprites[5 + var_r4], 0);
        }
        var_r4 += 1;
    } while (var_r4 < 6U);
}
