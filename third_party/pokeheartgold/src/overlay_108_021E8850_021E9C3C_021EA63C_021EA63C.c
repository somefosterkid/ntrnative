#include "global.h"

#include "heap.h"
#include "overlay_108_021E8850.h"
#include "overlay_108_safari_decoration_app.h"
#include "unk_02097268.h"

typedef struct SafariDecorationObjectList {
    u8 count;
    const u8 *ids;
} SafariDecorationObjectList;

extern const SafariDecorationObjectList ov108_021EAD08[];
extern const u8 ov108_021EAD28[][4];

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/overlay_108_021E8850_021E9488_021E979C_021EABF0.s keeps the DS copy; there each entry is {.word count, .word SYMBOL}, which
// the 64-bit port emits as a 4-byte literal next to an 8-byte pointer (wordmix.py): the PC build indexes this C table instead.
// The four id lists are local asm labels (.byte rows at 0x021EACE8..0x021EAD05), so the PC table points at C copies of the same bytes.
static const u8 sObjectIds_021EACE8[] = { 0x00, 0x01, 0x02, 0x13, 0x14, 0x0C };
static const u8 sObjectIds_021EACF0[] = { 0x09, 0x0A, 0x0B, 0x0F, 0x10, 0x12 };
static const u8 sObjectIds_021EACF8[] = { 0x06, 0x07, 0x08, 0x15, 0x16, 0x17 };
static const u8 sObjectIds_021EAD00[] = { 0x03, 0x04, 0x05, 0x0D, 0x0E, 0x11 };

const SafariDecorationObjectList ov108_021EAD08[] = {
    { 6, sObjectIds_021EACE8 },
    { 6, sObjectIds_021EAD00 },
    { 6, sObjectIds_021EACF8 },
    { 6, sObjectIds_021EACF0 },
};
#endif

SafariDecorationEntry *ov108_021EA63C(u32 arg0, s32 arg1, u8 arg2, u8 *arg3, enum HeapID heapID) {
    SafariDecorationEntry *v1;
    u8 *v2;
    s32 gender;
    s32 v0;
    s32 v4;
    s32 v5;
    s32 v10;
    s32 v11;
    u32 idx;
    u32 v13;
    const u8 *v12;
    u8 v7;
    u8 v8;
    const u8 *v3;
    u8 v14;
    u8 v15;
    const void *dest;
    SafariDecorationEntry *v9;
    v0 = arg1;
    v14 = 0;
    gender = arg2;
    v2 = arg3;
    v13 = 0;
    v3 = ov108_021EAD28[arg0 % 10U];
    v10 = 0;
    if (v0 > 0) {
        do {
            v11 = 0;
            v5 = v3[v10];
            v8 = ov108_021EAD08[v5].count;
            if (((s32)v8) > 0) {
                v12 = ov108_021EAD08[v5].ids;
                do {
                    v11 += 1;
                    v13 |= 1 << (*v12);
                    v14 += 1;
                    v12 += 1;
                } while (v11 < ((s32)v8));
            }
            v4 = v10 + 1;
            v10 = v4;
        } while (v4 < v0);
    }
    dest = Heap_Alloc(heapID, v14 * sizeof(SafariDecorationEntry));
    v1 = dest;
    MI_CpuFill8(dest, 0U, v14 * sizeof(SafariDecorationEntry));
    v15 = 0;
    idx = 0;
    do {
        if (1 & v13) {
            v9 = &v1[v15];
            v9->id = idx;
            GetSafariObjectConfig(&v9->config, (s32)idx, gender);
            v7 = ((u8 *)&v9->config)[1];
            v9->slot = ov108_021EA700(((u32)(v7 << 0x1C)) >> 0x1D, ((u32)(v7 << 0x19)) >> 0x1D);
            v15 += 1;
        }
        v13 = v13 >> 1;
        idx += 1;
    } while (((s32)idx) < 0x18);
    *v2 = v15;
    return v1;
}
