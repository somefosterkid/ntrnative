#include "global.h"

#include "error_handling.h"
#include "field_system.h"
#include "field_types_def.h"
#include "overlay_01.h"

void ov01_021F4404(FieldSystem *arg0);

void ov01_021F4440(FieldSystem *fieldSystem) {
    s32 iVar1;

    if (fieldSystem == NULL) {
        GF_AssertFail();
        return;
    }
    iVar1 = fieldSystem->location->mapId;
    if (((iVar1 != 2) && (iVar1 != 4)) && (iVar1 != 5)) {
        ov01_021F4404(fieldSystem);
    }
    return;
}
