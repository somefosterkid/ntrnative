#include "global.h"
#include "overlay_49_unk_struct_ov70_022629_fc.h"

s32 ov49_0225D214(void *, void *, u8, s32);

s32 ov49_0225D494(u8 *, s32);

s32 ov49_0225D4C8(void *, s32);

void ov49_0225ED68(void *arg0, void *arg1);

void ov49_0225ED68(void *arg0, void *arg1) {
    ov49_0225D4C8(*(void **)arg0, *((s32 *)&((UnkStruct_ov70_022629FC *)arg0)->unk_08));
    ov49_0225D214(arg1, *(void **)arg0, 0, 1);
    ov49_0225D214(arg1, *(void **)arg0, 1, 1);
    ov49_0225D494(*(void **)arg0, 1);
}
