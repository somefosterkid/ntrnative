#include "global.h"

#include "error_handling.h"

extern const u16 _020F6280[3];

int sub_02017FAC(u32 param0);

int sub_02017FAC(u32 param0) {
    int v0;

    for (v0 = 0; v0 < (3 - 1); v0++) {
        if (param0 < _020F6280[v0 + 1]) {
            return v0;
        }
    }

    return v0;
}

int sub_02017FCC(u32 param0);

int sub_02017FCC(u32 param0) {
    int v0 = sub_02017FAC(param0);
    return param0 - _020F6280[v0];
}

int sub_02017FE4(int baseValue, int modifier);

int sub_02017FE4(int baseValue, int modifier) {
    GF_ASSERT(baseValue < 3);
    return _020F6280[baseValue] + modifier;
}
