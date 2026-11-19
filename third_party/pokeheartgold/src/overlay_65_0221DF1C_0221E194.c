#include "global.h"

#include "bg_window.h"

void ov65_0221DE24(s32, u32, u32);

void ov65_0221FB90(s32, u32, u32, void *, void *);

u32 ov65_0221FC54(u32, s32, s32, s32, u32);

void ov65_0221E050();

void ov65_0221E8EC();

u32 ov65_0221E194(s32 param_1);

u32 ov65_0221E194(s32 param_1) {
    u32 uVar1;

    uVar1 = ov65_0221FC54(*(u32 *)(param_1 + 0x180), param_1 + 0x664, param_1 + 0x670, param_1 + 0x36c0, 1);
    switch (uVar1) {
    case 1:
    case 3:
        FillBgTilemapRect(*(BgConfig **)(param_1 + 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90(param_1 + 0x5b4, 0x1c, 1, *(void **)(param_1 + 400), *(void **)(param_1 + 0x184));
        ov65_0221DE24(param_1, 0x18, 1);
        *(void (**)())(param_1 + 0x2220) = ov65_0221E8EC;
        *(u32 *)(param_1 + 0x68) = 0;
        break;
    case 2:
    case 4:
        FillBgTilemapRect(*(BgConfig **)(param_1 + 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90(param_1 + 0x594, 0xf, 1, *(void **)(param_1 + 400), *(void **)(param_1 + 0x184));
        *(void (**)())(param_1 + 0x2220) = ov65_0221E050;
    }
    return 0;
}
