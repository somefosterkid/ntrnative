#include "global.h"

#include "bg_window.h"

void ov65_0221DE24(s32, u32, u32);

void ov65_0221F0C4(s32);

void ov65_0221F0D8(s32, u32, u32);

void ov65_0221FB90(s32, u32, u32, void *, void *);

u32 ov65_0221FC54(u32, s32, s32, s32, u32);

void ov65_0221F354();

u32 ov65_0221F2A8(s32 param_1);

u32 ov65_0221F2A8(s32 param_1) {
    u32 uVar1;

    uVar1 = ov65_0221FC54(*(u32 *)(param_1 + 0x180), param_1 + 0x664, param_1 + 0x670, param_1 + 0x36c0, 0);
    switch (uVar1) {
    case 1:
    case 3:
        ov65_0221F0C4(param_1);
        ov65_0221F0D8(param_1, 0x17, 0x14);
        *(void (**)())(param_1 + 0x2220) = ov65_0221F354;
        break;
    case 2:
    case 4:
        FillBgTilemapRect(*(BgConfig **)(param_1 + 0x180), 0, 0, 0, 0, 0x20, 0x18, 0);
        ov65_0221FB90(param_1 + 0x5b4, 0x14, 1, *(void **)(param_1 + 400), *(void **)(param_1 + 0x184));
        *(void (**)())(param_1 + 0x2220) = ov65_0221F354;
        ov65_0221DE24(param_1, 0x18, 4);
    }
    return 0;
}
