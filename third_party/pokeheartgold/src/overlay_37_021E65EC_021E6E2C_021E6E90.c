#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "text.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"

s32 ov37_021E68AC(void *);

s32 ov37_021E76A0(s32);

s32 ov37_021E7844(void *, s32);

s32 ov37_021E6E90(void *arg0, s32 arg1);

s32 ov37_021E6E90(void *arg0, s32 arg1) {
    if (ov37_021E76A0(*(s32 *)((u8 *)arg0 + 0x30)) != 0) {
        ov37_021E7844(arg0, 0xC);
    }
    ov37_021E68AC(arg0);
    return arg1;
}

int sub_02037454(void);

u16 sub_0203769C(void);

void ov37_021E762C(UnkStruct_02095EAC *param0, int param1, int param2);

void ov37_021E76D0(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6EB4(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6EB4(UnkStruct_02095EAC *param0, int param1) {
    if ((param0->unk_30 != 0xff) && (ov37_021E76A0(param0->unk_30) == 0)) {
        RemoveTextPrinter(param0->unk_30);
    }

    ov37_021E762C(param0, 2, 1);
    ov37_021E7844(param0, 22);
    PlaySE(1508);

    if (sub_0203769C() == 0) {
        ov37_021E76D0(param0, 0);
    }

    param0->unk_9414 = sub_02037454();
    ov37_021E68AC(param0);

    return param1;
}
