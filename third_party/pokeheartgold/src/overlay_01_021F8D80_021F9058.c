#include "global.h"

struct MapObject;
typedef struct MapObject MapObject;
void sub_0205F9A0(MapObject *mapObj, const VecFx32 *vec);
void ov01_021F9140(MapObject *param0);

void ov01_021F9058(MapObject *param0);

void ov01_021F9058(MapObject *param0) {
    VecFx32 v0;

    ov01_021F9140(param0);

    v0.x = 0;
    v0.y = ((fx32)(((0) > 0) ? ((0) * (1 << 12) + 0.5f) : ((0) * (1 << 12) - 0.5f)));
    v0.z = 0;

    sub_0205F9A0(param0, &v0);
}
