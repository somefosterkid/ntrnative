#include "global.h"

#include "math_util.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"

void ov96_021EB564(void *param_1, s32 param_2);

void ov96_021F7620(UnkStruct_ov96_021F74A4 *param_1);

// Activates up to 25 random inactive entries.
void ov96_021F7620(UnkStruct_ov96_021F74A4 *param_1) {
    UnkStruct_ov96_021F74A4_Entry *list[29];
    UnkStruct_ov96_021F74A4_Entry *entry;
    s32 count;
    s32 i;
    u8 idx;

    entry = param_1->entries;
    count = 0;
    for (i = 0; i < 29; i++) {
        if (entry->active == 0) {
            list[count++] = entry;
        }
        entry++;
    }
    for (i = 0; i < 25; i++) {
        if (count <= 0) {
            return;
        }
        idx = LCRandom() % count;
        list[idx]->active = 1;
        ov96_021EB564(list[idx]->unk_10, 7);
        list[idx] = list[count - 1];
        count--;
    }
}
