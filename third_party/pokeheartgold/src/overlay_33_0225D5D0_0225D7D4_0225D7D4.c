#include "global.h"

#include "bg_window.h"

extern void *ov33_0225DA50;

void ov33_0225D7D4(void **param_1);

void ov33_0225D7D4(void **param_1) {
    u32 uVar1;
    u32 uVar2;
    Window *window;
    s32 *new_var;
    s32 iVar3;
    uVar2 = 0;
    uVar1 = *((u16 *)(((u8 *)param_1) + 0x1ba));
    new_var = &(*((s32 *)((&ov33_0225DA50) + (uVar1 - 1) * 3)));
    if (uVar2 < uVar1) {
        iVar3 = 0;
        window = (Window *)(param_1 + 6);
        do {
            AddWindow((BgConfig *)(*param_1), window, (WindowTemplate *)((*new_var) + iVar3));
            FillWindowPixelBuffer(window, 0);
            uVar2 = uVar2 + 1;
            iVar3 = iVar3 + 8;
            window = window + 1;
        } while (uVar2 < (*((u16 *)(((u8 *)param_1) + 0x1ba))));
    }
    return;
}

void ov33_0225D820(void *param_1);

void ov33_0225D820(void *param_1) {
    u32 uVar1;
    Window *window;
    uVar1 = 0;
    if (uVar1 < (*((u16 *)(param_1 + 0x1ba)))) {
        window = (Window *)(param_1 + 0x18);
        do {
            RemoveWindow(window);
            uVar1 = uVar1 + 1;
            window = window + 1;
        } while (uVar1 < (*((u16 *)(param_1 + 0x1ba))));
    }
    return;
}
