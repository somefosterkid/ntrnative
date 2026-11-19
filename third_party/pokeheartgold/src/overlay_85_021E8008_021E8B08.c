#include "global.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_85.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "overlay_85_ov109_021D5140.h"

s32 ov85_021E8E00(void *);

s32 ov85_021E8E38(void *);

s32 ov85_021E9288(void *);

s32 ov85_021E9FD0(void);

s32 ov85_021E9FEC(void);

s32 ov85_021E8B08(OverlayManager *man, int *state) {
    OverlayManager *overlayManager;
    s32 v1;
    UnkStruct_ov109_021D5140 *v0;

    overlayManager = man;
    v0 = OverlayManager_GetData(man);
    SysTask_Destroy(v0->unk_30);
    SpriteTransfer_DeleteCharTransferTask(v0->unk_200[2][0]);
    SpriteTransfer_DeletePlttTransferTask(v0->unk_200[2][1]);
    v1 = 0;
    do {
        Destroy2DGfxResObjMan(v0->unk_1F0[v1]);
        v1 += 1;
    } while (v1 < 4);
    SpriteList_Delete(v0->unk_60);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov85_021E9288(v0);
    ov85_021E8E38(v0->unk_14);
    DestroyMsgData(v0->unk_38);
    MessageFormat_Delete(v0->unk_34);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    Main_SetVBlankIntrCB(NULL, NULL);
    *(s32 *)((u8 *)(v0->unk_0C) + 0x10) = v0->unk_08;
    *(s32 *)((u8 *)(v0->unk_0C) + 8) = ov85_021E9FD0();
    *(s32 *)((u8 *)(v0->unk_0C) + 0xC) = ov85_021E9FEC();
    ov85_021E8E00(v0);
    OverlayManager_FreeData(overlayManager);
    Heap_Destroy(HEAP_ID_102);
    return 1;
}
