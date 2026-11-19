#include "global.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_32.h"
#include "overlay_32_name_picker_data.h"

void ov32_0225D6C4(BgConfig *bgConfig) {
    FreeBgTilemapBuffer(bgConfig, 6U);
    FreeBgTilemapBuffer(bgConfig, 5U);
    FreeBgTilemapBuffer(bgConfig, 4U);
}

void ov32_0225D6E0(NamePickerData *data, NARC *narc) {
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 2, data->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 1, data->bgConfig, GF_BG_LYR_SUB_1, 0U, 0U, 1, HEAP_ID_8);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 0, data->bgConfig, GF_BG_LYR_SUB_2, 0U, 0U, 1, HEAP_ID_8);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 3, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0U, HEAP_ID_8);
}

void ov32_0225D748(NamePickerData *data) {
    data->msgData = NewMsgDataFromNarc(MSGDATA_LOAD_DIRECT, NARC_msgdata_msg, 0xC4, HEAP_ID_8);
    data->msgFormat = MessageFormat_New(HEAP_ID_8);
}

void ov32_0225D76C(NamePickerData *data) {
    MessageFormat_Delete(data->msgFormat);
    DestroyMsgData(data->msgData);
}
