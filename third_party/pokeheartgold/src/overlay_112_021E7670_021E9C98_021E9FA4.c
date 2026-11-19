#include "global.h"

#include "bg_window.h"
#include "font.h"
#include "pm_string.h"
#include "text.h"
#include "overlay_112_pokewalker_app.h"

u8 ov112_021E9FA4(Ov112PokewalkerApp *param_1, s32 param_2, String *param_3, enum Font param_4, u32 param_5);

u8 ov112_021E9FA4(Ov112PokewalkerApp *param_1, s32 param_2, String *param_3, enum Font param_4, u32 param_5) {
    u8 uVar1;

    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    uVar1 = AddTextPrinterParameterizedWithColor(&param_1->windows[param_2], param_4, param_3, 0, 0, 0, param_5, (PrinterCallback_t)0x0);
    return uVar1;
}

u8 ov112_021E9FD8(Ov112PokewalkerApp *param_1, s32 param_2, String *param_3, enum Font param_4);

u8 ov112_021E9FD8(Ov112PokewalkerApp *param_1, s32 param_2, String *param_3, enum Font param_4) {
    u8 uVar1;

    FillWindowPixelBuffer(&param_1->windows[param_2], 0);
    uVar1 = AddTextPrinterParameterizedWithColor(&param_1->windows[param_2], param_4, param_3, 0, 0, 0, 0x10200, (PrinterCallback_t)0x0);
    return uVar1;
}

u8 ov112_021EA010(Ov112PokewalkerApp *param_1, s32 param_2, String *param_3, enum Font param_4, u32 param_5);

u8 ov112_021EA010(Ov112PokewalkerApp *param_1, s32 param_2, String *param_3, enum Font param_4, u32 param_5) {
    u8 uVar1;

    FillWindowPixelBuffer(&param_1->windows2[param_2], 0);
    uVar1 = AddTextPrinterParameterizedWithColor(&param_1->windows2[param_2], param_4, param_3, 0, 0, 0, param_5, (PrinterCallback_t)0x0);
    return uVar1;
}
