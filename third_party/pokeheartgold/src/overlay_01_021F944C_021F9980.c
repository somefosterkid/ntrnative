#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

struct UnkStruct_ov5_021ED0A4_t;


typedef enum {
    UnkEnum_ov5_021ED334_00 = 0,
    UnkEnum_ov5_021ED334_01,
    UnkEnum_ov5_021ED334_02,
} UnkEnum_ov5_021ED334;


extern u16 ov01_02207294[];

void ov01_021F9980(UnkStruct_ov5_021ED0A4 *param0, const int *param1) {
    while ((*param1) != 0xffff) {
        ov01_021F9968(param0, *param1);
        param1++;
    }
}

void ov01_021F99A4(UnkStruct_ov5_021ED0A4 *arg0, s32 arg1) {
    ov01_021FA1DC(arg0);
    ov01_021F98CC(arg0, arg0->unk_F8, arg1, 0xFF, ov01_02207294);
    ov01_021F9B10(arg0, arg1);
}

void ov01_021F99D0(UnkStruct_ov5_021ED0A4 *arg0, s32 arg1) {
    ov01_021FA1DC(arg0);
    ov01_021F98CC(arg0, arg0->unk_F8, arg1, 0xFF, ov01_02207294);
    ov01_021F9B54(arg0, arg1);
}
