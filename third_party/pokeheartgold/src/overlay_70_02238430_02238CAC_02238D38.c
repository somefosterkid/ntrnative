#include "global.h"

#include "unk_02005D10.h"

void ov70_02238D38(void *arg0, s32 arg1, void *arg2, u32 arg3);

void ov70_02238D38(void *arg0, s32 arg1, void *arg2, u32 arg3) {
    switch (arg3) {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
        PlaySE(0x5DCU);
        break;
    }
}
