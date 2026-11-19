#include "global.h"
struct UnkStruct_ov115_02261ADC;

#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "heap.h"
#include "overlay_91.h"
#include "overlay_91_unk_struct_ov115_02261_adc.h"

void ov91_0225F434(void);

s32 ov91_0225FD2C(void *, s32, s32, enum HeapID);

static void ov91_0225F4C0(UnkStruct_ov115_02261ADC *arg0, s32 arg1, s32 arg2, enum HeapID heapID) {
    *((GF3DVramMan * *)&arg0->unk_18) = GF_3DVramMan_Create(heapID, 0, 1, 0, 4, ov91_0225F434);
    ov91_0225FD2C(arg0, arg1, arg2, heapID);
}

s32 ov91_0225FDC8(UnkStruct_ov115_02261ADC *param0);

void ov91_0225F4F8(struct UnkStruct_ov115_02261ADC *arg0);

void ov91_0225F4F8(UnkStruct_ov115_02261ADC *arg0) {
    ov91_0225FDC8(arg0);
    GF_3DVramMan_Delete(*((GF3DVramMan * *)&arg0->unk_18));
}

void *ov90_02258BD4(s32, enum HeapID);

s32 ov91_0225D628(void *, void *, s32, s32, enum HeapID);

s32 ov91_0225D8F0(void *, void *, void *, s32, s32, NARC *, enum HeapID);

s32 ov91_0225DBD0(void *, s32, s32, enum HeapID);

s32 ov91_0225F414(void);

s32 ov91_0225F878(void *, enum HeapID);

s32 ov91_0225FA9C(void *, enum HeapID);

s32 ov91_0225FB80(void *, NARC *, enum HeapID);

s32 ov91_0225FCD8(void *, enum HeapID);

s32 ov91_0225FDE0(void *, NARC *, s32, enum HeapID);

s32 ov91_0225FF00(void *, NARC *, enum HeapID, void *);

s32 ov91_022604F4(void *, NARC *, enum HeapID);

s32 ov91_02260C48(void *, void *, s32, enum HeapID);

s32 ov91_02260D14(void *, void *, s32, enum HeapID);

s32 ov91_02260F50(void *, void *, NARC *, s32, s32, enum HeapID, void *);

s32 ov91_022613AC(void *, u8);

s32 ov91_02261580(void *, void *, NARC *, enum HeapID);

void ov91_0225F508(void *arg0, enum HeapID arg1);

void ov91_0225F508(void *arg0, enum HeapID heapID) {
    NARC *narc;

    HeapExp_FndInitAllocator(arg0 + 0x86CC, heapID, 0x20);
    ov91_0225F414();
    ov91_0225F4C0(arg0 + 0x1AB4, *(s32 *)((u8 *)arg0 + 0x10), *(s32 *)((u8 *)arg0 + 0x14), heapID);
    ov91_0225F878(arg0 + 0x1AB4, heapID);
    ov91_0225FA9C(arg0 + 0x1AB4, heapID);
    ov91_0225FCD8(arg0 + 0x1AB4, heapID);
    narc = NARC_New(NARC_a_1_8_9, heapID);
    ov91_0225FB80(arg0 + 0x1AB4, narc, heapID);
    ov91_02260F50(arg0 + 0x1C54, arg0 + 0x1AB4, narc, *(s32 *)((u8 *)arg0 + 0x10), *(s32 *)((u8 *)arg0 + 0x14), heapID, arg0 + 0x86CC);
    ov91_0225FF00(arg0 + 0x1F74, narc, heapID, arg0 + 0x86CC);
    ov91_022604F4(arg0 + 0x1AB4, narc, heapID);
    ov91_02260C48(arg0 + 0x1AB4, arg0 + 0x8544, *(s32 *)((u8 *)arg0 + 0x14), heapID);
    ov91_02260D14(arg0 + 0x1AB4, arg0 + 0x854C, *(s32 *)((u8 *)arg0 + 0x14), heapID);
    ov91_0225FDE0(arg0 + 0x1AB4, narc, *(s32 *)((u8 *)arg0 + 0x14), heapID);
    ov91_0225D8F0(arg0 + 0x1F38, arg0 + 0x1AB4, *(void **)((u8 *)arg0 + 0x877C), *(s32 *)((u8 *)arg0 + 0x10), *(s32 *)((u8 *)arg0 + 0x14), narc, heapID);
    ov91_0225DBD0(arg0 + 0x1AB4, *(s32 *)((u8 *)arg0 + 0x10), *(s32 *)((u8 *)arg0 + 0x14), heapID);
    ov91_0225D628(arg0 + 0x1E38, arg0 + 0x1AB4, *(s32 *)((u8 *)arg0 + 0x10), *(s32 *)((u8 *)arg0 + 0x14), heapID);
    ov91_022613AC(arg0 + 0x21A8, (u8) * (s32 *)((u8 *)arg0 + 0x14));
    ov91_02261580(arg0 + 0x862C, arg0 + 0x1AB4, narc, heapID);
    NARC_Delete(narc);
    *(void **)((u8 *)arg0 + 0x21A4) = ov90_02258BD4(*(s32 *)((u8 *)arg0 + 0x1AD0), heapID);
}
