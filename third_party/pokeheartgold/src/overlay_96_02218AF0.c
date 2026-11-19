#include "global.h"
#include "overlay_96_ov96_rd_obj.h"
struct Ov96RdObj;

void ov96_02218AF0(struct Ov96RdObj *arg0);

void ov96_02218AF0(Ov96RdObj *arg0) {
    s32 v0;
    s32 v1;

    if ((u32)(arg0->state - 9) > 2U) {
        v1 = arg0->unk_50;
        if (v1 <= 0) {
            arg0->w54 = 0;
            return;
        }
        v0 = v1 - arg0->w54;
        arg0->unk_50 = v0;
        if (v0 < 0) {
            arg0->unk_50 = 0;
        }
        arg0->w54 = (s32)(arg0->w54 + 0x2000);
    }
}
