#include "global.h"
#include "overlay_45_unk_struct_ov66_022336_c4.h"

#include "error_handling.h"

BOOL ov45_0222F74C(PPW_LOBBY_RESULT result);

PPW_LOBBY_RESULT ov45_02231EC0(void);

BOOL ov45_0222EB94(void);

BOOL ov45_0222EB94(void) {
    s32 iVar1;

    if (_022577C0 == 0) {
        GF_AssertFail();
    }
    iVar1 = ov45_0222F74C(ov45_02231EC0());
    if (iVar1 == 1) {
        _022577C0->unk_12E = 3;
    }
    return iVar1;
}
