#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96_rd_obj.h"

s32 ov96_0221862C(Ov96RdObj *obj);


void ov96_02219460(Ov96RdObj *arg0, Ov96RdFlags *arg1, s32 arg2);

void ov96_02219460(Ov96RdObj *arg0, Ov96RdFlags *arg1, s32 arg2) {
    s32 temp;
    u8 *dst;
    s32 lt;
    s8 val;

    temp = arg0->info->value >> 12;
    dst = &arg0->b5F;
    if (temp < 30) {
        lt = 1;
    } else {
        lt = 0;
    }
    if (ov96_0221862C(arg0) != 0) {
        if (lt != 0) {
            val = 1;
        } else {
            val = 0;
        }
    } else {
        val = 3;
    }
    if (arg2 == 0) {
        *dst = val;
        return;
    }

    if (arg1->b14) {
        if ((u32)val <= 1U) {
            *dst = 2;
            return;
        }
        if (val == 3) {
            *dst = 4;
            return;
        }
        GF_AssertFail();
        return;
    }
    if (arg1->b15 == 0) {
        *dst = val;
    }
}
