#include "global.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

void ov01_021F9A70(UnkStruct_ov5_021ED0A4 *gfx, const int *list) {
    const int *v0;
    int id;

    v0 = list;
    id = *v0;
    if (id != 0xFF) {
        do {
            ov01_021F9A18(gfx, id);
            v0 += 1;
            id = *v0;
        } while (id != 0xFF);
    }
}
