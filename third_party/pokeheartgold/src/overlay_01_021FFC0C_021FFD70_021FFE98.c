#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FFC0C.h"
#include "unk_020689C8.h"

s32 sub_02023E50(Billboard *, VecFx32 *);

void ov01_021FFE98(OverworldAnimManager *a0, FieldEffectAnimWork *work) {
    VecFx32 a1;

    if (sub_0205F0A8(work->args.mapObject, work->objectId, work->mapId) == 0) {
        ov01_021F1640(a0);
        return;
    }
    sub_02068DB8(a0, &a1);
    sub_02023E50(work->billboard, &a1);
}
