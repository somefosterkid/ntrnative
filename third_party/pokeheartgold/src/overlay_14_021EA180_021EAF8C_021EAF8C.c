#include "global.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

s32 ov14_021EAF8C(UnkStruct_ov14_boxapp *app, int *state) {
    s32 next;

    next = ov14_021F7D9C[*state](app);
    *state = next;
    if (next != 0xb3) {
        return 1;
    }
    return 0;
}
