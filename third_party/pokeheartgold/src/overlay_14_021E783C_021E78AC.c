#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "overlay_14_unk_struct_ov14_boxapp.h"
#include "overlay_14_box_funcs.h"

void ov14_021E76D0(UnkStruct_ov14_boxapp *param_1, s32 param_2, u32 param_3, u32 param_4);
void ov14_021E7740(void *param_1, u32 param_2, u32 param_3, s32 param_4, s32 param_5);

void ov14_021E78AC(UnkStruct_ov14_boxapp *param_1, s32 param_2);

void ov14_021E78AC(UnkStruct_ov14_boxapp *param_1, s32 param_2) {
    u8 *iVar1;
    int new_var;
    s16 sVar2;
    unsigned char new_var2;
    s16 uVar3;
    new_var = 0xb8;
    iVar1 = ((u8 *)(param_1)->sys);
    new_var2 = *((s8 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_448[1]);
    if (new_var2 == 0) {
        uVar3 = 0x25c;
        sVar2 = 0xe;
    } else {
        uVar3 = new_var;
        sVar2 = 0xf;
    }
    *((u8 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_448[1]) = (*((u8 *)&((UnkStruct_ov14_boxsys *)iVar1)->unk_448[1])) ^ 1;
    ov14_021E76D0(param_1, param_2, (s32)(*((s8 *)&(param_1)->sys->unk_448[0])), uVar3);
    PaletteData_LoadNarc((param_1)->sys->unk_18, NARC_a_0_1_9, param_2 + 0x28, HEAP_ID_10, PLTTBUF_MAIN_BG, 0x20, sVar2 << 4);
    ov14_021E7740(param_1, param_2, (s32)(*((s8 *)&(param_1)->sys->unk_448[0])), (s16)uVar3, sVar2);
    ScheduleBgTilemapBufferTransfer((param_1)->sys->unk_14, 3);
    return;
}
