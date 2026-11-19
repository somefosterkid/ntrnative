#include "global.h"

#include "filesystem.h"
#include "heap.h"
#include "overlay_49.h"
#include "overlay_49_unk_struct_ov70_022615e8.h"

void ov49_0225CDE8(void);

void ov49_0225CDE8(void) {
}

void *ov49_0225D4FC(s32, s32, enum HeapID);

SysTask *ov49_0225D520(void *);

s32 ov49_0225D5FC(void *, NARC *, void *, enum HeapID, void *);

s32 ov49_0225D6F0(void *, void *);

s32 ov49_0225D854(void *, NARC *, void *, void *, enum HeapID);

s32 ov49_0225DC2C(void *, NARC *, void *, void *, enum HeapID);

void ov49_0225CDEC(void *arg0, s8 arg1, s8 arg2, enum HeapID heapID, enum HeapID arg4) {
    NARC *narc;
    void *v0;

    *((s8 *)&((UnkStruct_ov70_022615E8 *)arg0)->unk_127) = arg2;
    *((s8 *)&((UnkStruct_ov70_022615E8 *)arg0)->unk_126) = arg1;
    v0 = ov49_0225D4FC(arg1, arg2, heapID);
    narc = NARC_New(NARC_a_2_0_1, heapID);
    HeapExp_FndInitAllocator(arg0 + 0x494, arg4, 4);
    ov49_0225D5FC(arg0 + 0x12C, narc, v0, arg4, arg0 + 0x494);
    ov49_0225DC2C(arg0 + 0x29C, narc, arg0 + 0x494, v0, arg4);
    ov49_0225D854(arg0 + 0x1C4, narc, arg0 + 0x494, v0, arg4);
    NARC_Delete(narc);
    ov49_0225D520(v0);
    ov49_0225D6F0(arg0, arg0 + 0x12C);
    *((s32 *)&((UnkStruct_ov70_022615E8 *)arg0)->unk_128) = 1;
}
