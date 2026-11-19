#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "text.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "unk_0200A090.h"

int ov37_021E76A0(int param0);

void sub_02038C1C(int maxNumConnection);
int sub_02037454(void);
u16 sub_0203769C(void);
void ov37_021E76C0(UnkStruct_02095EAC *param0);
void ov37_021E76D0(UnkStruct_02095EAC *param0, int param1);

int ov37_021E76A0(int param0) {
    if (param0 == 0xff) {
        return 1;
    }

    if (TextPrinterCheckActive(param0) == 0) {
        return 1;
    }

    return 0;
}

void ov37_021E76C0(UnkStruct_02095EAC *param0) {
    ClearFrameAndWindow2(&param0->unk_33C, 0);
}

void ov37_021E76D0(UnkStruct_02095EAC *param0, int param1) {
    if (sub_0203769C() == 0) {
        int v0 = sub_02037454() + param1;

        if (v0 > 5) {
            v0 = 5;
        }

        sub_02038C1C(v0);
    }
}
