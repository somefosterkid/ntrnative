#include "global.h"
#include "overlay_07_battle_anim_common.h"

extern const Point2D ov07_02236468[2][6];

void ov07_02221F04(int isDoubles, int battlerType, s16 *px, s16 *py);

void ov07_02221F04(int isDoubles, int battlerType, s16 *px, s16 *py) {
    if (px != NULL) {
        *px = ov07_02236468[isDoubles][battlerType].x;
    }

    if (py != NULL) {
        *py = ov07_02236468[isDoubles][battlerType].y;
    }
}
