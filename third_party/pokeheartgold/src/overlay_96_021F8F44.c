#include "global.h"

#include "constants/heap.h"

#include "filesystem_files_def.h"

void ov96_021EB29C(void *param_1, s32 param_2, u32 param_3);

void ov96_021EB2BC(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5);

void ov96_021EB2F4(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4, s32 param_5, u8 param_6);

void ov96_021EB334(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021EB36C(enum HeapID *param_1, NarcId param_2, s32 param_3, s32 param_4);

void ov96_021F8F44(u8 *param_1, s32 param_2);

void ov96_021F8F44(u8 *param_1, s32 param_2) {
    ov96_021EB29C(*(void **)(param_1 + 4), param_2, 0x68);
    ov96_021EB2BC(*(enum HeapID **)(param_1 + 4), NARC_poketool_pokezukan, 3, 0x68, 2);
    ov96_021EB2F4(*(enum HeapID **)(param_1 + 4), NARC_poketool_pokezukan, 0, 0x68, 2, 5);
    ov96_021EB334(*(enum HeapID **)(param_1 + 4), NARC_poketool_pokezukan, 2, 0x68);
    ov96_021EB36C(*(enum HeapID **)(param_1 + 4), NARC_poketool_pokezukan, 1, 0x68);
    return;
}
