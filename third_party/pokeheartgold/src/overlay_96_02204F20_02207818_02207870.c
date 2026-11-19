#include "global.h"

#include "error_handling.h"
#include "overlay_96_ov96hurdle_work.h"

s32 ov96_02207818(void *);

void ov96_02207870(void *arg0);

void ov96_02207870(void *arg0) {
    s32 temp_r6;
    s32 var_r4;
    var_r4 = 0;
    temp_r6 = 5 - (((Ov96HurdleWork *)arg0)->unk_61A);
    if (temp_r6 > 0) {
        do {
            ov96_02207818(arg0 + 0x564);
            ((Ov96HurdleWork *)arg0)->unk_61A += 1;
            if (((u32)(((Ov96HurdleWork *)arg0)->unk_61A)) > 5U) {
                GF_AssertFail();
            }
            var_r4 += 1;
        } while (var_r4 < temp_r6);
    }
}
