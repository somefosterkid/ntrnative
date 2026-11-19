void ov96_021F6524(u8 *a, int b, u8 c, u8 d) {
    s32 *arr = (s32 *)(a + 0xe00);
    s32 *pLo;
    s32 *pHi;
    u8 *loIdx;
    u8 *hiIdx;
    s32 n;
    s32 *row;
    s32 loVal;
    s32 hiVal;
    s32 i;

    if (arr[d] < arr[c]) {
        pLo = &arr[d];
        pHi = &arr[c];
        loIdx = &d;
        hiIdx = &c;
    } else {
        pLo = &arr[c];
        pHi = &arr[d];
        loIdx = &c;
        hiIdx = &d;
    }
    row = (s32 *)((a + b * 0x200) + 0x800);
    for (i = 0; i < 0x80; i++) {
        row[i] = 0;
    }
    loVal = *(s32 *)(a + *loIdx * 0x200 + *pLo * 4 + 0x200);
    hiVal = *(s32 *)(a + *hiIdx * 0x200 + *pHi * 4 + 0x200);
    n = *pHi - *pLo;
    for (i = 0; i < n - 1; i++) {
        *(s32 *)((a + b * 0x200) + (*pLo + i) * 4 + 0x804) = loVal + (i + 1) * (hiVal - loVal) / n;
    }
}
