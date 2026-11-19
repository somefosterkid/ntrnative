#include "global.h"

#include "unk_02005D10.h"
#include "unk_02035900.h"
#include "unk_02037C94.h"
#include "unk_0208FB64.h"

void ov73_021E6B98(void *arg0);

void ov73_021E705C(void *, u32, u32);

void sub_0208FD3C(void *, u16);

void sub_0208FB98(s32 param0, s32 param1, void *param2, void *param3) {
    u16 uVar1;

    *(u32 *)((u8 *)param3 + 0x4a08) = 0;
    *(u8 *)((u8 *)param3 + 0x4a14) = 1;
    uVar1 = sub_0203769C();
    sub_0208FD3C(param3, uVar1);
    ov73_021E6B98(param3);
    ov73_021E705C(param3, 0x19, param0 & 0xff);
    sub_020398D4(1, 1);
    PlaySE(0x657);
    return;
}
