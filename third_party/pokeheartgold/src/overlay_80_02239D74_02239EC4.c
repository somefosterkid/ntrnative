#include "global.h"

#include "heap.h"
#include "sys_task_api.h"
#include "constants/heap.h"
#include "bg_window.h"
#include "overlay_80_02239F48.h"
#include "palette.h"
#include "sys_task.h"

void ov80_02239EC4(SysTask **arg0);

void ov80_02239ED8(SysTask *task, void *param);
void ov80_02239FA8(SysTask *task, void *param);
UnkStruct_ov80_02239F48 *ov80_02239F48(PaletteData *param_1);

struct UnkStruct_ov17_0224DF54_t;
typedef struct {
    u16 unk_00;
    u8 unk_02;
    u8 unk_03;
    u8 unk_04;
    u8 unk_05;
    u8 unk_06;
    u8 unk_07;
    u8 unk_08_0 : 1;
    u8 unk_08_1 : 1;
    u8 unk_08_2 : 1;
    u8 : 5;
} UnkStruct_ov17_0224EDE0;
typedef struct UnkStruct_ov17_0224DF54_t UnkStruct_ov17_0224DF54;
typedef struct {
    SysTask *unk_00;
    UnkStruct_ov17_0224DF54 *unk_04;
    UnkStruct_ov17_0224EDE0 unk_08;
    u8 unk_11;
    u8 unk_12;
} UnkStruct_ov17_0224D69C;
void ov80_02239F94(UnkStruct_ov17_0224D69C *param0);

void ov80_02239EC4(SysTask **ptr) {
    SysTask_Destroy(*ptr);
    Heap_Free(ptr);
}

void ov80_02239ED8(SysTask *task, void *param) {
    u8 *param_2 = param;
    if (*(u8 *)(param_2 + 0x22008) < 0xc) {
        *(u8 *)(param_2 + 0x22008) = *(u8 *)(param_2 + 0x22008) + 1;
        return;
    }
    *(u8 *)(param_2 + 0x22008) = 0;
    BG_LoadCharTilesData(*(BgConfig **)(param_2 + 4), 3, (void *)(param_2 + 0x2008 + (u32) * (u8 *)(param_2 + 0x22009) * 0x10000), 0x10000, 0);
    BG_LoadScreenTilemapData(*(BgConfig **)(param_2 + 4), 3, (void *)(param_2 + 8 + (u32) * (u8 *)(param_2 + 0x22009) * 0x1000), 0x1000);
    ScheduleBgTilemapBufferTransfer(*(BgConfig **)(param_2 + 4), 3);
    *(u8 *)(param_2 + 0x22009) = *(u8 *)(param_2 + 0x22009) ^ 1;
    return;
}

UnkStruct_ov80_02239F48 *ov80_02239F48(PaletteData *param_1) {
    UnkStruct_ov80_02239F48 *dest;
    u16 *puVar1;
    SysTask *pSVar2;

    dest = Heap_Alloc(HEAP_ID_101, sizeof(UnkStruct_ov80_02239F48));
    MI_CpuFill8(dest, 0, sizeof(UnkStruct_ov80_02239F48));
    dest->unk_04 = param_1;
    dest->unk_68 = 2;
    puVar1 = PaletteData_GetUnfadedBuf(param_1, PLTTBUF_MAIN_BG);
    MIi_CpuCopy16(puVar1 + 0x50, dest->unk_08, 0x60);
    pSVar2 = SysTask_CreateOnMainQueue(ov80_02239FA8, dest, 0x1368d);
    dest->unk_00 = pSVar2;
    return dest;
}

void ov80_02239F94(UnkStruct_ov17_0224D69C *param0) {
    SysTask_Destroy(param0->unk_00);
    Heap_Free(param0);
}
