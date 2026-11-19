#include "global.h"

#include "overlay_96_unk_struct_ov96_021_f74a4.h"

void ov96_021EB588(void *param_1, VecFx32 *param_2);

extern u8 ov96_0221C21C[];

void ov96_021F7544(UnkStruct_ov96_021F74A4 *param_1);

void ov96_021F7544(UnkStruct_ov96_021F74A4 *param_1) {
    u16 *puVar1;
    s32 iVar2;
    UnkStruct_ov96_021F74A4_Entry *entry;
    VecFx32 VStack_20;

    entry = param_1->entries;
    puVar1 = (u16 *)ov96_0221C21C;
    iVar2 = 0;
    do {
        VStack_20.x = (u32)*puVar1 << 0xf;
        VStack_20.y = (u32)puVar1[1] << 0xf;
        VStack_20.z = 0;
        ov96_021EB588(entry->unk_10, &VStack_20);
        ov96_021EB588(entry->unk_14, &VStack_20);
        ov96_021EB588(entry->unk_18, &VStack_20);
        entry->pos = VStack_20;
        iVar2 = iVar2 + 1;
        puVar1 = puVar1 + 2;
        entry = entry + 1;
    } while (iVar2 < 0x1d);
    return;
}
