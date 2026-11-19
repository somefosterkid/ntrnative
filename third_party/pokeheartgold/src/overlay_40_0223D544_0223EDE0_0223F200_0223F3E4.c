#include "global.h"

#include "palette.h"
#include "sprite_system.h"
#include "touchscreen.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

typedef struct UnkStruct_ov40_0223F3E4_temp_r4 {
    u8 unk_0[0x4];
    u8 unk_4;
    u8 unk_5[0x3];
    s32 unk_8;
    s32 unk_C;
    u8 unk_10;
    u8 unk_11[0x3];
    ManagedSprite *unk_14;
    u8 unk_18[0x14];
    u8 unk_2C;
    u8 unk_2D[0x3];
    ManagedSprite *unk_30;
    u8 unk_34[0x4D8];
    void *unk_50C;
    s32 unk_510;
} UnkStruct_ov40_0223F3E4_temp_r4;

s32 ov40_0222BF80(void *, s32);

s32 ov40_0222D66C(void *, void *, s32);

s32 ov40_0222DA00(void *, void *, s32, s32);

s32 ov40_0222DA84(void *, s32);

s32 ov40_0222F488(void *, void *);

s32 ov40_0222F5EC(void *, s32);

s32 ov40_0222F720(void *);

s32 ov40_0222F734(void *);

s32 ov40_0222F920(void *, void *);

s32 ov40_0222FA18(void *);

void *ov40_0222FA24(void *);

s32 ov40_0222FA88(void *);

s32 ov40_0222FE68(void *);

void *ov40_0222FE98(void *);

s32 ov40_02230944(void *);

s32 ov40_0223D874(void *);

s32 ov40_0223D8D4(void *);

s32 ov40_0223E024(void *);

s32 ov40_0223E064(void *);

s32 ov40_022420B4(void *, s32);

s32 sub_020878EC(void *, s16, s16);

s32 sub_020879E0(void *, s32);

s32 sub_02087A08(void *, s32, s32);

extern TouchscreenHitbox ov40_0224564C;

extern TouchscreenHitbox ov40_02245650;

s32 ov40_0223F3E4(UnkStruct_0208C06C *arg0);

s32 ov40_0223F3E4(UnkStruct_0208C06C *arg0) {
    s32 temp_r1;
    UnkStruct_ov40_0223F3E4_temp_r4 *temp_r4;

    temp_r4 = arg0->unk_860;
    temp_r1 = arg0->unk_08;
    switch (temp_r1) {
    case 0:
        ov40_0222FA88(&arg0->unk_14.unk_46C);
        ov40_0222F5EC(&arg0->unk_14.unk_48C, arg0->unk_14.unk_46C.unk_10);
        ov40_0222F488(&arg0->unk_14.unk_48C, arg0);
        sub_020878EC(arg0->unk_6F0, 0x10, ((0x18 * arg0->unk_14.unk_48C.unk_38.unk_04) + 0x4C));
        ov40_0223D8D4(arg0);
        if (TouchscreenHitbox_TouchNewIsIn(&ov40_0224564C) != 0) {
            ov40_02230944(arg0);
            temp_r4->unk_C = 0x11;
            arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        }
        if (TouchscreenHitbox_TouchNewIsIn(&ov40_02245650) == 0) {
            break;
        }
        ov40_02230944(arg0);
        temp_r4->unk_C = 0x10;
        arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        break;
    case 1:
        ov40_0222FA24(&arg0->unk_14.unk_46C);
        ov40_0222F720(&arg0->unk_14.unk_48C);
        ov40_0222F920(&arg0->unk_14.unk_48C, arg0);
        ov40_0222FE98(temp_r4->unk_50C);
        ov40_0223D874(arg0);
        ov40_0222FE68(arg0);
        ov40_0223E024(arg0);
        ov40_0223E064(arg0);
        sub_020879E0(arg0->unk_6F0, 0);
        sub_02087A08(arg0->unk_6F0, 0, 0);
        if (temp_r4->unk_C == 0x10) {
            ov40_0222FA18(&arg0->unk_14.unk_46C);
            ov40_0222F734(&arg0->unk_14.unk_48C);
            temp_r4->unk_510 = 0;
        }
        arg0->unk_08 = (s32)(arg0->unk_08 + 1);
    case 2:
        ov40_0222DA84((void *)&temp_r4->unk_8, 1);
        if (ov40_0222DA00(temp_r4, (void *)&temp_r4->unk_4, 1, 0) != 0) {
            ov40_022420B4(arg0, 0);
            if (temp_r4->unk_C == 0x11) {
                ov40_0222D66C((void *)&temp_r4->unk_10, &arg0->unk_14, 3);
                ov40_0222D66C((void *)&temp_r4->unk_2C, &arg0->unk_14, 0x5E);
                ManagedSprite_SetAnim(temp_r4->unk_14, 0);
                ManagedSprite_SetAnim(temp_r4->unk_30, 3);
            }
            arg0->unk_08 = (s32)(arg0->unk_08 + 1);
        }
        PaletteData_BlendPalettes(((PaletteData *)arg0->unk_14.unk_14), PLTTBUF_SUB_OBJ, 0xCU, temp_r4->unk_8, (s32) arg0->unk_14.unk_44);
        PaletteData_BlendPalettes(((PaletteData *)arg0->unk_14.unk_14), PLTTBUF_MAIN_OBJ, 0xCU, temp_r4->unk_8, (s32) arg0->unk_14.unk_44);
        break;
    default:
        ov40_0222BF80(arg0, temp_r4->unk_C);
        break;
    }
    return 0;
}
