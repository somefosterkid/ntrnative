#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01_021F6830.h"
#include "player_avatar.h"
#include "save_local_field_data.h"

void ov01_021F4404(FieldSystem *arg0);

void ov01_021F4404(FieldSystem *fieldSystem) {
    FieldSystem_SyncMapObjectsToSave(fieldSystem);
    ov01_021F6830(fieldSystem, 4, 0);
    fieldSystem->location->x = (s32)PlayerAvatar_GetXCoord(fieldSystem->playerAvatar);
    fieldSystem->location->y = (s32)PlayerAvatar_GetZCoord(fieldSystem->playerAvatar);
    fieldSystem->location->warpId = -1;
    fieldSystem->location->direction = (s32)PlayerAvatar_GetFacingDirection(fieldSystem->playerAvatar);
}
