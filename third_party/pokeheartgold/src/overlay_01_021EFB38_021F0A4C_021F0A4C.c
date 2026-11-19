#include "global.h"

#include "bg_window.h"
#include "overlay_01_unk_struct_ov5_021de6bc.h"

struct UnkStruct_ov5_021DE928;

struct Window;

typedef struct UnkStruct_ov5_021DE928 {
    Window *unk_00;
    UnkStruct_ov5_021DE6BC *unk_04[48];
    u8 unk_C4;
    u8 unk_C5;
    u8 unk_C6;
    u8 unk_C7;
    s8 unk_C8;
    u8 unk_C9;
    u8 unk_CA;
    u8 unk_CB[1];
} UnkStruct_ov5_021DE928;

BOOL ov01_021F08C0(UnkStruct_ov5_021DE6BC *param0);

extern const u8 ov01_02206980[8];

void ov01_021F0868(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, u8 arg9);

BOOL ov01_021F0A4C(UnkStruct_ov5_021DE928 *param0);

BOOL ov01_021F0A4C(UnkStruct_ov5_021DE928 *param0) {
    int v0, v1, v2;
    int v3;
    BOOL v4;

    if (param0->unk_CA == 0) {
        return 1;
    }

    if (param0->unk_C4 < 48) {
        param0->unk_C8--;

        if (param0->unk_C8 <= 0) {
            param0->unk_C8 = param0->unk_C7;

            v0 = ov01_02206980[param0->unk_C4 % 8];
            v0 = (32 / 2) + (v0 * 32);
            v1 = -16 - ((param0->unk_C4 / 8) * 32);
            v2 = (192 + -16) - ((param0->unk_C4 / 8) * 32);

            ov01_021F0868(param0->unk_04[param0->unk_C4], (s32)(v0), (s32)(v0), (s32)(v1), (s32)(v2), (s32)(param0->unk_C6), (s32)(param0->unk_00), (s32)(32), (s32)(32), param0->unk_C9);
            param0->unk_C4++;
        }
    }

    for (v3 = param0->unk_C5; v3 < param0->unk_C4; v3++) {
        v4 = ov01_021F08C0(param0->unk_04[v3]);

        if (v4 == 1) {
            param0->unk_C5++;
        }
    }

    if ((param0->unk_C5 >= 48) && (v4 == 1)) {
        param0->unk_CA = 0;
        return 1;
    }

    return 0;
}
