#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"

int sub_02037454(void);

void ov37_021E68AC(UnkStruct_02095EAC *param0);

int ov37_021E76A0(int param0);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6F14(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6F14(UnkStruct_02095EAC *param0, int param1) {
    if ((param0->unk_9414 != 0) && (sub_02037454() != param0->unk_9414)) {
        param0->unk_9414 = 0;
    }

    if (ov37_021E76A0(param0->unk_30)) {
        ov37_021E7844(param0, 23);
        param0->unk_374 = 0;
    }

    ov37_021E68AC(param0);
    return param1;
}
