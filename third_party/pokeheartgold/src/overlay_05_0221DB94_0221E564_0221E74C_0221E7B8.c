#include "global.h"

#include "bg_window.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "unk_0208805C.h"

void ov05_0221E7B8(UnkStruct_ov10_0221FB28 *arg0, u32 arg1);

void ov05_0221E7B8(UnkStruct_ov10_0221FB28 *arg0, u32 arg1) {
    u8 uVar1;
    u32 bVar2;
    Window *window;

    window = &arg0->unk_10[arg1 * 4 + 3];
    uVar1 = CalculateHpBarColor(arg0->unk_214[arg1].unk_06, arg0->unk_214[arg1].unk_08, 0x30);
    switch (uVar1) {
    case 3:
    case 4:
        BG_LoadPlttData(1, &arg0->unk_4E4[9], 4, ((u16)window->paletteNum * 0x10 + 9) * 2);
        break;
    case 2:
        BG_LoadPlttData(1, &arg0->unk_4E4[25], 4, ((u16)window->paletteNum * 0x10 + 9) * 2);
        break;
    case 1:
        BG_LoadPlttData(1, &arg0->unk_4E4[41], 4, ((u16)window->paletteNum * 0x10 + 9) * 2);
        break;
    }
    bVar2 = CalculateHpBarPixelsLength((u32)arg0->unk_214[arg1].unk_06,
        (u32)arg0->unk_214[arg1].unk_08,
        0x30);
    FillWindowPixelRect(window, 0xa, 0, 2, (u16)bVar2, 1);
    FillWindowPixelRect(window, 9, 0, 3, (u16)bVar2, 2);
    FillWindowPixelRect(window, 0xa, 0, 5, (u16)bVar2, 1);
    return;
}
