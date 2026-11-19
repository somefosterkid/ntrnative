#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

extern u16 ov01_022072CC[];

void ov01_021F99FC(UnkStruct_ov5_021ED0A4 *gfx, const int *list) {
    const int *v0;
    int id;

    v0 = list;
    id = *v0;
    if (id != 0xFF) {
        do {
            ov01_021F99A4(gfx, id);
            v0 += 1;
            id = *v0;
        } while (id != 0xFF);
    }
}

void ov01_021F9A18(UnkStruct_ov5_021ED0A4 *arg0, s32 arg1) {
    ov01_021FA1E4(arg0);
    ov01_021F98CC(arg0, arg0->unk_FC, arg1, 0xFF, ov01_022072CC);
    ov01_021F9C34(arg0, arg1);
}

void ov01_021F9A44(UnkStruct_ov5_021ED0A4 *arg0, s32 arg1) {
    ov01_021FA1E4(arg0);
    ov01_021F98CC(arg0, arg0->unk_FC, arg1, 0xFF, ov01_022072CC);
    ov01_021F9C78(arg0, arg1);
}
