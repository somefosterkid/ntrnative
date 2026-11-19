#include "global.h"

#include "constants/heap.h"

#include "field/dynamic_terrain_height.h"
#include "field/map_prop_animation.h"
#include "field/model_attributes.h"

#include "bag_cursor.h"
#include "bg_window.h"
#include "camera.h"
#include "field_types_def.h"
#include "map_matrix.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "save.h"
#include "sys_task.h"
#include "overlay_02_hmcut_in.h"
#include "unk_0200A090.h"
#include "sprite.h"

struct SpriteList;

struct SpriteResource;

BOOL SpriteList_Delete(SpriteList *collection);

BOOL SpriteList_DeleteAllSprites(SpriteList *collection);

void SpriteTransfer_DeleteCharTransferTask(SpriteResource *resource);

void SpriteTransfer_DeletePlttTransferTask(SpriteResource *resource);

void ov02_0224A288(HMCutIn *cutIn);

void ov02_0224A288(HMCutIn *cutIn) {
    int i;

    for (i = 0; i < 4; i++) {
        if (cutIn->charSource[i] != NULL) {
            SpriteTransfer_DeleteCharTransferTask(cutIn->charSource[i]);
        }
    }

    for (i = 0; i < 3; i++) {
        if (cutIn->paletteSource[i] != NULL) {
            SpriteTransfer_DeletePlttTransferTask(cutIn->paletteSource[i]);
        }
    }

    for (i = 0; i < 4; i++) {
        if (cutIn->cellSource[i] != NULL) {
            sub_0200A740(cutIn->cellSource[i]);
        }
    }

    for (i = 0; i < 2; i++) {
        if (cutIn->animSource[i] != NULL) {
            sub_0200A740(cutIn->animSource[i]);
        }
    }

    Destroy2DGfxResObjMan(cutIn->charLocation);
    Destroy2DGfxResObjMan(cutIn->paletteLocation);
    Destroy2DGfxResObjMan(cutIn->cellLocation);
    Destroy2DGfxResObjMan(cutIn->animLocation);
    SpriteList_DeleteAllSprites(cutIn->spriteList);
    SpriteList_Delete(cutIn->spriteList);
}
