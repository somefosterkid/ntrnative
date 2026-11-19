#include "global.h"

#include "heap.h"
#include "pokemon_icon_idx.h"
#include "pokemon_storage_system.h"
#include "pokemon_types_def.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

BoxPokemon *ov14_021E60C0(void *, u32, u32);
void ov14_021F2DE8(void *arg0, u32 arg1);
struct UnkStruct_ov14_boxsys;


typedef struct UnkStruct_ov14_021F2DE8_var_r2 {
    u8 unk_0[0x4058];
    s8 unk_4058;
} UnkStruct_ov14_021F2DE8_var_r2;
void *ov14_021F2DC4(struct UnkStruct_ov14_boxsys *, BoxPokemon *, void **);

void ov14_021F2DE8(void *arg0, u32 arg1) {
    void *spC;
    void *sp8;
    u32 sp4;
    BoxPokemon *temp_r0;
    s32 var_r6;
    s8 var_r1;
    u32 var_r4;
    void *temp_r0_2;
    UnkStruct_ov14_021F2DE8_var_r2 *var_r2;

    var_r4 = 0;
    sp4 = arg1;
    var_r6 = 0;
    do {
        if (PCStorage_GetMonDataByIndexPair(((PCStorage *)((UnkStruct_ov14_boxapp *)arg0)->storage), sp4, var_r4, 0xAC, NULL) == 0) {
            var_r2 = (void *)(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys) + var_r4);
            var_r1 = 0;
        } else {
            temp_r0 = ov14_021E60C0(arg0, sp4, var_r4);
            temp_r0_2 = ov14_021F2DC4(*((void **)&((UnkStruct_ov14_boxapp *)arg0)->sys), temp_r0, &spC);
            sp8 = temp_r0_2;
            MIi_CpuCopy32(*(void **)((u8 *)spC + 0x14), &((UnkStruct_ov14_boxapp *)arg0)->sys->unk_458[0][var_r6], 0x200U);
            Heap_Free(temp_r0_2);
            ((UnkStruct_ov14_boxapp *)arg0)->sys->unk_4076[var_r4] = Boxmon_GetIconPalette(temp_r0);
            var_r1 = 1;
            var_r2 = (void *)(((u8 *)((UnkStruct_ov14_boxapp *)arg0)->sys) + var_r4);
        }
        var_r4 += 1;
        var_r2->unk_4058 = var_r1;
        var_r6 += 0x200;
    } while (var_r4 < 0x1EU);
}
