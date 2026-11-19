#include "global.h"

#include "constants/heap.h"

#include "overlay_112_pokewalker_app.h"

#include "message_format.h"
#include "pokewalker.h"
#include "screen_fade.h"
#include "string_util.h"

void ov112_021E9F5C(Ov112PokewalkerApp *, u32, u32, u32);

void ov112_021E9FA4(Ov112PokewalkerApp *, u32, void *, u32, u32);

void ov112_021EA670(Ov112PokewalkerApp *, u32);

void ov112_021EA6B8(Ov112PokewalkerApp *, u32, u16, u16);

void ov112_021EAB78(Ov112PokewalkerApp *, u32, u32, u32);

u32 ov112_021EF758(Ov112PokewalkerApp *app);

u32 ov112_021EF758(Ov112PokewalkerApp *app) {
    s32 uVar1;
    s32 num;
    u32 uStack_10;
    u32 uStack_14;
    num = 0;
    uVar1 = sub_02032718(app->pokewalker);
    BufferPokewalkerCourseName(app->msgFormat, 2, uVar1 & 0xff);
    {
        u32 v0 = app->unk_10F0;
        u32 v1 = app->unk_1D758;
        if (v0 > v1) {
            num = v0 - v1;
        }
    }
    sub_02032674(app->pokewalker, &uStack_10, &uStack_14);
    BufferIntegerAsString(app->msgFormat, 5, num, 7, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(app->msgFormat, 3, (u32)(app->unk_9D7A), 7, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(app->msgFormat, 6, uStack_10, 7, PRINTING_MODE_LEFT_ALIGN, 1);
    BufferIntegerAsString(app->msgFormat, 7, uStack_14, 7, PRINTING_MODE_LEFT_ALIGN, 1);
    ov112_021E9F5C(app, 2, 0x38, 0x30400);
    ov112_021E9F5C(app, 3, 0x39, 0x30400);
    ov112_021E9F5C(app, 4, 0x3a, 0x30400);
    ov112_021E9FA4(app, 5, app->unk_1E4B4, 0, 0x30400);
    ov112_021E9F5C(app, 6, 0x3c, 0x30400);
    ov112_021E9FA4(app, 7, app->unk_1E4BC, 0, 0x30400);
    ov112_021E9F5C(app, 8, 0x3e, 0x30400);
    ov112_021EA670(app, 9);
    ov112_021EA6B8(app, 9, 0xd2, 0x40);
    ov112_021EAB78(app, 0, app->unk_9D44, (u32)((app->unk_9D51) << 27) >> 27);
    BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0, 6, 1, HEAP_ID_154);
    return 2;
}
