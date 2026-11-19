#include "global.h"

void ov40_0222BF80(u32, u32);

void ov40_02230944(u32);

void ov40_02241C10(u32 param_1, s32 param_2, u32 param_3);

void ov40_02241C10(u32 param_1, s32 param_2, u32 param_3) {
    if (param_2 == 0) {
        switch (param_1) {
        case 0:
            ov40_02230944(param_3);
            ov40_0222BF80(param_3, 9);
            return;
        case 1:
            ov40_02230944(param_3);
            ov40_0222BF80(param_3, 10);
            return;
        case 2:
            ov40_02230944(param_3);
            ov40_0222BF80(param_3, 0xc);
            return;
        case 3:
            ov40_02230944(param_3);
            ov40_0222BF80(param_3, 0xb);
        }
    }
    return;
}
