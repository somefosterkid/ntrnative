#include "global.h"
#include "overlay_112_sprite_app.h"

void ov112_021F2204(Ov112SpriteApp *, s32, s32);

s32 ov112_021F1624(Ov112SpriteApp *, s32, s32);

s32 ov112_021F22B0(void *, s32, s32);

void ov112_021F298C(Ov112SpriteApp *arg0);

void ov112_021F298C(Ov112SpriteApp *arg0) {
    s32 total;
    s32 a;
    s32 tier;
    s32 kind;
    u32 t;
    Ov112Record *p;

    p = arg0->records[arg0->curIdx];
    total = 0;
    ov112_021F2204(arg0, 0, 1);
    ov112_021F22B0(arg0->sprites[5], (u8)arg0->playerGender, 3);
    a = p->unk_7C / 100;
    if (a > 0x28F) {
        a = 0x28F;
    }
    total += a;
    a = 0xA * arg0->unk_142;
    if (a > 0x28) {
        a = 0x28;
    }
    total += a;
    a = 0xA * arg0->unk_141;
    if (a > 0x28) {
        a = 0x28;
    }
    total += a;
    a = 0xF * arg0->unk_143;
    if (a > 0x96) {
        a = 0x96;
    }
    total += a;
    total += arg0->bits144.flag8 * 0xFA;
    total += arg0->bits144.flag9 * 0xFA;
    t = total;
    if (t >= 0x226) {
        tier = 0x48;
    } else if (t >= 0x12C) {
        tier = 0x49;
    } else if (t >= 0xC8) {
        tier = 0x4A;
    } else if (t >= 0x96) {
        tier = 0x4B;
    } else if (t >= 0x64) {
        tier = 0x4C;
    } else if (t >= 0x1E) {
        tier = 0x4D;
    } else {
        tier = 0x4E;
    }
    if ((s32)arg0->time.hour < 0xC) {
        kind = 0x47;
    } else {
        kind = 0x5A;
    }
    ov112_021F1624(arg0, kind, tier);
}
