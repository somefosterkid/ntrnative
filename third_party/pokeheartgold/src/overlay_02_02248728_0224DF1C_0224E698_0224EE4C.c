#include "global.h"

#include "field_types_def.h"

u32 ov02_0224E828(FieldSystem *param_1, s16 param_2, s32 param_3, s32 param_4, s32 param_5, s16 *param_6, s16 *param_7, s32 arg7);

u32 ov02_0224EB48(FieldSystem *param_1, s32 param_2, s16 param_3, s32 param_4, s32 param_5, s16 *param_6, s16 *param_7, u8 param_8);

extern u8 ov02_02253A4C[];

BOOL ov02_0224EE4C(FieldSystem *fieldSystem, s16 x, s16 y, u8 dir, s32 param_5, s16 *outX, s16 *outY, u8 param_8);

BOOL ov02_0224EE4C(FieldSystem *fieldSystem, s16 x, s16 y, u8 dir, s32 param_5, s16 *outX, s16 *outY, u8 param_8) {
    s8 offsets[2];
    int i;

    offsets[0] = ov02_02253A4C[4];
    offsets[1] = ov02_02253A4C[5];
    *outX = x;
    *outY = y;
    switch (dir) {
    case 0:
    case 1:
        if (ov02_0224E828(fieldSystem, x, y, dir, param_5, outX, outY, param_8)) {
            for (i = 0; i < 2; i++) {
                if (ov02_0224E828(fieldSystem, x + offsets[i], y, dir, param_5, outX, outY, param_8)) {
                    *outX = x - i;
                    if (dir == 0) {
                        *outY = y - 1;
                    } else {
                        *outY = y + 2;
                    }
                    return TRUE;
                }
            }
        }
        break;
    case 2:
    case 3:
        if (ov02_0224EB48(fieldSystem, x, y, dir, param_5, outX, outY, param_8)) {
            for (i = 0; i < 2; i++) {
                if (ov02_0224EB48(fieldSystem, x, y - offsets[i], dir, param_5, outX, outY, param_8)) {
                    if (dir == 2) {
                        *outX = x - 2;
                    } else {
                        *outX = x + 1;
                    }
                    *outY = y + i;
                    return TRUE;
                }
            }
        }
        break;
    }
    return FALSE;
}
