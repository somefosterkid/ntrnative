#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02095FE4;

BOOL sub_02037030(int cmd, const void *data, int size);

int sub_02037454(void);

u16 sub_0203769C(void);

void ov37_021E68AC(UnkStruct_02095EAC *param0);

int ov37_021E75C4(void);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6B64(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6B64(UnkStruct_02095EAC *param0, int param1) {
    param0->unk_43DC.unk_08_3 = 0;

    if ((param0->unk_941C != sub_02037454()) || (param0->unk_941C != ov37_021E75C4())) {
        param0->unk_941E = 0;

        ov37_021E7844(param0, 9);
        ov37_021E68AC(param0);

        return param1;
    }

    param0->unk_941E++;

    if (param0->unk_941E > 30) {
        UnkStruct_02095FE4 v0;

        MI_CpuClear8(&v0, sizeof(UnkStruct_02095FE4));

        v0.unk_02 = 1;
        v0.unk_00 = sub_0203769C();

        sub_02037030(126, &v0, sizeof(UnkStruct_02095FE4));
        param0->unk_941E = 0;
        ov37_021E7844(param0, 10);
    }

    ov37_021E68AC(param0);

    return param1;
}

void ov37_021E6848(Sprite **param0, BOOL param1);

int ov37_021E6BFC(UnkStruct_02095EAC *param0, int param1);

int ov37_021E6BFC(UnkStruct_02095EAC *param0, int param1) {
    param0->unk_43DC.unk_08_3 = 0;
    param0->unk_9420 = 0;

    ov37_021E7844(param0, 4);
    ov37_021E6848(param0->unk_2AC, 0);
    ov37_021E68AC(param0);

    return param1;
}
