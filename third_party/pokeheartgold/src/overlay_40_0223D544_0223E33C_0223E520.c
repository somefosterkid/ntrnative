#include "global.h"

#include "palette.h"
#include "touchscreen.h"
#include "overlay_40_unk_struct_0208_c06_c.h"
#include "overlay_40_unk_struct_ov62_02241204.h"

void ov40_0222BF80(void *, s32);

BOOL ov40_0222DA00(int *, int *, int, int);

BOOL ov40_0222DA84(int *, int);

void ov40_0222DF60(UnkStruct_0208C06C *, int);

void ov40_0222E8C4(void *, UnkStruct_0208C06C *, const UnkStruct_ov62_02248CDC *);

int ov40_0222F38C(void *, UnkStruct_0208C06C *);

void ov40_0222F6D0(void *, s32);

void ov40_0222F720(void *);

void ov40_0222F734(void *);

void ov40_0222F740(void *, UnkStruct_0208C06C *, s32);

void ov40_0222F920(void *, UnkStruct_0208C06C *);

void ov40_0222F9D4(void *, UnkStruct_0208C06C *);

void ov40_0222FA18(void *);

void ov40_0222FA24(void *);

void ov40_0222FA5C(void *, void *);

void ov40_0222FA88(void *);

void ov40_022307DC(UnkStruct_0208C06C *, int, int);

void ov40_02230944(UnkStruct_0208C06C *);

void ov40_02230964(UnkStruct_0208C06C *, BOOL);

void ov40_0223DCF0(UnkStruct_0208C06C *, int);

void ov40_022420B4(UnkStruct_0208C06C *, int);

s32 sub_02087E1C(UnkStruct_0208C06C *);

extern const TouchscreenHitbox ov40_02245650[1];

extern const UnkStruct_ov62_02248CDC ov40_02245758;

extern const UnkStruct_ov62_02248CDC ov40_02245784;

BOOL ov40_0223E520(UnkStruct_0208C06C *param0);

BOOL ov40_0223E520(UnkStruct_0208C06C *param0) {
    UnkStruct_ov62_02241204 *v0 = param0->unk_860;

    switch (param0->unk_08) {
    case 0:
        ov40_0222DF60(param0, 0x71);
        v0->unk_4C2 = 0xff;
        ov40_0223DCF0(param0, v0->unk_4C2);
        ov40_022420B4(param0, 1);
        ov40_022307DC(param0, 0x3d, 7);
        param0->unk_08++;
        break;
    case 1:
        ov40_0222DA84(&v0->unk_08, 0);
        if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 0, 2)) {
            ov40_02230964(param0, 1);
            ov40_0222F9D4(&param0->unk_14.unk_46C, param0);
            ov40_0222F734(&param0->unk_14.unk_48C);
            if (sub_02087E1C(param0) == 1) {
                ov40_0222E8C4(&param0->unk_14.unk_48C, param0, &ov40_02245784);
            } else {
                ov40_0222E8C4(&param0->unk_14.unk_48C, param0, &ov40_02245758);
            }
            ov40_0222FA5C(&param0->unk_14.unk_46C, &param0->unk_14.unk_48C);
            ov40_0222F740(&param0->unk_14.unk_48C, param0, 2);
            ov40_02230964(param0, 0);
            param0->unk_08++;
        }
        PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xc, v0->unk_08, param0->unk_14.unk_44);
        break;
    case 2:
        ov40_0222FA88(&param0->unk_14.unk_46C);
        ov40_0222F6D0(&param0->unk_14.unk_48C, param0->unk_14.unk_46C.unk_10);
        {
            int v1 = ov40_0222F38C(&param0->unk_14.unk_48C, param0);

            if (v1 != 0) {
                v0->unk_4C2 = v1;
                ov40_0223DCF0(param0, v1);
                param0->unk_08++;
            }
        }
        if (TouchscreenHitbox_TouchNewIsIn(&ov40_02245650[0])) {
            ov40_02230944(param0);
            param0->unk_08++;
        }
        break;
    case 3:
        ov40_0222FA24(&param0->unk_14.unk_46C);
        ov40_0222F720(&param0->unk_14.unk_48C);
        ov40_0222F920(&param0->unk_14.unk_48C, param0);
        ov40_0222FA18(&param0->unk_14.unk_46C);
        ov40_0222F734(&param0->unk_14.unk_48C);
        param0->unk_08++;
    case 4:
        ov40_0222DA84(&v0->unk_08, 1);
        if (ov40_0222DA00(&v0->unk_00, &v0->unk_04, 1, 2)) {
            ov40_022420B4(param0, 0);
            param0->unk_08++;
        }
        PaletteData_BlendPalettes(param0->unk_14.unk_14, PLTTBUF_SUB_OBJ, 0xc, v0->unk_08, param0->unk_14.unk_44);
        break;
    default:
        ov40_0222BF80(param0, 0xb);
    }
    return 0;
}
