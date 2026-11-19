#include "global.h"
#include "overlay_28.h"
#include "sprite.h"
#include "sprite_transfer.h"

void ov28_0225DB54(DowsingMchnApp *app) {
    Sprite_Delete(app->sprites[0]);
    Sprite_Delete(app->sprites[1]);
    Sprite_Delete(app->sprites[2]);
    SpriteTransfer_DeleteCharTransferTask(app->buttonResObjs[0]);
    SpriteTransfer_DeletePlttTransferTask(app->buttonResObjs[1]);
}
