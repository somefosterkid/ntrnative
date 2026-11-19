#include "global.h"
#include "billboard.h"

struct MapObject;

typedef struct MapObject MapObject;

void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov01_021F93AC(const MapObject *param0, VecFx32 *param1);

BOOL ov01_021FA3E8(MapObject *param0, Billboard *param1);

BOOL ov01_021FA3E8(MapObject *param0, Billboard *param1) {
    VecFx32 v1;

    ov01_021F93AC(param0, &v1);

    v1.z += (((fx32)0x0000000000001000L) * 6);

    sub_02023E50(param1, &v1);

    return 0;
}
