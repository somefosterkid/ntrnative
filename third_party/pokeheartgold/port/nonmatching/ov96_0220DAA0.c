s32 ov96_021EB0CC(s32, s32, s32, s32, s32);
extern VecFx32 ov96_0221CEF4;

s8 ov96_0220DAA0(void *arg0, u8 arg1, s32 arg2) {
    s32 best;
    s32 i;
    s32 min;
    s32 d;
    s32 *pd;
    u8 *p;
    VecFx32 dist;

    p = arg0;
    best = 0xC;
    pd = &dist.x;
    dist = ov96_0221CEF4;
    min = 0x100000;
    for (i = 0; i < 3; i++) {
        if (ov96_021EB0CC(*(s32 *)(p + 4), *(s32 *)(p + 0x1C), *(s32 *)(p + 0x20), arg1, arg2) != 0) {
            VecFx32 vec = { 0, 0, 0 };
            d = (arg1 << 12) - *(s32 *)(p + 0x1C);
            if (d < 0) {
                d = -d;
            }
            vec.x = d;
            d = (arg2 << 12) - *(s32 *)(p + 0x20);
            if (d < 0) {
                d = -d;
            }
            vec.y = d;
            *pd = VEC_Mag(&vec);
        }
        p += 0x48;
        pd++;
    }
    pd = &dist.x;
    for (i = 0; i < 3; i++) {
        if (pd[i] < min) {
            min = pd[i];
            best = i;
        }
    }
    return best;
}
