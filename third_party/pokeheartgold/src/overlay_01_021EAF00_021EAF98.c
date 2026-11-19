#include "global.h"

#include "application/view_photo.h"

#include "overlay_01_021EAF00.h"
#include "poke_overlay.h"
#include "overlay_manager.h"

FS_EXTERN_OVERLAY(OVY_19);

void ov01_021EAF98(FieldSystem *fieldSystem) {
    HandleLoadOverlay(FS_OVERLAY_ID(OVY_19), OVY_LOAD_ASYNC);
    fieldSystem->unk_D8 = FieldSystem_CreateViewPhotoTask(fieldSystem);
}

void ov01_021EAFB4(FieldSystem *fieldSystem) {
    FieldSystem_DestroyViewPhotoTask(fieldSystem);
    fieldSystem->unk_D8 = NULL;
    UnloadOverlayByID(FS_OVERLAY_ID(OVY_19));
}

BOOL ov01_021EAFD0(FieldSystem *fieldSystem) {
    return TRUE;
}
