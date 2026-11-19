#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "filesystem_files_def.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_bg_switch.h"
#include "palette.h"
#include "sys_task.h"
#include "sys_task_api.h"

void ov07_0221E914(SysTask *task, void *data);

extern void ov07_02237870(void);

s32 ov07_0221E9D4(BattleBgSwitch *bgSwitch);

s32 ov07_0221E9D4(BattleBgSwitch *bgSwitch) {
    u8 *var_r0;
    u16 var_r1;
    u8 var_r3;
    u16 *var_r3_2;
    u16 var_r5_2;
    u16 var_r6;
    BattleAnimSystem *system;
    s32 var_r5;
    BattleBgAnim *temp_r0;
    BattleBgPaletteAnim *temp_r0_2;
    u32 temp_r0_3;
    u8 *temp_r0_4;

    system = bgSwitch->battleAnimSystem;
    temp_r0 = Heap_Alloc((enum HeapID)system->heapID, sizeof(BattleBgAnim));
    MI_CpuFill8(temp_r0, 0U, sizeof(BattleBgAnim));
    temp_r0_2 = Heap_Alloc((enum HeapID)system->heapID, sizeof(BattleBgPaletteAnim));
    temp_r0->paletteAnim = temp_r0_2;
    MI_CpuFill8(temp_r0_2, 0U, sizeof(BattleBgPaletteAnim));
    bgSwitch->bgExtraAnimActive = 1;
    temp_r0->cancel = 0;
    temp_r0->paletteAnim->heapID = (enum HeapID)system->heapID;
    temp_r0->paletteAnim->paletteData = system->paletteData;
    ov07_0221DD38(system->bgAnim, temp_r0, 3);
    temp_r0_3 = bgSwitch->bgID;
    switch (temp_r0_3) {
    case 0:
    case 1:
    case 3:
    case 5:
        var_r5 = 0;
        break;
    case 4:
        var_r5 = 2;
        break;
    case 19:
        var_r5 = 4;
        break;
    case 0x35:
        var_r5 = 6;
        break;
    case 0x34:
        var_r5 = 8;
        break;
    case 22:
        var_r5 = 0xA;
        break;
    case 0x3A:
        var_r5 = 0xC;
        break;
    case 28:
        var_r5 = 0xE;
        break;
    case 29:
        var_r5 = 0x10;
        break;
    case 0x39:
        var_r5 = 0x1A;
        break;
    case 7:
        var_r5 = 0x1C;
        break;
    case 14:
        var_r5 = 0x14;
        break;
    case 17:
        var_r5 = 0x12;
        break;
    case 2:
        var_r5 = 0x16;
        break;
    case 11:
        var_r5 = 0x18;
        break;
    case 0x2C:
        var_r5 = 0x1E;
        break;
    default:
#if defined(SDK_PORT)
        if (&ov07_02237870 != NULL) {
#else
        if ((u32)&ov07_02237870 != 0) {
#endif
            GF_AssertFail();
        }
        var_r5 = 0;
        break;
    }
    temp_r0_4 = GfGfxLoader_LoadFromNarc(NARC_a_0_0_9, var_r5, 0, (enum HeapID)system->heapID, 1);
    if (temp_r0_4 == NULL) {
        GF_AssertFail();
    }
    temp_r0->paletteAnim->narcMemberBase = (s8)(var_r5 + 1);
    var_r3 = *(u8 *)temp_r0_4;
    var_r1 = 0;
    var_r0 = temp_r0_4;
    if (var_r3 != 0xFF) {
        do {
            var_r0 += 1;
            temp_r0->paletteAnim->paletteIdx[var_r1] = var_r3;
            var_r1 += 1;
            var_r3 = *var_r0;
        } while (var_r3 != 0xFF);
    }
    temp_r0->paletteAnim->numPalettes = (s8)var_r1;
    var_r5_2 = *(u16 *)((u8 *)temp_r0_4 + 0x80);
    var_r6 = 0;
    var_r3_2 = (unsigned short *)(temp_r0_4 + 0x80);
    if (var_r5_2 != 0xFF98) {
        do {
            temp_r0->paletteAnim->frameCount[var_r6] = var_r5_2;
            var_r3_2 += 1;
            var_r5_2 = *var_r3_2;
            var_r6 += 1;
        } while (var_r5_2 != 0xFF98);
    }
    Heap_Free(temp_r0_4);
    SysTask_CreateOnMainQueue(ov07_0221E914, temp_r0, 0x1001U);
    return 0;
}
