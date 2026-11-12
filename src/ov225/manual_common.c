// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov319/manual_common.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/manual.h"
#include "app/manual_common.h"
#include "app/manual_data.h"
#include "app/manual_graphic.h"
#include "app/manual_touchbar.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/heap.h"
#include "gfl/msg.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "system/gf_font.h"
#include "system/printsys.h"

// What the manual's screens share, named after the ROM's "manual_common.c": the graphics, the font, the messages, the
// tables, the touch bar, and the picture of the upper screen, a 256x192 direct-color bitmap on BG 2

#define IMAGE_SIZE (256 * 192 * 2)

static void func_ov225_021b9ee8(ManualCommon *common);
static void func_ov225_021b9f20(ManualCommon *common);

// pokebw2's ManualCommon_Create
ManualCommon *func_ov225_021b9d18(GameData *gameData, HeapID heapId) {
    ManualCommon *common = GFL_HeapAllocate(heapId, sizeof(ManualCommon), TRUE, "manual_common.c", 75);

    common->gameData = gameData;
    common->heapId = heapId;
    common->graphic = func_ov225_021b9900(1, common->heapId);
    common->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, common->heapId);
    common->titleQueue = func_0201c440(common->heapId);
    common->queue = func_0201c440(common->heapId);
    common->touchBarQueue = func_0201c440(common->heapId);
    common->msgSystem = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_7, common->heapId);
    common->msgManual = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_TITLE_8, common->heapId);
    common->arc = GFL_ArcSysCreateFileHandle(ARCID_MANUAL, common->heapId);
    common->imageArc = GFL_ArcSysCreateFileHandle(ARCID_MANUAL_IMAGE, common->heapId);
    GFL_G2DIOLoadNCLR(ARCID_FONT, 5, PALTYPE_SUB_BG, 0, 2 * 0x20, 0x20, common->heapId);
    common->data = func_ov225_021b9b3c(common->arc, common->heapId);
    func_ov225_021b9ee8(common);
    common->image = MANUAL_IMAGE_NONE;
    func_ov225_021b9e80(common, func_ov225_021b9cdc(common->data));
    common->touchBar = func_ov225_021b9f24(common);
    return common;
}

// pokebw2's ManualCommon_Free
void func_ov225_021b9dec(ManualCommon *common) {
    func_ov225_021b9f8c(common->touchBar);
    func_ov225_021b9e80(common, MANUAL_IMAGE_NONE);
    func_ov225_021b9f20(common);
    func_ov225_021b9ba4(common->data);
    GFL_ArcToolFree(common->arc);
    GFL_ArcToolFree(common->imageArc);
    GFL_MsgDataFree(common->msgSystem);
    GFL_MsgDataFree(common->msgManual);
    func_0201c6ec(common->titleQueue);
    func_0201c4c0(common->titleQueue);
    func_0201c6ec(common->queue);
    func_0201c4c0(common->queue);
    func_0201c6ec(common->touchBarQueue);
    func_0201c4c0(common->touchBarQueue);
    GFL_FontFree(common->font);
    func_ov225_021b99a0(common->graphic);
    GFL_HeapFree(common);
}

// pokebw2's ManualCommon_Main
void func_ov225_021b9e60(ManualCommon *common) {
    func_0201c4e4(common->titleQueue);
    func_0201c4e4(common->queue);
    func_0201c4e4(common->touchBarQueue);
    func_ov225_021b99f4(common->graphic);
}

// pokebw2's ManualCommon_LoadImage
void func_ov225_021b9e80(ManualCommon *common, u16 image) {
    void *buffer;
    u32 size;

    if (common->image != image) {
        common->image = image;
        if (common->image == MANUAL_IMAGE_NONE) {
            buffer = GFL_HeapAllocate(common->heapId, IMAGE_SIZE, TRUE, "manual_common.c", 245);
            size = IMAGE_SIZE;
        } else {
            buffer = GFL_ArcToolReadHeapNewLZGetLen(common->imageArc, common->image, TRUE, common->heapId, &size);
        }
        DC_FlushRange(buffer, size);
        GX_LoadBG2Scr(buffer, 0, IMAGE_SIZE);
        GFL_HeapFree(buffer);
    }
}

// pokebw2's ManualCommon_InitBG
static void func_ov225_021b9ee8(ManualCommon *common) {
    G2_SetBG2ControlDCBmp(GX_BG_SCRSIZE_DCBMP_256x256, GX_BG_AREAOVER_XLU, GX_BG_BMPSCRBASE_0x00000);
    G2_SetBG2Priority(0);
    G2_BG0Mosaic(FALSE);
    GFL_BGSysSetBGEnabled(2, TRUE);
}

// pokebw2's ManualCommon_FreeBG
static void func_ov225_021b9f20(ManualCommon *common) {
}
