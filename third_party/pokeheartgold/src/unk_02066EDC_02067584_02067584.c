#include "global.h"

#include "field_system.h"
#include "field_types_def.h"
#include "unk_02066EDC.h"

BOOL FieldSystem_MapIsBattleTowerMultiPartnerSelectRoom(FieldSystem *fieldSystem) {
    if (fieldSystem->location->mapId == 0x10f) {
        return 1;
    }
    return 0;
}
