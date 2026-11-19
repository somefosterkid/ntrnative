#include "global.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_02259560.h"

void ov41_02246014(UnkStruct_ov22_02255040 *param0, int param1);

UnkStruct_ov22_02259560 *ov41_02248A94(UnkStruct_ov22_02257F50 *param0);

void ov41_02248724(UnkStruct_ov22_02257F50 *param0);

void ov41_02248724(UnkStruct_ov22_02257F50 *param0) {
    UnkStruct_ov22_02259560 *head;
    UnkStruct_ov22_02259560 *node;
    s32 new_var;
    s32 i;
    i = -1;
    head = ov41_02248A94(param0);
    for (node = head->unk_08; node != head; node = node->unk_08) {
        if ((u32)node->unk_04 <= (3 - 1)) {
            new_var = i;
            ov41_02246014(node->unk_00, new_var);
        }
        i = i - 1;
    }
}
