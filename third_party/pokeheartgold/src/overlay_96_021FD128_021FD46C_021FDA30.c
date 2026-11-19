#include "global.h"

#include "overlay_96_ov96_rower_work.h"

typedef struct Ov96RowerPacket {
    s16 x[4];    // 0x00
    u8 y[4];     // 0x08
    u8 a[4];     // 0x0C
    u8 b[4];     // 0x10
    u32 bits;    // 0x14
    u16 unk18;   // 0x18
    u16 unk1A;   // 0x1A
    u8 unk1C;    // 0x1C
    u8 unk1D;    // 0x1D
    u8 unk1E[4]; // 0x1E
} Ov96RowerPacket;

void ov96_021FDA30(Ov96RowerWork *work, Ov96RowerPacket *pkt);

void ov96_021FDA30(Ov96RowerWork *work, Ov96RowerPacket *pkt) {
    int i;
    int k;
    u8 hi;
    u32 bits;
    u8 flags;
    Ov96Rower *r;

    hi = 0;
    bits = 0;
    flags = 0;
    i = 0;
    r = work->rowers;
    for (; i < 4; i++, r++) {
        u8 a;
        u8 b;
        a = 0;
        a += r->subIdx;
        a += r->unkA2 << 2;
        hi += r->unk9E << (i * 2);
        b = (r->unk9D << 7) + r->unk9C;
        pkt->y[i] = r->unk80 / FX32_ONE;
        pkt->x[i] = r->unk7C / FX32_ONE;
        pkt->a[i] = a;
        pkt->b[i] = b;
        for (k = 0; k < 3; k++) {
            bits += r->subs[k].unk18 << ((k + i * 3) * 2);
        }
        pkt->unk1E[i] = work->unk644[i];
        flags += r->unkD1 << i;
    }
    pkt->unk1D = hi;
    pkt->bits = bits;
    pkt->bits += work->unk3C0 << 24;
    pkt->bits += work->unk63C << 25;
    pkt->bits += flags << 26;
    pkt->unk1A = work->unk63E;
    pkt->unk1C = work->unk63D;
    pkt->unk18 = work->unk640;
}
