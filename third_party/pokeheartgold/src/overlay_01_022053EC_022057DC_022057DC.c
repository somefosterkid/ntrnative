#include "global.h"

#include "map_object.h"
#include "overlay_01_unk_struct_ov5_021_ed0_a4_t.h"

void *ov01_021FC5A4(struct ResourceHeap *, u32);

void ov01_022057DC(MapObjectManager *mapObjectManager);

void ov01_022057DC(MapObjectManager *mapObjectManager) {
    s32 *temp_r0;
    s32 new_var2;
    s32 temp_r2;
    u8 *new_var;
    s32 var_r1;
    u8 *new_var3;
    temp_r0 = ov01_021FC5A4(((UnkStruct_ov5_021ED0A4_t *)sub_0205F1A0(mapObjectManager))->unk_FC, 0x16);
    temp_r2 = *temp_r0;
    new_var2 = temp_r2;
    new_var = (((u8 *)temp_r0) + 4) + (temp_r2 * 2);
    new_var3 = new_var + temp_r2;
    var_r1 = 0;
    if (new_var2 > 0) {
        do {
            *((u8 *)(new_var3 + ((unsigned long long)var_r1))) = 1;
            var_r1 += 1;
        } while (var_r1 < temp_r2);
    }
}
