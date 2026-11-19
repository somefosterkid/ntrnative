#include "global.h"
#include "overlay_28.h"
#include "sprite.h"
#include "sprite_transfer.h"

void ov28_0225DBFC(DowsingMchnApp *app) {
    u32 v0;

    v0 = 0;
    do {
        Sprite_Delete(app->sprites[4 + v0]);
        v0 += 1;
    } while (v0 < 4U);
    SpriteTransfer_DeleteCharTransferTask(app->needleResObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(app->needleResObjs[1]);
}
