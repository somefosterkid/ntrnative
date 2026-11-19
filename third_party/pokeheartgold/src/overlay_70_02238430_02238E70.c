#include "global.h"
struct GTSApplicationState;

#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_70_gtsapplication_state.h"

void ov70_022387AC(void *);

s32 ov70_022387F8(void);

s32 ov70_022388D0(void);

void ov70_02238900(GTSApplicationState *);

s32 ov70_02238B80(GTSApplicationState *);

void ov70_02238E70(void *arg0);

void ov70_02238E70(void *arg) {
    ov70_022387F8();
    ov70_022388D0();
    ov70_02238900(arg);
    ov70_02238B80(arg);
    Main_SetVBlankIntrCB(ov70_022387AC, arg);
}

s32 ov70_02241308(GTSApplicationState *arg0);

void ov70_02238E98(struct GTSApplicationState *arg0);

void ov70_02238E98(GTSApplicationState *arg0) {
    s32 v0;

    ov70_02241308(arg0);
    SpriteTransfer_DeleteCharTransferTask(arg0->spriteResource[0][0]);
    SpriteTransfer_DeleteCharTransferTask(arg0->spriteResource[1][0]);
    SpriteTransfer_DeletePlttTransferTask(arg0->spriteResource[0][1]);
    SpriteTransfer_DeletePlttTransferTask(arg0->spriteResource[1][1]);
    v0 = 0;
    do {
        Destroy2DGfxResObjMan(arg0->spriteResourceCollection[v0]);
        v0 += 1;
    } while (v0 < 4);
    SpriteList_Delete(arg0->spriteList);
    arg0->spriteList = NULL;
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
}
