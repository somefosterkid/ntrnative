#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "message_format.h"
#include "screen_fade.h"
#include "overlay_112_pokewalker_app.h"

s32 ov112_021E7CA4(void *, s32, s32);

s32 ov112_021E9C10(Ov112PokewalkerApp *, s32);

u8 ov112_021E9FD8(Ov112PokewalkerApp *, s32, String *, s32);

s32 ov112_021EFAD4(void *, void *);

s32 ov112_021EFB14(Ov112PokewalkerApp *arg0) {
    s32 var_r4;
    u8 *var_r7;
    Ov112PokewalkerApp *var_r5;
    void *var_r6;

    var_r5 = arg0;
    ov112_021E9C10(arg0, 4);
    ov112_021E9A78(var_r5, 5);
    FillBgTilemapRect(var_r5->bgConfig, 1U, 0U, 0U, 0U, 0x20U, 0x18U, 0x10U);
    if (ov112_021EFAD4(var_r5->unk_9D70, &var_r5->unk_AABC) == 0) {
        ov112_021EA688(var_r5, 9);
        return 0x11;
    }
    ov112_021E7CA4(var_r5, 2, 0xF);
    ov112_021E9FD8(var_r5, 1, var_r5->unk_1E4A0, 0);
    ov112_021E9FD8(var_r5, 1, var_r5->unk_1E4A0, 0);
    var_r4 = 0;
    do {
        ov112_021EA688(var_r5, var_r4 + 9);
        var_r4 += 1;
    } while (var_r4 < 4);
    ov112_021EA6B8(var_r5, 9, 0x28, 0x40);
    ov112_021EA6B8(var_r5, 0xA, 0x90, 0x40);
    ov112_021EA6B8(var_r5, 0xB, 0x28, 0x60);
    ov112_021EA6B8(var_r5, 0xC, 0x90, 0x60);
    var_r4 = 0;
    var_r6 = var_r5;
    var_r7 = &var_r5->unk_9D7C[0].unk_D;
    do {
        if (*(u16 *)((u8 *)var_r6 + offsetof(Ov112PokewalkerApp, unk_9D7C[0].unk_0)) != 0) {
            ov112_021EA670(var_r5, var_r4 + 9);
            BufferSpeciesName(var_r5->msgFormat, var_r4 + 8, *(u16 *)((u8 *)var_r6 + offsetof(Ov112PokewalkerApp, unk_9D7C[0].unk_0)));
            ov112_021E9F5C(var_r5, var_r4 + 3, var_r4 + 0x43, 0x30400);
            ov112_021EAB78(var_r5, var_r4, *(u16 *)((u8 *)var_r6 + offsetof(Ov112PokewalkerApp, unk_9D7C[0].unk_0)), (u32)(*var_r7 << 0x1B) >> 0x1B);
        }
        var_r4 += 1;
        var_r6 += sizeof(((Ov112PokewalkerApp *)0)->unk_9D7C[0]);
        var_r7 += 0x10;
    } while (var_r4 < 3);
    if (((u32)(var_r5->unk_AABC << 0x1A) >> 0x1F) != 0) {
#if defined(SDK_PORT) || defined(NONMATCHING)
        Ov112PokewalkerMonRec *rec = &var_r5->unk_AD00;
        if (rec->unk_0 != 0) {
            ov112_021EA670(var_r5, 0xC);
            BufferSpeciesName(var_r5->msgFormat, 0xBU, rec->unk_0);
            ov112_021E9F5C(var_r5, 6, 0x46, 0x30400);
            ov112_021EAB78(var_r5, 3, rec->unk_0, (u32)(rec->unk_D << 0x1B) >> 0x1B);
        }
#else
        var_r4 = (s32)&var_r5->unk_AD00;
        if (*(u16 *)var_r4 != 0) {
            ov112_021EA670(var_r5, 0xC);
            BufferSpeciesName(var_r5->msgFormat, 0xBU, *(u16 *)var_r4);
            ov112_021E9F5C(var_r5, 6, 0x46, 0x30400);
            ov112_021EAB78(var_r5, 3, *(u16 *)var_r4, (u32)(*(u8 *)((u8 *)var_r4 + 0xD) << 0x1B) >> 0x1B);
        }
#endif
    }
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 1, HEAP_ID_154);
    return 0xD;
}
