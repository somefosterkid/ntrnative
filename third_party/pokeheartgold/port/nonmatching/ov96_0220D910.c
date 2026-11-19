/* PORTOK hk42 */
s32 ov96_021EB0A4(s32, s16, s16, s32 *, s32 *);
extern s32 ov96_0221CFBC;
extern s32 ov96_0221CFBE;


// ov96_0221CFBC: rows of 3 {s16 x, s16 y} (12 bytes); the bitfield at +0x40 bits 16..19 picks row idx/3, column idx%3.
void ov96_0220D910(void *arg0) {
    s32 sp8;
    s32 sp4;
    VecFx32 zero;
    u8 *p = (u8 *)arg0;
    u8 idx = (u8)((u32)(*(s32 *)(p + 0x40) << 0xC) >> 0x1C);
    s32 col = (idx % 3) * 4;
    s32 row = (idx / 3) * 0xC;

    ov96_021EB0A4(*(s32 *)(p + 4), *(s16 *)((u8 *)&ov96_0221CFBC + row + col), *(s16 *)((u8 *)&ov96_0221CFBE + row + col), &sp8, &sp4);
    *(s32 *)(p + 0x1C) = sp8 << 0xC;
    *(s32 *)(p + 0x20) = sp4 << 0xC;
    *(s32 *)(p + 0x10) = sp8 << 0xC;
    *(s32 *)(p + 0x14) = sp4 << 0xC;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    *(VecFx32 *)(p + 0x34) = zero;
    *(s32 *)(p + 0x40) = 0xFBFFFFFF & ((*(s32 *)(p + 0x40) & 0xFF0FFFFF) | 0x200000);
}
