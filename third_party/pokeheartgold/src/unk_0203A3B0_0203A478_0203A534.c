#include "global.h"

#include "error_handling.h"
#include "system.h"

extern s8 _0210F940;

extern s8 _0210F954;

extern s8 _0210F964;

NNSG2dCharacterData *sub_0203A534(s32 arg0, s32 arg1, s32 arg2, void *arg3);

NNSG2dCharacterData *sub_0203A534(s32 arg0, s32 arg1, s32 arg2, void *arg3) {
    NNSG2dCharacterData *sp0;
    if (arg0) {
        if (arg1 == 0x300010) {
            GF_AssertFail();
        }
        sub_0201A3F8(&_0210F940, (void *)&arg3);
    } else if (arg1 != 0x300010) {
        sub_0201A3F8(&_0210F954, (void *)&arg3);
        DC_FlushRange((void *)arg3, 0x258);
    } else {
        sub_0201A3F8(&_0210F964, (void *)&arg3);
        DC_FlushRange((void *)arg3, 0x4C0);
    }
    NNS_G2dGetUnpackedBGCharacterData((void *)arg3, &sp0);
    return sp0;
}
