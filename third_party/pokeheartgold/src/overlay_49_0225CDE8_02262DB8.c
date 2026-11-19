#include "global.h"

#include "overlay_49.h"
#include "overlay_49_unk_struct_ov70_02266af0.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

s32 ov49_0225A22C(UnkStruct_ov70_0225DEE8 *);

void ov49_02262DB8(UnkStruct_ov70_02266C68 *countdown, UnkStruct_ov70_0225DEE8 *plaza) {
#if defined(SDK_PORT) || defined(NONMATCHING)
    countdown->unk_00 = 0;
    countdown->unk_01 = 0;
    countdown->unk_02 = 0;
    countdown->unk_03 = 0;
    countdown->unk_04 = 0;
    countdown->unk_06 = 0;
    countdown->unk_07 = 0;
#else
    // the DS clears all 8 bytes one by one (unk_04 as two byte stores)
    u8 *bytes = (u8 *)countdown;
    bytes[0] = 0;
    bytes[1] = 0;
    bytes[2] = 0;
    bytes[3] = 0;
    bytes[4] = 0;
    bytes[5] = 0;
    bytes[6] = 0;
    bytes[7] = 0;
#endif
    ov49_0225A22C(plaza);
}

void ov49_02262DD4(UnkStruct_ov70_02266C68 *countdown, s32 frames) {
    s16 v0;

    v0 = (s16)(frames / 30);
    if (v0 != countdown->unk_04) {
        countdown->unk_04 = v0;
        countdown->unk_07 = (u8)(8 | countdown->unk_07);
    }
}

void ov49_02262DF8(UnkStruct_ov70_02266C68 *countdown, UnkStruct_ov70_0225DEE8 *plaza, s32 arg2) {
    ov49_02262E10(countdown, plaza, arg2, 0);
}

void ov49_02262E04(UnkStruct_ov70_02266C68 *countdown, UnkStruct_ov70_0225DEE8 *plaza, s32 arg2) {
    ov49_02262E10(countdown, plaza, arg2, 1);
}
