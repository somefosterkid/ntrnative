#include "global.h"

#include "font.h"
#include "heap.h"
#include "menu_input_state.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "sys_task_api.h"
#include "system.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_020210A0.h"
#include "unk_02037C94.h"
#include "unk_0205A44C.h"
#include "overlay_73_unk_struct_020961_e8_t.h"
struct UnkStruct_020961E8_t;

s32 ov73_021E5ED4(struct UnkStruct_020961E8_t *);

s32 ov73_021E5F0C(BgConfig *);

s32 ov73_021E6048(struct UnkStruct_020961E8_t *);

s32 ov73_021E6400(struct UnkStruct_020961E8_t *);

s32 sub_0205AD24(void *);

s32 ov73_021E5BAC(OverlayManager *man, s32 *state);

s32 ov73_021E5BAC(OverlayManager *man, s32 *state) {
    OverlayManager *overlayManager;
    s32 v1;
    UnkStruct_020961E8 *v0;

    overlayManager = man;
    v0 = OverlayManager_GetData(man);
    SysTask_Destroy(v0->unk_20);
    SpriteTransfer_DeleteCharTransferTask(v0->unk_1F0[2][0]);
    SpriteTransfer_DeletePlttTransferTask(v0->unk_1F0[2][1]);
    v1 = 0;
    do {
        Destroy2DGfxResObjMan(v0->unk_1E0[v1]);
        v1 += 1;
    } while (v1 < 4);
    SpriteList_Delete(v0->unk_50);
    OamManager_Free();
    ObjCharTransfer_Destroy();
    ObjPlttTransfer_Destroy();
    ov73_021E6400(v0);
    ov73_021E6048(v0);
    ov73_021E5F0C(v0->unk_00);
    sub_02038C1C(2);
    sub_02037FF0();
    sub_0205AD24(*(void **)((u8 *)(v0->unk_08) + 4));
    DisableTouchPad();
    FontID_Release(FONT_UNK_4);
    DestroyMsgData(v0->unk_28);
    MessageFormat_Delete(v0->unk_24);
    MenuInputStateMgr_SetState(*(MenuInputStateMgr **)((u8 *)(v0->unk_08) + 0x10), v0->unk_314);
    ov73_021E5ED4(v0);
    OverlayManager_FreeData(overlayManager);
    reg_GX_POWCNT |= REG_GX_POWCNT_DSEL_MASK;
    sub_0205A904(0);
    Main_SetVBlankIntrCB(NULL, NULL);
    Heap_Destroy(HEAP_ID_50);
    return 1;
}
