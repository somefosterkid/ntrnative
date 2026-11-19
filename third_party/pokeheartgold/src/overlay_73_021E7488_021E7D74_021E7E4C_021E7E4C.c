#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_manager.h"
#include "sound_02004A44.h"
#include "system.h"
#include "unk_02034B0C.h"
#include "unk_020915B0.h"
#include "ptr_align.h"
#include "overlay_73_wifi_battle_tower_app_state.h"

typedef enum HeapID HeapID;


typedef struct {
    u32 w[4];
} UnkStruct_ov73_021EA68C;

extern UnkStruct_ov73_021EA68C ov73_021EA68C;

extern void *_021EA940;

void ov73_021E8148(WifiBattleTowerAppState *appState, OverlayManager *appMan);

void ov73_021E8454(void *arg);

s32 ov73_021E7E4C(void *ovy, s32 *state);

s32 ov73_021E7E4C(void *ovy, s32 *state) {
    WifiBattleTowerAppState *work;
    UnkStruct_ov73_021EA68C modes;
    void *heapBuf;

    switch (*state) {
    case 0:
        Main_SetVBlankIntrCB(0, 0);
        HBlankInterruptDisable();
        GfGfx_DisableEngineAPlanes();
        GfGfx_DisableEngineBPlanes();
        reg_GX_DISPCNT = reg_GX_DISPCNT & 0xFFFFE0FF;
        reg_GXS_DB_DISPCNT = reg_GXS_DB_DISPCNT & 0xFFFFE0FF;
        Heap_Create((HeapID)(3), (HeapID)(0x96), 0x50000);
        work = OverlayManager_CreateAndGetData((struct OverlayManager *)((ovy)), sizeof(WifiBattleTowerAppState), (HeapID)(0x96));
        memset(work, 0, sizeof(WifiBattleTowerAppState));
        work->bgConfig = BgConfig_Alloc((enum HeapID)(0x96));
        _021EA940 = work;
        modes = ov73_021EA68C;
        SetBothScreensModesAndDisable((const struct GraphicsModes *)(&modes));
        work->stringTemplate = MessageFormat_New_Custom(0xb, 0x20, (HeapID)(0x96));
        work->msgLoader1 = NewMsgDataFromNarc((MsgDataLoadType)(0), (NarcId)(0x1b), 0x31f, (HeapID)(0x96));
        work->msgLoader2 = NewMsgDataFromNarc((MsgDataLoadType)(0), (NarcId)(0x1b), 0x30a, (HeapID)(0x96));
        work->msgLoader3 = NewMsgDataFromNarc((MsgDataLoadType)(0), (NarcId)(0x1b), 0x320, (HeapID)(0x96));
        SetKeyRepeatTimers(4, 8);
        ov73_021E8148(work, ovy);
        ov73_021E8454(work);
        Sound_SetSceneAndPlayBGM(0x34, 0, 0);
        heapBuf = Heap_Alloc((HeapID)(0x96), 0x20020);
        work->dwcHeapBuffer = heapBuf;
        work->dwcHeap = NNS_FndCreateExpHeapEx(PTR_ALIGN_UP(heapBuf, 0x20), 0x20000, 0);
        Sound_SetSceneAndPlayBGM(0xb, 0x47d, 1);
        reg_GX_POWCNT = reg_GX_POWCNT & 0xFFFF7FFF;
        *state = 1;
        break;
    case 1:
        LoadDwcOverlay();
        LoadOVY38();
        sub_02034D8C();
        *state = 0;
        return 1;
    }
    return 0;
}
