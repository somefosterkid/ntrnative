#include "global.h"

#include "map_object.h"
#include "overlay_01_021F1348.h"
#include "overlay_01_021FF854.h"
#include "overworld_anim_manager.h"
#include "unk_020689C8.h"

void sub_02023E50(Billboard *billboard, const VecFx32 *pos);

void ov01_021FFBD4(OverworldAnimManager *manager, FieldEffectAnimWork *work) {
    VecFx32 pos;

    if (sub_0205F0F8(work->args.mapObject, work->spriteId, work->objectId, work->mapId) == 0) {
        ov01_021F1640(manager);
        return;
    }
    sub_02068DB8(manager, &pos);
    sub_02023E50(work->billboard, &pos);
}
