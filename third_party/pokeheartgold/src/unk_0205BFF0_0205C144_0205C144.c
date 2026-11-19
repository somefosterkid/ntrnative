#include "global.h"

#include "error_handling.h"
#include "unk_0205BFF0.h"

u32 sub_0205C144(u8 a0) {
    u32 unaff_r4;

    switch (a0) {
    case 0:
        unaff_r4 = 0;
        break;
    case 1:
        unaff_r4 = 1;
        break;
    case 2:
        unaff_r4 = 2;
        break;
    case 3:
        unaff_r4 = 2;
        GF_AssertFail();
    }
    return unaff_r4;
}
