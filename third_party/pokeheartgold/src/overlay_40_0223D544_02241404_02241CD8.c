#include "global.h"
#include "overlay_40_unk_struct_0208_c06_c.h"

s32 ov40_0222BF80(s32, s32);

s32 ov40_02230944(s32);

s32 ov40_02241BDC(s32);

void ov40_02241CD8(s32 arg0, s32 arg1, s32 arg2);

void ov40_02241CD8(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 == 0) {
        switch (arg0) {
        case 0:
            ov40_02230944(arg2);
            ov40_02241BDC(arg2);
            ov40_0222BF80(arg2, 0x12);
            return;
        case 1:
            ov40_02230944(arg2);
            ov40_0222BF80(arg2, 0x11);
            break;
        }
    }
}
