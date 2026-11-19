#include "global.h"
struct UnkStruct_ov10_0221FB28_t;

#include "bg_window.h"
#include "filesystem.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_05.h"
#include "palette.h"
#include "pm_string.h"
#include "overlay_05_unk_struct_ov10_0221_fb28_t.h"
#include "battle/trainer_intro_data.h"

void ov05_0221CFF0(void *arg0);

void ov05_0221CFF0(void *arg0) {
    if (*(s32 *)((u8 *)arg0 + 0xB94) != 0) {
        RemoveWindow(arg0 + 0xB88);
    }
    if (*(s32 *)((u8 *)arg0 + 0xBA4) != 0) {
        RemoveWindow(arg0 + 0xB98);
    }
}

void ov05_0221D020(struct UnkStruct_ov10_0221FB28_t *arg0);

void ov05_0221D020(UnkStruct_ov10_0221FB28_t *arg0) {
    MessageFormat *messageFormat;
    MsgData *msgData;
    String *string;

    string = arg0->unk_BB4;
    if (string != NULL) {
        String_Delete(string);
    }
    messageFormat = *((MessageFormat * *)&arg0->unk_BB0);
    if (messageFormat != NULL) {
        MessageFormat_Delete(messageFormat);
    }
    msgData = *((MsgData * *)&arg0->unk_BAC);
    if (msgData != NULL) {
        DestroyMsgData(msgData);
    }
}

void ov05_0221D054(struct UnkStruct_ov10_0221FB28_t *arg0);

void ov05_0221D054(UnkStruct_ov10_0221FB28_t *arg0) {
    GfGfx_EngineATogglePlanes(0x1FU, 0U);
    FreeBgTilemapBuffer(arg0->unk_0C, 3U);
    FreeBgTilemapBuffer(arg0->unk_0C, 2U);
    FreeBgTilemapBuffer(arg0->unk_0C, 1U);
    if (*((s32 *)&arg0->unk_BA8) == 0) {
        FreeBgTilemapBuffer(arg0->unk_0C, 0U);
    }
    Heap_Free(arg0->unk_0C);
}

void ov05_0221D094(UnkStruct_ov10_0221FB28 *arg0) {
    NARC *narc;

    narc = NARC_New(NARC_a_1_0_4, arg0->trainerIntroData->heapID);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 0, arg0->unk_0C, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->trainerIntroData->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 4, arg0->unk_0C, GF_BG_LYR_SUB_0_F, 0U, 0U, 0, arg0->trainerIntroData->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 2, arg0->unk_0C, GF_BG_LYR_SUB_1_F, 0U, 0U, 0, arg0->trainerIntroData->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 3, arg0->unk_0C, GF_BG_LYR_MAIN_3, 0U, 0U, 0, arg0->trainerIntroData->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 1, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0U, arg0->trainerIntroData->heapID);
    PaletteData_LoadPaletteSlotFromHardware(arg0->unk_08, PLTTBUF_MAIN_BG, 0U, 0xA0U);
    BG_SetMaskColor(1U, 0x18C6U);
    NARC_Delete(narc);
}
