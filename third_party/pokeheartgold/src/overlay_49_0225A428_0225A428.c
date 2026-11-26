#include "global.h"

#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov49_0225B89C(UnkStruct_ov70_0225F350 *, s8, s16);
#else
void ov49_0225B89C(UnkStruct_ov70_0225F350 *, s32, s32);
#endif

u8 ov49_0225B8F8(UnkStruct_ov70_0225F350 *);

s32 ov49_0225B8FC(UnkStruct_ov70_0225F350 *);

s32 ov49_0225B928(UnkStruct_ov70_0225F350 *);

s32 ov49_0225B934(UnkStruct_ov70_0225F350 *);

void ov49_0225A428(UnkStruct_ov70_0225DEE8 *arg0, s32 arg1, s32 arg2);

void ov49_0225A428(UnkStruct_ov70_0225DEE8 *arg0, s32 arg1, s32 arg2) {
    if ((arg1 != ov49_0225B8F8(&arg0->unk_1E8)) || (ov49_0225B8FC(&arg0->unk_1E8) != 1) || (ov49_0225B934(&arg0->unk_1E8) != 0) || (arg2 != ov49_0225B928(&arg0->unk_1E8))) {
        ov49_0225B89C(&arg0->unk_1E8, arg1, arg2);
    }
}

void ov49_0225B8A8(UnkStruct_ov70_0225F350 *, UnkStruct_ov70_0225E4EC *, u32, s32);

void ov49_0225A478(UnkStruct_ov70_0225DEE8 *arg0, s32 arg1);

void ov49_0225A478(UnkStruct_ov70_0225DEE8 *arg0, s32 arg1) {
    ov49_0225B8A8(&arg0->unk_1E8, &arg0->unk_3C, arg1, 0x77);
}
