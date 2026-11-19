#include "global.h"

typedef struct UnkStruct_ov71_02249A50 {
    u8 unk_0[0x4];
    s32 unk_4;
} UnkStruct_ov71_02249A50;

s32 ov71_02249A50(UnkStruct_ov71_02249A50 *arg0);

s32 ov71_02249A50(UnkStruct_ov71_02249A50 *arg0) {
    if (arg0->unk_4 == 0) {
        return 1;
    }
    return 0;
}
