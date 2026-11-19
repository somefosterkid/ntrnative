#include "global.h"

#include "error_handling.h"

s32 ov111_021E5C94(s32 arg0);

s32 ov111_021E5C94(s32 arg0) {
    switch (arg0) {
    case 0:
        return 1;
    case 1:
        return 0;
    default:
        GF_AssertFail();
        GF_AssertFail();
        return 0;
    }
}
