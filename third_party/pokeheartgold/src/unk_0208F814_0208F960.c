#include "global.h"
#include "sprite_resources_header.h"

#include "unk_02095EAC.h"
#include "bg_window.h"
#include "options.h"
#include "pm_string.h"
#include "unk_0200A090.h"
#include "unk_02032844.h"
#include "unk_0208F814.h"

typedef struct {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkStruct_02095FE4;

void ov37_021E6FC8(UnkStruct_02095EAC *param0, int param1, u8 param2);

int ov37_021E75E0(void);

#if defined(SDK_PORT) || defined(NONMATCHING)
void sub_02038C1C(s8 maxNumConnection);
#else
void sub_02038C1C(int maxNumConnection);
#endif

BOOL sub_02037108(int cmd, const void *data, int size);

int sub_02037454(void);

u16 sub_0203769C(void);

void sub_0208F960(int param0, int param1, void *param2, void *param3) {
    UnkStruct_02095EAC *v0 = (UnkStruct_02095EAC *)param3;
    UnkStruct_02095FE4 v1;
    UnkStruct_02095FE4 *v2 = param2;

    if (param0 != 0) {
        if (sub_0203769C() == 0) {
            v1 = *v2;
            v1.unk_00 = param0;
            v1.unk_01 = v0->unk_37C;

            switch (v2->unk_02) {
            case 0:
                if ((v0->unk_37C != sub_02037454()) || (v0->unk_37C != ov37_021E75E0()) || (v0->unk_37C != MATH_CountPopulation(sub_02033250()))) {
                    v1.unk_03 = 0;
                } else {
                    v0->unk_9418 |= 1 << param0;
                    v1.unk_03 = 1;
                    sub_02038C1C(sub_02037454());
                }
                break;
            case 1:
                break;
            }

            sub_02037108(126, &v1, sizeof(UnkStruct_02095FE4));
        }
    } else {
        switch (v2->unk_02) {
        case 0:
            if (v2->unk_00 == sub_0203769C()) {
                if (v2->unk_03 == 0) {
                    ov37_021E6FC8(v0, 9, v2->unk_00);
                } else {
                    v0->unk_941C = v2->unk_01;
                    ov37_021E6FC8(v0, 8, v2->unk_00);
                }
            }
            break;
        case 1:
            ov37_021E6FC8(v0, 21, v2->unk_00);
            break;
        }
    }
}

void sub_0208FA50(void) {
    return;
}
