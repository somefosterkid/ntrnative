#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "unk_02005D10.h"
#include "unk_0200A090.h"

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02095FE4;

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02038C1C(s8 maxNumConnection);
#else
void sub_02038C1C(int maxNumConnection);
#endif

BOOL sub_02037030(int cmd, const void *data, int size);

int sub_02037454(void);

u16 sub_0203769C(void);

void ov37_021E68AC(UnkStruct_02095EAC *param0);

void ov37_021E762C(UnkStruct_02095EAC *param0, int param1, int param2);

int ov37_021E75C4(void);

void ov37_021E6848(Sprite **param0, BOOL param1);

void ov37_021E7844(UnkStruct_02095EAC *param0, int param1);

void ov37_021E78A4(UnkStruct_02095EAC *param0);

int ov37_021E78C4(UnkStruct_02095EAC *param0);

int ov37_021E69DC(UnkStruct_02095EAC *param0, int param1);

int ov37_021E69DC(UnkStruct_02095EAC *param0, int param1) {
    int v0;

    param0->unk_43DC.unk_08_3 = 0;

    if ((param0->unk_43E6[0].unk_09 == 2) && (sub_0203769C() != 0)) {
        ov37_021E68AC(param0);

        if (ov37_021E78C4(param0)) {
            PlaySE(1522);
        }

        return param1;
    }

    if ((sub_0203769C() == 0) && (param0->unk_9418 != 0)) {
        ov37_021E68AC(param0);
        return param1;
    }

    if (ov37_021E75C4() != sub_02037454()) {
        ov37_021E68AC(param0);
        return param1;
    }

    v0 = YesNoPrompt_HandleInput(param0->unk_9454);

    switch (v0) {
    case 1:
        if (sub_0203769C() == 0) {
            ov37_021E7844(param0, 13);
            ov37_021E762C(param0, 4, 1);
        } else {
            UnkStruct_02095FE4 v1;

            MI_CpuClear8(&v1, sizeof(UnkStruct_02095FE4));
            v1.unk_02 = 0;
            v1.unk_00 = sub_0203769C();

            param0->unk_9420 = 1;
            param0->unk_941E = 0;

            ov37_021E7844(param0, 7);

            sub_02037030(126, &v1, sizeof(UnkStruct_02095FE4));
            ClearFrameAndWindow2(&param0->unk_33C, 1);
        }

        ov37_021E78A4(param0);
        CopyWindowToVram(&param0->unk_32C);
        break;
    case 2:
        ov37_021E7844(param0, 4);
        ov37_021E6848(param0->unk_2AC, 0);
        ClearFrameAndWindow2(&param0->unk_33C, 1);
        ov37_021E78A4(param0);
        CopyWindowToVram(&param0->unk_32C);

        if (sub_0203769C() == 0) {
            sub_02038C1C(sub_02037454() + 1);
            param0->unk_9458 = 1;
        }
        break;
    }

    ov37_021E68AC(param0);

    return param1;
}
