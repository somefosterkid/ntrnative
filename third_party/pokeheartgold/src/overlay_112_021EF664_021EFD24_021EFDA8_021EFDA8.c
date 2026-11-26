#include "global.h"

#include "bg_window.h"
#include "item.h"
#include "pm_string.h"
#include "screen_fade.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E7CA4(void *, s32, s32);

s32 ov112_021E9C10(Ov112PokewalkerApp *, s32, s32, s32);

u8 ov112_021E9FA4(void *, s32, String *, s32, s32);

u8 ov112_021E9FD8(Ov112PokewalkerApp *, s32, String *, s32);

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov112_021EFD24(u16 *, s16);
#else
s32 ov112_021EFD24(u16 *, u16);
#endif

s32 ov112_021EFDA8(Ov112PokewalkerApp *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u16 sp10[13];
    String *temp_r6;
    s32 var_r4;
    u16 *var_r4_3;
    s32 var_r5_3;
    u16 temp_r1;
    ov112_021E9C10(arg0, 5, arg2, arg3);
    ov112_021E9A78(arg0, 6);
    if (ov112_021EFD4C(arg0) == 0) {
        return 0x24;
    }
    FillBgTilemapRect(arg0->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    ov112_021E7CA4(arg0, 2, 0xF);
    ov112_021E9FD8(arg0, 1, arg0->unk_1E4A4, 0);
    MIi_CpuClearFast(0U, sp10, 0x1CU);
    var_r4 = 0;
    do {
        ov112_021EFD24(sp10, arg0->unk_9DAC[var_r4].unk_0);
        var_r4 += 1;
    } while (var_r4 < 3);
    var_r4 = 0;
    do {
        ov112_021EFD24(sp10, arg0->unk_9DB8[var_r4].unk_0);
        var_r4 += 1;
    } while (var_r4 < 0xA);
    if ((((u32)((arg0->unk_AABC) << 0x19)) >> 0x1F) != 0) {
        ov112_021EFD24(sp10, arg0->unk_B002);
        MIi_CpuClearFast(0U, &arg0->unk_AABC, 0x6C8U);
        MIi_CpuClearFast(0U, arg0->unk_9D70, 0xD4CU);
    }
    temp_r6 = String_New(0x13U, HEAP_ID_154);
    var_r5_3 = 0;
    var_r4_3 = sp10;
loop_9:
    temp_r1 = *var_r4_3;

    if (temp_r1 != 0) {
        GetItemNameIntoString(temp_r6, temp_r1, HEAP_ID_154);
        ov112_021E9FA4(arg0, var_r5_3 + 2, temp_r6, 0, 0x30400);
        var_r5_3 += 1;
        var_r4_3 += 1;
        if (var_r5_3 < 0xE) {
            goto loop_9;
        }
    }
    String_Delete(temp_r6);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_154);
    return 0x12;
}
