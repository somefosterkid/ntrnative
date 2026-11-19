#include "global.h"

#include "font.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_manager.h"
#include "render_text.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_020210A0.h"
#include "signature_app.h"

s32 ov52_021E8568(void *);

s32 ov52_021E85A0(BgConfig *);

s32 ov52_021E8B94(void *);

s32 ov52_021E9300(u8 *, u8 *);

s32 TrainerCardSignature_Exit(OverlayManager *manager, s32 *state);

s32 TrainerCardSignature_Exit(OverlayManager *manager, s32 *state) {
    OverlayManager *man;
    s32 v1;
    SignatureApp *v0;

    man = manager;
    v0 = OverlayManager_GetData(manager);
    ov52_021E9300((u8 *)v0->savedSignature, v0->signatureWindow.pixelBuffer);
    Main_SetVBlankIntrCB(NULL, NULL);
    SpriteTransfer_DeleteCharTransferTask(v0->spriteResources[0][0]);
    SpriteTransfer_DeleteCharTransferTask(v0->spriteResources[2][0]);
    SpriteTransfer_DeletePlttTransferTask(v0->spriteResources[0][1]);
    SpriteTransfer_DeletePlttTransferTask(v0->spriteResources[2][1]);
    v1 = 0;
    do {
        Destroy2DGfxResObjMan(v0->spriteResourceCollection[v1]);
        v1 += 1;
    } while (v1 < 4);
    SpriteList_Delete(v0->spriteList);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov52_021E8B94(v0);
    FontID_Release(FONT_SUBSCREEN);
    ov52_021E85A0(v0->bgConfig);
    DisableTouchPad();
    TextFlags_SetCanTouchSpeedUpPrint(0);
    TextFlags_SetCanABSpeedUpPrint(0);
    DestroyMsgData(((MsgData *)v0->loader));
    MessageFormat_Delete(((MessageFormat *)v0->template));
    ov52_021E8568(v0);
    OverlayManager_FreeData(man);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_39);
    return 1;
}
