#include "global.h"

#include "overlay_96_ov96_02203468work.h"

typedef struct Ov96Packet {
    u16 sums[4];   // 0x00
    u16 cells[12]; // 0x08
    u32 bits;      // 0x20
} Ov96Packet;

void ov96_02202738(Ov96_02203468Work *work, Ov96Packet *pkt);

void ov96_02202738(Ov96_02203468Work *work, Ov96Packet *pkt) {
    u8 counts[4] = { 0, 0, 0, 0 };
    u16 sums[4] = { 0, 0, 0, 0 };
    int i;
    u32 bits;
    u8 flag;
    int val;
    int x, z;

    bits = 0;
    for (i = 0; i < 12; i++) {
        bits |= work->balls[i].state << (i * 2);
        val = work->items[i].unk_1B;
        flag = work->balls[i].hit != 0 ? 1 : 0;
        work->balls[i].hit = 0;
        x = work->balls[i].pos.x / FX32_ONE;
        z = work->balls[i].pos.z / FX32_ONE;
        if (x < 1) {
            x = 1;
        } else if (x > 64) {
            x = 64;
        }
        if (z < 1) {
            z = 1;
        } else if (z > 64) {
            z = 64;
        }
        counts[i / 3] += work->balls[i].unk_46 << ((i % 3) * 2);
        pkt->cells[i] = (x - 1) + ((z - 1) << 6) + (val << 12) + (flag << 13) + (work->items[i].unk_19 << 14);
        sums[i / 3] += work->items[i].unk_14;
        if (sums[i / 3] > 999) {
            sums[i / 3] = 999;
        }
    }
    for (i = 0; i < 4; i++) {
        pkt->sums[i] = (counts[i] << 10) + sums[i];
    }
    pkt->bits = bits;
    pkt->bits = bits + (work->unk_5D0 << 24);
    pkt->bits += ((work->unk_5E8 + 30) / 30) << 25;
}
