#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"

PlayerProfile *sub_02034818(int netId);

u16 sub_0203769C(void);

void ov37_021E76C0(UnkStruct_02095EAC *param0);

void ov37_021E6848(Sprite **param0, BOOL param1);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

void ov37_021E78A4(UnkStruct_02095EAC *param0);

void ov37_021E6FC8(UnkStruct_02095EAC *param0, int param1, u8 param2);

void ov37_021E70BC(Window *param0, void *param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9);
void ov37_021E713C(Window *param0, const u8 *param1, int param2, int param3, int *param4, int *param5, int param6, int param7);
void ov37_021E72B4(UnkStruct_ov58_021D2820 *param0, UnkStruct_ov58_021D2754 *param1);

void ov37_021E6FC8(UnkStruct_02095EAC *param0, int param1, u8 param2) {
    switch (param1) {
    case 1:
        if ((param0->unk_368 == 6) || (param0->unk_368 == 14)) {
            ov37_021E78A4(param0);
        }

        ov37_021E6848(param0->unk_2AC, 0);
        BufferPlayersName(param0->unk_0C, 0, sub_02034818(param2));

        param0->unk_384 = param2;
        param0->unk_9418 = 0;
        break;
    case 3:
        ov37_021E76C0(param0);
        break;
    case 21:
        if (param0->unk_9420 == 1) {
            return;
        }

        BufferPlayersName(param0->unk_0C, 0, sub_02034818(param2));

        if (param2 == sub_0203769C()) {
            return;
        }

        if (sub_0203769C() == 0) {
            param0->unk_9418 &= 0xffff ^ param2;
        }

        if ((param0->unk_368 == 6) || (param0->unk_368 == 14)) {
            ov37_021E78A4(param0);

            CopyWindowToVram(&param0->unk_32C);
        }

        ov37_021E6848(param0->unk_2AC, 0);
        break;
    case 15:
        break;
    case 8:
        break;
    case 9:
        break;
    default:
        GF_ASSERT("FALSE");
        return;
    }

    ov37_021E7844(param0, param1);
}

void ov37_021E70BC(Window *param0, void *param1, int param2, int param3, int param4, int param5, int param6, int param7, int param8, int param9) {
    if (param6 < 0) {
        int v0;

        v0 = param6 * -1;

        if (v0 > param8) {
            v0 = param8;
        }

        param6 = 0;
        param2 += v0;
        param4 -= v0;
        param8 -= v0;
    }

    if (param7 < 0) {
        int v1;

        v1 = param7 * -1;

        if (v1 > param9) {
            v1 = param9;
        }

        param7 = 0;
        param3 += v1;
        param5 -= v1;
        param9 -= v1;
    }

    BlitBitmapRectToWindow(param0, param1, param2, param3, param4, param5, param6, param7, param8, param9);
}

void ov37_021E713C(Window *param0, const u8 *param1, int param2, int param3, int *param4, int *param5, int param6, int param7) {
    int v0, v1, v2, v3;
    int v4 = *param4;
    int v5 = *param5;
    int v6 = param2;
    int v7 = param3;

    if ((param6 == 0) && (param7 == 0)) {
        *param4 = param2;
        *param5 = param3;
        return;
    }

    v0 = MATH_IAbs(v6 - v4);
    v1 = MATH_IAbs(v7 - v5);

    if (v0 > v1) {
        if (v4 > v6) {
            v3 = (v5 > v7) ? 1 : -1;
            v2 = v4;
            v4 = v6;
            v6 = v2;
            v5 = v7;
        } else {
            v3 = (v5 < v7) ? 1 : -1;
        }

        ov37_021E70BC(param0, (void *)param1, 0, 0, 6, 6, v4, v5, 6, 6);
        v2 = v0 >> 1;

        while (++v4 <= v6) {
            if ((v2 -= v1) < 0) {
                v2 += v0;
                v5 += v3;
            }

            ov37_021E70BC(param0, (void *)param1, 0, 0, 6, 6, v4, v5, 6, 6);
        }
    } else {
        if (v5 > v7) {
            v3 = (v4 > v6) ? 1 : -1;
            v2 = v5;
            v5 = v7;
            v7 = v2;
            v4 = v6;
        } else {
            v3 = (v4 < v6) ? 1 : -1;
        }

        ov37_021E70BC(param0, (void *)param1, 0, 0, 6, 6, v4, v5, 6, 6);
        v2 = v1 >> 1;

        while (++v5 <= v7) {
            if ((v2 -= v0) < 0) {
                v2 += v1;
                v4 += v3;
            }

            ov37_021E70BC(param0, (void *)param1, 0, 0, 6, 6, v4, v5, 6, 6);
        }
    }

    *param4 = param2;
    *param5 = param3;
}

void ov37_021E72B4(UnkStruct_ov58_021D2820 *param0, UnkStruct_ov58_021D2754 *param1) {
    int v0;

    for (v0 = 0; v0 < 5; v0++) {
        param1[v0].unk_02 = param0[v0].unk_08_3;

        if (param0[v0].unk_08_3 != 0) {
            param1[v0].unk_00 = param0[v0].unk_00[param0[v0].unk_08_3 - 1];
            param1[v0].unk_01 = param0[v0].unk_04[param0[v0].unk_08_3 - 1];
        }
    }
}
