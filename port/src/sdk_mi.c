// Black's memory helpers that libntr lacks (lib/nitro/src/mi/mi_memory.c in the ROM build, ARM assembly there). The
// PC versions do what the assembly does inside the destination's bounds, with libc.
#include <string.h>

#include "sdk_black.h"

// memset of value's low byte
void sys_memset_fast(void *dest, u32 value, u32 size)
{
    memset(dest, (u8)value, size);
}

// A forward copy. The assembly copies forwards in blocks, so it is also right for an overlap with the source above the
// destination (sys_memcpy_ex relies on that): memmove is right for both
void sys_memcpy_fast(const void *src, void *dest, u32 size)
{
    memmove(dest, src, size);
}

// memmove: forwards when the source is above the destination, else from the end
void sys_memcpy_ex(const void *src, void *dest, u32 size)
{
    memmove(dest, src, size);
}

// Bytes [from, to) of a clear that stores the word data in each word from d on: byte k gets data's byte k % 4
static void ClearWords(u8 *d, u32 from, u32 to, u32 data)
{
    const u8 *w = (const u8 *)&data;
    u32 k = from;

    for (; k < to && (k & 3) != 0; k++) {
        d[k] = w[k & 3];
    }
    for (; k + 4 <= to; k += 4) {
        memcpy(d + k, &data, 4);
    }
    for (; k < to; k++) {
        d[k] = w[k & 3];
    }
}

// The end of each bank's part of the LCDC view (HW_LCDC_VRAM), A to I, from the view's start
static const u32 LCDC_BANK_END[9] = {
    0x20000, 0x40000, 0x60000, 0x80000, 0x90000, 0x94000, 0x98000, 0xa0000, 0xa4000,
};

// NitroSDK's MIi_CpuClearFast: the word data in each word of size bytes (32 bytes at a time in the assembly). Size is a
// multiple of 4 in the SDK's contract; the assembly stores a whole last word for any other size, past the end. Here a
// last partial word gets data's low bytes only, so a PC allocation of exactly size bytes isn't overrun.
// The DS shows a VRAM bank in the LCDC view only while the bank is set for LCDC (GX_SetBankForLCDC): a store to another
// bank's part of the view is lost. libntr's LCDC view is the banks' memory itself, whatever they are set for, so the
// clear skips the banks not set for LCDC. field_transition.c's building-entry fade clears HW_LCDC_VRAM (bank A, the
// field's textures) with only bank D set for LCDC: nothing on the DS, every texture of the house's map, props and
// sprites on PC (stop 24b).
void MIi_CpuClearFast(u32 data, void *destp, u32 size)
{
    u8 *d = (u8 *)destp;
    uPtr start = (uPtr)d;
    uPtr lcdc = (uPtr)HW_LCDC_VRAM;
    uPtr lcdcEnd = lcdc + LCDC_BANK_END[8];
    u32 k = 0;

    if (start >= lcdcEnd || start + size <= lcdc) {
        ClearWords(d, 0, size, data);
        return;
    }
    while (k < size) {
        uPtr a = start + k;
        u32 n = size - k;
        BOOL store = TRUE;

        if (a < lcdc) {
            if (lcdc - a < n) {
                n = (u32)(lcdc - a);
            }
        } else if (a < lcdcEnd) {
            u32 off = (u32)(a - lcdc);
            u32 bank = 0;

            while (off >= LCDC_BANK_END[bank]) {
                bank++;
            }
            if (LCDC_BANK_END[bank] - off < n) {
                n = LCDC_BANK_END[bank] - off;
            }
            store = (GX_GetBankForLCDC() >> bank) & 1;
        }
        if (store) {
            ClearWords(d, k, k + n, data);
        }
        k += n;
    }
}
