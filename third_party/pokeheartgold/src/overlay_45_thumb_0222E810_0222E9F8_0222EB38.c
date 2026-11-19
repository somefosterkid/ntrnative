#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"

typedef enum {
    UnkEnum_ov66_022336B8_00,
    UnkEnum_ov66_022336B8_01,
    UnkEnum_ov66_022336B8_02,
    UnkEnum_ov66_022336B8_03,
    UnkEnum_ov66_022336B8_04,
} UnkEnum_ov66_022336B8;

s32 ov45_0222F7B0(s32 arg0);

BOOL ov45_0222F74C(PPW_LOBBY_RESULT result);

PPW_LOBBY_RESULT ov45_02231C8C(s32 arg0);

BOOL ov45_0222EB38(UnkEnum_ov66_022336B8 param0);

BOOL ov45_0222EB38(UnkEnum_ov66_022336B8 param0) {
    s32 iVar1;

    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    iVar1 = ov45_0222F74C(ov45_02231C8C(ov45_0222F7B0(param0)));
    if (iVar1 == 1) {
        _022577C0->unk_12D = (s8)param0;
        _022577C0->unk_12F = 0;
    }
    return iVar1;
}
