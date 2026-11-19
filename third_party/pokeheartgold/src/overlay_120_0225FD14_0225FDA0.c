#include "global.h"

#include "bg_window.h"
#include "error_handling.h"
#include "gf_gfx_loader.h"
#include "heap.h"
#include "overlay_120.h"
#include "sys_task_api.h"

static void ov120_0225FDA0(SysTask *task, void *data) {
    Ov120BgLoader *loader = data;
    EncounterEffect *effect;
    NNSG2dCharacterData *charData;
    u16 state;

    state = loader->vblankState;
    effect = loader->effect;
    switch (state) {
    case 0:
        charData = loader->charData1;
        BG_LoadCharTilesData(effect->fieldSystem->bgConfig, 1, charData->pRawData, charData->szByte, 0);
        loader->vblankState++;
        return;
    case 1:
        charData = loader->charData2;
        BG_LoadCharTilesData(effect->fieldSystem->bgConfig, 3, charData->pRawData, charData->szByte, 0);
        loader->vblankState++;
        return;
    case 2:
        loader->vblankTask = NULL;
        SysTask_Destroy(task);
        return;
    default:
        GF_AssertFail();
        return;
    }
}

static void ov120_0225FE08(SysTask *task, void *data) {
    Ov120BgLoader *loader = data;
    EncounterEffect *effect;
    u16 state;

    effect = loader->effect;
    if (effect->narc == NULL) {
        GF_AssertFail();
    }
    state = loader->state;
    switch (state) {
    case 0:
        GfGfxLoader_GXLoadPalFromOpenNarc(effect->narc, 0xA0, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x20, HEAP_ID_FIELD1);
        loader->ncgr1 = ov120_0225FD14(effect->narc, 0xA1, &loader->charData1, HEAP_ID_FIELD1);
        loader->ncgr2 = ov120_0225FD14(effect->narc, 0xA4, &loader->charData2, HEAP_ID_FIELD1);
        loader->vblankState = 0;
        loader->vblankTask = SysTask_CreateOnVBlankQueue(ov120_0225FDA0, loader, 0);
        loader->state++;
        return;
    case 1:
        if (loader->vblankTask == NULL) {
            ov120_0225FD2C(effect->fieldSystem->bgConfig, effect->narc, 0xA2, 1);
            ov120_0225FD2C(effect->fieldSystem->bgConfig, effect->narc, 0xA5, 3);
            loader->state++;
            return;
        }
        return;
    case 2:
        Heap_Free(loader->ncgr1);
        Heap_Free(loader->ncgr2);
        loader->task = NULL;
        SysTask_Destroy(task);
        return;
    default:
        GF_AssertFail();
        break;
    }
}

void ov120_0225FECC(Ov120BgLoader *loader, EncounterEffect *effect) {
    loader->effect = effect;
    loader->state = 0;
    loader->task = SysTask_CreateOnMainQueue(ov120_0225FE08, loader, 1);
}

BOOL ov120_0225FEE8(Ov120BgLoader *loader) {
    if (loader == NULL) {
        GF_AssertFail();
    }
    if (loader->task == NULL) {
        return TRUE;
    }
    return FALSE;
}
