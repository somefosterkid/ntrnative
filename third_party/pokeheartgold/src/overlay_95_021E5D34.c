#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
void ClearWindowTilemapAndCopyToVram(MapObject *mapObj);
void RemoveWindow(MapObject *param0);

void ov95_021E5D34(MapObject *mapObj);

void ov95_021E5D34(MapObject *mapObj) {
    ClearWindowTilemapAndCopyToVram(mapObj);
    RemoveWindow(mapObj);
}
