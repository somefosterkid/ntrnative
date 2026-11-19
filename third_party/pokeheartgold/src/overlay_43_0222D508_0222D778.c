#include "global.h"

#include "bg_window.h"
#include "sprite.h"
#include "sprite_system.h"
#include "overlay_43_unk_struct_ov64_0222_e21_c.h"
#include "overlay_43_unk_struct_ov64_02230904.h"

void ov43_0222AA70(void *arg0);

s32 ov43_0222AD00(void *, s32);

s32 ov43_0222AD40(void *, s32, s32);

s32 ov43_0222DACC(void *);

s32 ov43_0222DB84(void *, void *);

s32 ov43_0222DC7C(void *, void *);

s32 ov43_0222DF90(void *, void *);

void ov43_0222D778(void *arg0, void *arg1);

void ov43_0222D778(void *arg0, void *arg1) {
    void *var_r7;
    s32 var_r6;
    void *var_r5;
    void *var_r4;
    void *temp_r2;

    var_r5 = arg1;
    var_r7 = arg0;
    ov43_0222AD00(var_r5, 0);
    Sprite_SetPositionXY(((Sprite *)((UnkStruct_ov64_0222E21C *)var_r5)->unk_278.unk_08[0]), 8, 0x62);
    ov43_0222AD40(var_r5, 0, 0);
    Sprite_SetPositionXY(((Sprite *)((UnkStruct_ov64_0222E21C *)var_r5)->unk_278.unk_08[1]), 0xF8, 0x62);
    ov43_0222AD40(var_r5, 1, 0);
    var_r4 = var_r7;
    var_r6 = 0;
    var_r4 += 0x14;
    do {
        ov43_0222DF90(var_r4, var_r5);
        var_r6 += 1;
        var_r4 += 0x4C;
    } while (var_r6 < 3);
    ov43_0222DC7C(var_r7, var_r5);
    ov43_0222DB84(var_r7, var_r5);
    ov43_0222DACC(var_r7);
    FillBgTilemapRect(*(BgConfig **)var_r5, 5U, 0U, 0U, 0U, 0x20U, 0x20U, 0U);
    FillBgTilemapRect(*(BgConfig **)var_r5, 4U, 0U, 0U, 0U, 0x20U, 0x20U, 0U);
    FillBgTilemapRect(*(BgConfig **)var_r5, 2U, 0U, 0U, 0U, 0x20U, 0x20U, 0U);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)var_r5, 5U);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)var_r5, 4U);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)var_r5, 2U);
    temp_r2 = ((UnkStruct_ov64_02230904 *)var_r7)->unk_DC.unk_00[5];
    LoadRectToBgTilemapRect(*(BgConfig **)var_r5, 0U, temp_r2 + 0xC, 0U, 0U, ((*(u16 *)temp_r2 >> 3) & 0xFF), ((*(u16 *)((u8 *)temp_r2 + 2) >> 3) & 0xFF));
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)var_r5, 0U);
    ov43_0222AA70(var_r5);
    SetBgPriority(2U, 2U);
}
