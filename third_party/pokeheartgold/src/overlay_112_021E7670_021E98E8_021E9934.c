#include "global.h"

void ov112_021E9934(u8 *data, int y, int x, u32 flags);

void ov112_021E9934(u8 *data, int y, int x, u32 flags) {
    int idx = x / 8;
    int bit = x % 8;
    u32 f2 = flags & 2;
    int off;
    u32 mask;
    if (flags & 1) {
        mask = 1 << bit;
        off = y * 2 + idx * 0x80;
        data[off] = data[off] | mask;
    } else {
        mask = 1 << bit;
        off = y * 2 + idx * 0x80;
        data[off] &= (0xff ^ mask);
    }
    if (f2) {
        data[off + 1] = data[off + 1] | mask;
    } else {
        data[off + 1] &= (0xff ^ mask);
    }
}
