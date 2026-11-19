#include "global.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"
struct UnkStruct_ov70_022629FC;

void ov49_0225EB84(void *arg0);

void ov49_0225EB84(void *arg0) {
    u16 v0;

    v0 = *(u16 *)((u8 *)arg0 + 4);
    if ((u32)(u16)(v0 + 0xFFFD) > 1U) {
        if ((u32)v0 <= 1U) {
            *(u16 *)((u8 *)arg0 + 4) = 0U;
            return;
        }
        *(u16 *)((u8 *)arg0 + 4) = 3U;
    }
}

s32 ov49_0225D214(void *, void *, u8, void *);

s32 ov49_0225D494(u8 *, s32);

s32 ov49_0225D4C8(void *, s32);

void ov49_0225EBA8(void *arg0, void *arg1);

void ov49_0225EBA8(void *arg0, void *arg1) {
    ov49_0225D4C8(*(void **)arg0, *((s32 *)&((UnkStruct_ov70_022629FC *)arg0)->unk_08));
    ov49_0225D214(arg1, *(void **)arg0, 0, 0);
    ov49_0225D214(arg1, *(void **)arg0, 1, 0);
    ov49_0225D214(arg1, *(void **)arg0, 2, 0);
    ov49_0225D494(*(void **)arg0, 1);
}

s32 ov49_0225D328(s32, void *, s32);

s32 ov49_0225D4E8(void *);

s32 ov49_0225D4F0(void *, s32, s32, s32);

void ov49_0225EBE4(void **arg0, s32 arg1);

void ov49_0225EBE4(void **arg0, s32 arg1) {
    ov49_0225D328(arg1, *arg0, 0);
    ov49_0225D328(arg1, *arg0, 1);
    ov49_0225D328(arg1, *arg0, 2);
    ov49_0225D494(*arg0, 0);
    ov49_0225D4E8(*arg0);
    ov49_0225D4F0(*arg0, 0x1000, 0x1000, 0x1000);
}

void ov49_0225EC28(struct UnkStruct_ov70_022629FC *arg0);

void ov49_0225EC28(UnkStruct_ov70_022629FC *arg0) {
    arg0->unk_06 = 0;
}
