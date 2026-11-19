#include "global.h"
#include "overlay_14_box_funcs.h"

u32 ov14_021E7960(s16 x, s16 y);

struct Box2Area;

typedef struct Box2Area Box2Area;

struct Box2Area {
    u8 left;
    u8 right;
    u8 top;
    u8 bottom;
};

extern const Box2Area _021F7BBC;

u32 ov14_021E7960(s16 x, s16 y) {
    if (ov14_021E7940(x, y, &_021F7BBC) == 1) {
        if (x < 12) {
            x = 0;
        } else if (x >= 156) {
            x = 5;
        } else {
            x = (x - 12) / 24;
        }
        y = (y - 40) / 24;
        return x + y * 6;
    }
    return 0xff;
}
