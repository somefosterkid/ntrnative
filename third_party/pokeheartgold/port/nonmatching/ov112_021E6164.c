/* PORTOK hk46 */
#include "global.h"

/* PORTOK hk46: rewritten from the asm.
   ov112_021FFAA4 (raw offsets as in the sibling TUs of this unit, e.g. ov112_021E6A6C):
     +0x04 u32 index 0..3 (which block), +0x0C u32 block-3 source, +0x30 u32 base (+0x224 = data), +0x34 u8 *flags block
     (byte 0: bits 0-3 "block k done", bits 4-7 "block k bad"), +0x38 u8 *header (byte-swapped in place), +0x3C/+0x40 u64 tick.
   ov112_021FFB24: +0x1C u8 received command, +0x1D u8 state. */
extern u8 ov112_021FFAA4[];
extern u8 ov112_021FFB24[];

#define AA4(off, T) (*(T *)(ov112_021FFAA4 + (off)))
#define B24_CMD (ov112_021FFB24[0x1C])
#define B24_STATE (ov112_021FFB24[0x1D])

void ov112_021E5A68(void *buf, s32 size, u8 cmd, s32 flag);
void ov112_021E5EC4(s32 arg0, s32 arg1, u16 arg2);
void ov112_021E5EEC(void);
void ov112_021E6134(void);
void OS_Halt(void);

static inline u32 Swap32(u32 x) {
    return ((x >> 8) & 0xFF00) | (u8)(x >> 24) | ((x << 8) & 0xFF0000) | ((x << 24) & 0xFF000000);
}

static inline u16 Swap16(u16 x) {
    return (u16)((u8)(x >> 8) | ((x << 8) & 0xFF00));
}

/* send block AA4(4) (0..3): state = 0xB0 + 2 * index, copy it out, then ov112_021E5EEC */
static inline void ov112_SendBlock(void) {
    switch (AA4(0x04, u32)) {
    case 0:
        B24_STATE = 0xB0;
        ov112_021E5EC4((s32)(AA4(0x34, u8 *) + 4), 0xB804, 0x240);
        break;
    case 1:
        B24_STATE = 0xB2;
        ov112_021E5EC4((s32)(AA4(0x34, u8 *) + 0x244), 0xBA44, 0x2FC);
        break;
    case 2:
        B24_STATE = 0xB4;
        ov112_021E5EC4((s32)(AA4(0x34, u8 *) + 0x540), 0xBD40, 0x188);
        break;
    case 3:
        B24_STATE = 0xB6;
        ov112_021E5EC4(AA4(0x0C, s32), 0xBF00, 0xCBC);
        break;
    }
    ov112_021E5EEC();
}

/* commands 0xB8/0xBA/0xBC/0xBE: block k = (cmd - 0xB8) / 2 is done; mask = 0xF & ~(1 << k) */
static inline void ov112_BlockAck(u8 cmd, u8 bit) {
    u8 *flags;
    u8 low;

    /* "bad" bit (4 + index) of the block being sent */
    switch (AA4(0x04, u32)) {
    case 0:
        if ((*AA4(0x34, u8 *) >> 4) & 1) {
            ov112_021E5A68(NULL, 0, 0x9C, 1);
            return;
        }
        break;
    case 1:
        if ((*AA4(0x34, u8 *) >> 5) & 1) {
            ov112_021E5A68(NULL, 0, 0x9C, 1);
            return;
        }
        break;
    case 2:
        if ((*AA4(0x34, u8 *) >> 6) & 1) {
            ov112_021E5A68(NULL, 0, 0x9C, 1);
            return;
        }
        break;
    case 3:
        if ((*AA4(0x34, u8 *) >> 7) & 1) {
            ov112_021E5A68(NULL, 0, 0x9C, 1);
            return;
        }
        break;
    }

    flags = AA4(0x34, u8 *);
    low = *flags & 0xF;
    if ((low & (0xF & ~bit)) == (0xF & ~bit)) {
        /* all other blocks done: mark this one and send the block */
        *flags = (*flags & ~0xF) | ((u8)(low | bit) & 0xF);
        ov112_SendBlock();
        return;
    }
    if (low & bit) {
        ov112_021E5A68(NULL, 0, 0x9E, 1);
        return;
    }
    B24_STATE = cmd;
    ov112_021E5A68(NULL, 0, 0x24, 1);
}

void ov112_021E6164(void);

void ov112_021E6164(void) {
    f32 elapsed;
    u8 *hdr;
    u8 *entry;
    int i;

    elapsed = (f32)(((OS_GetTick() - AA4(0x3C, u64)) << 6) / 0x82EA) / 1000.0f; /* computed, unused (as in the asm) */
    (void)elapsed;

    switch (B24_STATE) {
    case 0x46:
        B24_STATE = 0x48;
        ov112_021E5EC4(0xCE80, AA4(0x38, s32), 0xD4C);
        ov112_021E6134();
        return;

    case 0x48:
        /* the received header is big-endian: swap it in place */
        hdr = AA4(0x38, u8 *);
        *(u16 *)(hdr + 0xA) = Swap16(*(u16 *)(hdr + 0xA));
        for (i = 0; i < 7; i++) {
            u32 *p = (u32 *)(AA4(0x38, u8 *) + 0x70 + i * 4);
            *p = Swap32(*p);
        }
        hdr = AA4(0x38, u8 *);
        *(u32 *)(hdr + 0) = Swap32(*(u32 *)(hdr + 0));
        hdr = AA4(0x38, u8 *);
        *(u32 *)(hdr + 4) = Swap32(*(u32 *)(hdr + 4));
        entry = AA4(0x38, u8 *) + 0x8C;
        for (i = 0; i < 0x18; i++) {
            *(u32 *)(entry + 0x00) = Swap32(*(u32 *)(entry + 0x00));
            *(u16 *)(entry + 0x78) = Swap16(*(u16 *)(entry + 0x78));
            *(u16 *)(entry + 0x7A) = Swap16(*(u16 *)(entry + 0x7A));
            *(u32 *)(entry + 0x7C) = Swap32(*(u32 *)(entry + 0x7C));
            *(u32 *)(entry + 0x80) = Swap32(*(u32 *)(entry + 0x80));
            entry += 0x88;
        }
        switch (B24_CMD) {
        case 0x60:
            B24_STATE = 0x4C;
            ov112_021E5EC4(0xDE24, AA4(0x30, s32) + 0x224, 0x1568);
            ov112_021E6134();
            return;
        case 0xB0:
        case 0xB2:
        case 0xB4:
        case 0xB6:
        case 0xB8:
        case 0xBA:
        case 0xBC:
        case 0xBE:
            B24_STATE = 0x4A;
            ov112_021E5EC4(0xB800, AA4(0x34, s32), 4);
            ov112_021E6134();
            return;
        default:
            B24_STATE = 0x4A;
            ov112_021E5EC4(0xB800, AA4(0x34, s32), 0x6C8);
            ov112_021E6134();
            return;
        }

    case 0x4A:
        switch (B24_CMD) {
        case 0x52:
            ov112_021E5A68(NULL, 0, 0x24, 1);
            return;
        case 0x40:
            B24_STATE = 0x4C;
            ov112_021E5EC4(0xDE24, AA4(0x30, s32) + 0x224, 0x1568);
            ov112_021E6134();
            return;
        case 0xB0:
            if ((*AA4(0x34, u8 *) >> 4) & 1) {
                ov112_021E5A68(NULL, 0, 0x9C, 1);
                return;
            }
            B24_STATE = 0xB0;
            ov112_021E5EC4((s32)(AA4(0x34, u8 *) + 4), 0xB804, 0x240);
            ov112_021E5EEC();
            return;
        case 0xB2:
            if ((*AA4(0x34, u8 *) >> 5) & 1) {
                ov112_021E5A68(NULL, 0, 0x9C, 1);
                return;
            }
            B24_STATE = 0xB2;
            ov112_021E5EC4((s32)(AA4(0x34, u8 *) + 0x244), 0xBA44, 0x2FC);
            ov112_021E5EEC();
            return;
        case 0xB4:
            if ((*AA4(0x34, u8 *) >> 6) & 1) {
                ov112_021E5A68(NULL, 0, 0x9C, 1);
                return;
            }
            B24_STATE = 0xB4;
            ov112_021E5EC4((s32)(AA4(0x34, u8 *) + 0x540), 0xBD40, 0x188);
            ov112_021E5EEC();
            return;
        case 0xB6:
            if ((*AA4(0x34, u8 *) >> 7) & 1) {
                ov112_021E5A68(NULL, 0, 0x9C, 1);
                return;
            }
            B24_STATE = 0xB6;
            ov112_021E5EC4(AA4(0x0C, s32), 0xBF00, 0xCBC);
            ov112_021E5EEC();
            return;
        case 0xB8:
            ov112_BlockAck(0xB8, 1);
            return;
        case 0xBA:
            ov112_BlockAck(0xBA, 2);
            return;
        case 0xBC:
            ov112_BlockAck(0xBC, 4);
            return;
        case 0xBE:
            ov112_BlockAck(0xBE, 8);
            return;
        default:
            OS_Halt();
            return;
        }

    case 0x4C:
        ov112_021E5A68(NULL, 0, 0x24, 1);
        return;
    }
}
