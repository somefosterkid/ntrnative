#include "global.h"

#include "overlay_01_021F4704.h"

Bdhc *ov01_021F65D0(MapLoadManager *mapLoadManager, u8 mapSlot) {
    return mapLoadManager->maps[mapSlot]->bdhc;
}
