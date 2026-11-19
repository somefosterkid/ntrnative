#include "global.h"

#include "error_handling.h"

void ov96_0221A730(s32 *param_1);

extern u8 ov96_0221D9C8[];

void ov96_0221A5B8(s32 *param_1, s32 param_2);

void ov96_0221A5B8(s32 *param_1, s32 param_2) {
    if (*(s32 *)(param_2 * 4 + ((u8 *)ov96_0221D9C8)) == 0) {
        ov96_0221A730(param_1);
        return;
    }
    GF_AssertFail();
    return;
}
