#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "heap.h"
#include "overlay_96_hurdle_cpu.h"
#include "overlay_96_ov96hurdle_hud.h"

extern WindowTemplate ov96_0221CBEC;

void ov96_02208A4C(Ov96HurdleHud *hud);

void ov96_02208A4C(Ov96HurdleHud *hud) {
    AddWindow(hud->bgConfig, &hud->window, &ov96_0221CBEC);
    BG_FillCharDataRange(hud->bgConfig, GF_BG_LYR_SUB_2_F, 0U, 1U, 0U);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_15_OFFSET, hud->heapId);
}

void ov96_021EB52C(s16 *param_1, s32 param_2, s32 param_3);

void ov96_02208A80(Ov96HurdleHudBadge *badge, s32 param_2);

void ov96_02208A80(Ov96HurdleHudBadge *badge, s32 param_2) {
    ov96_021EB52C((s16 *)badge->unk_0, 1, param_2);
    ov96_021EB52C((s16 *)badge->unk_8, 1, param_2);
    ov96_021EB52C((s16 *)badge->unk_4, 1, param_2);
    return;
}

Ov96HurdleCpu *ov96_02208AA8(enum HeapID heapId, s8 count) {
    Ov96HurdleCpu *cpu;

    cpu = Heap_Alloc(heapId, sizeof(Ov96HurdleCpu));
    MI_CpuFill8(cpu, 0, sizeof(Ov96HurdleCpu));
    cpu->count = count;
    cpu->unk_559 = 4 - count;
    cpu->unk_558 = cpu->unk_559;
    cpu->interval = 6;
    cpu->timer = cpu->interval;
    return cpu;
}

void ov96_02208AE8(Ov96HurdleCpu *cpu, s32 idx, struct Ov96HurdleRunner *runner) {
    cpu->runners[idx].runner = runner;
}
