#include "global.h"

extern const s16 ov07_022377F4[][3];
extern const s16 ov07_022377DC[][2];
extern const s16 ov07_022377F4[6][3];
extern const s16 ov07_022377DC[6][2];

s16 ov07_02234B5C(int battlerType, int coordIndex);

s16 ov07_02234B5C(int battlerType, int coordIndex) {
    if (coordIndex == 0) {
        return ov07_022377DC[battlerType][0];
    } else {
        return ov07_022377F4[battlerType][coordIndex];
    }
}
