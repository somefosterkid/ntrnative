#include "global.h"

#include "error_handling.h"
#include "pm_string.h"

typedef struct UnkStruct_ov99_021E9518 {
    void *unk_0;
    u8 unk_4[0x1B8];
    String *unk_1BC;
} UnkStruct_ov99_021E9518;

String *ov99_021E9518(UnkStruct_ov99_021E9518 *arg0, s32 arg1, s32 arg2);

String *ov99_021E9518(UnkStruct_ov99_021E9518 *arg0, s32 arg1, s32 arg2) {
    if (arg0 == NULL) {
        GF_AssertFail();
    }
    CopyU16ArrayToString(arg0->unk_1BC, (const u16 *)(*(u8 **)((u8 *)(arg0->unk_0) + 4) + (0xA4 * arg1) + 0x30 + (0x18 * arg2)));
    return arg0->unk_1BC;
}
