#include "global.h"

#include "overlay_96_ov96_arena_work.h"

void ov96_021F0F04(Ov96ArenaWork *work, u8 *param_2);

void ov96_021F0F04(Ov96ArenaWork *work, u8 *param_2) {
    u32 sumA;
    u32 sumB;
    u32 sumC;
    u8 i;
    s32 x;
    s32 y;
    Ov96ArenaMon *e;
    u8 idx;
    u32 val;
    u8 b18;
    u8 b40;

    sumA = 0;
    sumB = 0;
    sumC = 0;
    i = 0;
    do {
        e = &work->mon[i / 3][i % 3];
        x = e->pos.x / 4096;
        y = e->pos.y / 4096;
        if (x < 0) {
            x = 0;
        } else if (x > 255) {
            x = 255;
        }
        if (y < 0) {
            y = 0;
        } else if (y > 255) {
            y = 255;
        }
        b18 = e->state;
        b40 = e->unk_40;
        param_2[i] = x;
        param_2[i + 12] = y;
        sumA += (b40 - 1) << (i * 2);
        sumB += b18 << (i * 2);
        sumC += e->unk_44 << i;
        i++;
    } while (i < 12);
    *(u32 *)(param_2 + 0x18) = sumA;
    *(u32 *)(param_2 + 0x18) += work->unk_726 << 24;
    *(u32 *)(param_2 + 0x1c) = 0;
    *(u32 *)(param_2 + 0x1c) += sumB;
    *(u32 *)(param_2 + 0x1c) += work->unk_732 << 24;
    *(u32 *)(param_2 + 0x1c) += work->unk_74B << 28;
    idx = work->unk_724;
    val = work->unk_6E0[idx];
    work->unk_724 = (idx + 1) % 4;
    *(u32 *)(param_2 + 0x20) = 0;
    *(u32 *)(param_2 + 0x20) += sumC;
    *(u32 *)(param_2 + 0x20) += work->unk_729 << 12;
    *(u32 *)(param_2 + 0x20) += work->unk_748 << 13;
    *(u32 *)(param_2 + 0x20) += work->unk_74A << 14;
    *(u32 *)(param_2 + 0x20) += work->unk_74C << 15;
    *(u32 *)(param_2 + 0x20) += (u8)val << 18;
    *(u32 *)(param_2 + 0x20) += idx << 26;
    *(u32 *)(param_2 + 0x20) += work->unk_727 << 28;
    work->unk_748 = 0;
    work->unk_74B = 12;
    work->unk_74C = 0;
}
