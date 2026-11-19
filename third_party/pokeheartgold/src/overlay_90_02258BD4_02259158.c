#include "global.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "unk_0200B150.h"

void ov90_02259158(SpriteList **arg0);

void ov90_02259158(SpriteList **arg0) {
    SpriteList_Delete(*arg0);
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    OamManager_Free();
}
