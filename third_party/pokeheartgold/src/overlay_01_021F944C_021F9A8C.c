#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"



void ov01_021F9A8C(UnkStruct_ov5_021ED0A4 *gfx, const int *param1, const int *param2, const int *param3) {
    ov01_021F9980(gfx, param1);
    ov01_021F99FC(gfx, param2);
    ov01_021F9A70(gfx, param3);
}

void ov01_021F9AAC(int *param0, int param1, int param2) {
    do {
        *param0 = param1;
        param0++;
        param2--;
    } while (param2);
}

int ov01_021F9AB4(int *param0, int param1, int param2, int param3) {
    do {
        if ((*param0) == param2) {
            *param0 = param1;
            return 1;
        }

        param0++;
        param3--;
    } while (param3);

    return 0;
}

int ov01_021F9AD0(int *param0, int param1, int param2) {
    do {
        if ((*param0) == param1) {
            return 1;
        }

        param0++;
        param2--;
    } while (param2);

    return 0;
}

int ov01_021F9AE4(int *param0, int param1, int param2, int param3) {
    do {
        if ((*param0) == param1) {
            *param0 = param2;
            return 1;
        }

        param0++;
        param3--;
    } while (param3);

    return 0;
}
