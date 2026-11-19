#include "global.h"

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"

void ov81_02242B38(void **param_1);

void ov81_02242B38(void **param_1) {
    u32 uVar1;

    uVar1 = 0;
    do {
        SpriteTransfer_DeleteCharTransferTask((SpriteResource *)param_1[uVar1 * 4 + 0x4f]);
        SpriteTransfer_DeletePlttTransferTask((SpriteResource *)param_1[uVar1 * 4 + 0x50]);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 6);
    uVar1 = 0;
    do {
        Destroy2DGfxResObjMan((GF_2DGfxResMan *)param_1[uVar1 + 0x4b]);
        uVar1 = uVar1 + 1 & 0xff;
    } while (uVar1 < 4);
    SpriteList_Delete((SpriteList *)*param_1);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    return;
}
