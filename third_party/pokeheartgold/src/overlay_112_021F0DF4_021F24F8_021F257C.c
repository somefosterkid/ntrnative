#include "global.h"
#include "overlay_112_sprite_app.h"

s32 ov112_021F1624(Ov112SpriteApp *, s32, s32);

s32 ov112_021F2204(Ov112SpriteApp *, s32, s32);

void ov112_021F257C(Ov112SpriteApp *arg0);

void ov112_021F257C(Ov112SpriteApp *arg0) {
    s32 var_r0;
    s32 var_r2;
    s32 var_r6;
    u32 temp_r2;
    Ov112Record *var_r4;
    u8 temp_r1;
    Ov112SpriteApp *var_r5;

    var_r5 = arg0;
    var_r4 = var_r5->records[var_r5->curIdx];
    temp_r2 = var_r4->kind - 1;
    var_r6 = ((s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1) * 5;
    ov112_021F2204(arg0, 0, 1);
    ov112_021F2204(var_r5, 1, 0);
    var_r0 = var_r4->unk_78 - var_r4->unk_7A;
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    temp_r1 = var_r4->kind;
    if ((temp_r1 % 2) == 1) {
        if (var_r0 >= 0x64) {
            var_r2 = 1;
        } else {
            var_r2 = 2;
        }
    } else if (var_r0 >= 0x64) {
        var_r2 = 4;
    } else {
        var_r2 = 3;
    }
    ov112_021F1624(var_r5, var_r6, var_r6 + var_r2);
}
