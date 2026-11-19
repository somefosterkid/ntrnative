#include "global.h"

#include "billboard.h"
#include "billboard_gfx_sequence.h"

struct MapObject;

typedef struct MapObject MapObject;

void sub_02023EA4(Billboard *billboard, u8 draw);

BOOL MapObject_TestFlagsBits(const MapObject *mapObj, u32 flag);

void ov01_021FA40C(MapObject *param0, Billboard *param1);

void ov01_021FA40C(MapObject *param0, Billboard *param1) {
    int v0 = 1;

    if (MapObject_TestFlagsBits(param0, (1 << 9)) == 1) {
        v0 = 0;
    }

    if (MapObject_TestFlagsBits(param0, (1 << 12)) == 1) {
        if (!MapObject_TestFlagsBits(param0, (1 << 13))) {
            v0 = 0;
        }
    }

    sub_02023EA4(param1, v0);
}
