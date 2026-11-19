#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "obj_char_transfer.h"
#include "obj_pltt_transfer.h"
#include "overlay_manager.h"
#include "sprite.h"
#include "sprite_transfer.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_0200B150.h"
#include "unk_020210A0.h"
#include "unk_02032844.h"
#include "unk_02034B0C.h"
#include "unk_02037C94.h"
#include "unk_0205A44C.h"
#include "bg_window.h"
#include "vram_transfer_manager.h"

void ov37_021E5F20(u32 *);

void ov37_021E5F5C(u32);

void ov37_021E6540(u32 *);

void sub_0205AD24(u32);

BOOL ov37_021E5B94(OverlayManager *man, s32 *state);

void ov37_021E5CC8(void *param0);

BOOL ov37_021E5B94(OverlayManager *man, s32 *state) {
    u16 uVar1;
    u32 *puVar2;
    u32 *puVar3;
    s32 iVar4;

    puVar2 = OverlayManager_GetData(man);
    puVar3 = OverlayManager_GetArgs(man);
    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        SpriteTransfer_DeleteCharTransferTask((SpriteResource *)puVar2[0x5c]);
        SpriteTransfer_DeleteCharTransferTask((SpriteResource *)puVar2[0x60]);
        SpriteTransfer_DeletePlttTransferTask((SpriteResource *)puVar2[0x5d]);
        SpriteTransfer_DeletePlttTransferTask((SpriteResource *)puVar2[0x61]);
        iVar4 = 0;
        puVar3 = puVar2;
        do {
            Destroy2DGfxResObjMan((GF_2DGfxResMan *)puVar3[0x58]);
            iVar4 = iVar4 + 1;
            puVar3 = puVar3 + 1;
        } while (iVar4 < 4);
        SpriteList_Delete((SpriteList *)puVar2[0xd]);
        OamManager_Free();
        ObjCharTransfer_Destroy();
        ObjPlttTransfer_Destroy();
        ov37_021E6540(puVar2);
        ov37_021E5F5C(*puVar2);
        DisableTouchPad();
        DestroyMsgData((MsgData *)puVar2[4]);
        MessageFormat_Delete((MessageFormat *)puVar2[3]);
        *state = *state + 1;
        break;
    case 1:
        sub_02038C1C(1);
        sub_02037FF0();
        sub_0205AD24(*puVar3);
        reg_GX_POWCNT = reg_GX_POWCNT | 0x8000;
        sub_0205A904(0);
        sub_020356EC(0);
        *state = *state + 1;
        break;
    case 2:
        if (puVar2[0x2500] != 0) {
            uVar1 = sub_02033250();
            if (uVar1 == 1) {
                *state = *state + 1;
            }
        } else {
            *state = *state + 1;
        }
        break;
    case 3:
        sub_02038C1C(2);
        ov37_021E5F20(puVar2);
        Heap_Free((void *)puVar2[2]);
        OverlayManager_FreeData(man);
        Main_SetVBlankIntrCB((GFIntrCB)0x0, (void *)0x0);
        Heap_Destroy(HEAP_ID_39);
        sub_02038C1C(2);
        return 1;
    }
    return 0;
}

void ov37_021E5CC8(void *param0) {
    GF_RunVramTransferTasks();
    OamManager_ApplyAndResetBuffers();
    DoScheduledBgGpuUpdates((BgConfig *)param0);

    OS_SetIrqCheckFlag(OS_IE_V_BLANK);
}
