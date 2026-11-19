#include "global.h"

#include "unk_02005D10.h"

s32 ov40_0223EBB8(void *, s32);

s32 ov40_0223EC40(void *, s32);

void ov40_02241E14(s32 arg0, s32 arg1, void *arg2);

void ov40_02241E14(s32 arg0, s32 arg1, void *arg2) {
    if (arg1 == 0) {
        if (ov40_0223EBB8(arg2, arg0) == 0) {
            PlaySE(0x57CU);
            return;
        }
        ov40_0223EC40(arg2, arg0);
    }
}
