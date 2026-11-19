#include "global.h"

#include "error_handling.h"
#include "heap.h"
#include "overlay_96_hurdle_cpu.h"

void ov96_02208AF0(Ov96HurdleCpu *cpu, struct Ov96HurdleSlot *slots) {
    cpu->slots = slots;
}

void ov96_02208AF8(Ov96HurdleCpu *cpu, const void *data) {
    s32 i;
    u16 x;
    u16 y;
    const u8 *points;

    points = data;
    i = 0;
    do {
        x = points[0] * 8;
        y = points[1] * 8;
        if ((x != 0) && (y != 0)) {
            cpu->hurdles[i].active = TRUE;
            cpu->hurdles[i].x = x;
            cpu->hurdles[i].y = y;
        } else {
            cpu->hurdles[i].active = FALSE;
        }
        points += 2;
        i++;
    } while (i < 20);
}

void ov96_02208B2C(Ov96HurdleCpu *cpu) {
    Heap_Free(cpu);
}

u8 ov96_02208B34(Ov96HurdleCpu *cpu) {
    s32 v1;
    u8 v0;
    u8 v2;

    v2 = 4;
    if (cpu->unk_559 == 4) {
        return 0;
    }
    cpu->timer++;
    if (cpu->timer >= cpu->interval) {
        cpu->active = TRUE;
        cpu->timer = 0;
    }
    if (cpu->active) {
        v0 = cpu->unk_558;
        v1 = v0 + cpu->unk_555;
        if (v1 < (s32)v0) {
            GF_AssertFail();
        }
        ov96_02208BB8(cpu, v1);
        v2 = v1;
        cpu->unk_555++;
        if (cpu->unk_555 >= cpu->count) {
            cpu->unk_555 = 0;
            cpu->active = FALSE;
        }
    }
    return v2;
}
