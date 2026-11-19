#include "global.h"

#include "overlay_96_ov96hurdle_work.h"

typedef struct Ov96HurdlePacket {
    u32 runners[4]; // 0x00
    u32 unk10;      // 0x10
    u32 unk14;      // 0x14
    u16 unk18[5];   // 0x18
    u16 unk22;      // 0x22
} Ov96HurdlePacket;

void ov96_02205AFC(Ov96HurdleWork *work, Ov96HurdlePacket *pkt);

void ov96_02205AFC(Ov96HurdleWork *work, Ov96HurdlePacket *pkt) {
    int i;
    u8 a = 0;
    u8 b = 0;
    u8 c = 0;
    for (i = 0; i < 5; i++) {
        pkt->unk18[i] = work->slots[i].value;
    }
    for (i = 0; i < 4; i++) {
        Ov96HurdleRunner *r = &work->runners[i];
        pkt->runners[i] = (r->unk_AA << 28) | ((r->unk_AC << 18) | ((r->pos.x / FX32_ONE) | ((r->pos.y / FX32_ONE) << 9)));
        pkt->unk18[i] |= r->unk_B1 << 5;
        pkt->unk18[i] |= (r->unk_B0 - 1) << 7;
        pkt->unk18[i] |= r->unk_A6 << 9;
        pkt->unk18[i] |= r->unk_A7 << 11;
        b |= r->unk_AB << i;
        a |= r->unk_9D << (i * 2);
        c |= work->unk_6BC[i] << i;
    }
    pkt->unk14 = 0;
    pkt->unk14 |= work->unk_51E;
    pkt->unk14 |= work->unk_51C << 9;
    pkt->unk14 |= work->unk_519 << 18;
    pkt->unk14 |= c << 19;
    pkt->unk10 = 0;
    pkt->unk10 |= work->unk_522;
    pkt->unk10 |= work->unk_520 << 9;
    pkt->unk10 |= work->unk_51B << 18;
    pkt->unk10 |= b << 19;
    pkt->unk10 |= a << 23;
    pkt->unk10 |= work->unk_50E << 31;
    pkt->unk22 = work->unk_50C;
}
