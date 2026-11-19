#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "font.h"
#include "geonet_globe.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "msgdata.h"
#include "options.h"
#include "pm_string.h"
#include "render_window.h"
#include "text.h"

extern const WindowTemplate ov69_021E764C;

extern const WindowTemplate ov69_021E7654;

extern const WindowTemplate ov69_021E765C;

extern const WindowTemplate ov69_021E766C;

#if defined(SDK_PORT) || defined(NONMATCHING)
// asm/overlay_69.s keeps the DS copy (.rodata 0x021E764C..0x021E7673, interleaved with asm-only tables the cut cannot split),
// which the PC link does not see: the PC build defines the four WindowTemplates from the DS bytes here.
const WindowTemplate ov69_021E764C = { .bgId = 6, .left = 2, .top = 1, .width = 27, .height = 6, .palette = 4, .baseTile = 0xB3 };
const WindowTemplate ov69_021E7654 = { .bgId = 2, .left = 1, .top = 21, .width = 6, .height = 2, .palette = 15, .baseTile = 0x1C1 };
const WindowTemplate ov69_021E765C = { .bgId = 2, .left = 25, .top = 21, .width = 6, .height = 2, .palette = 15, .baseTile = 0x1CD };
const WindowTemplate ov69_021E766C = { .bgId = 6, .left = 2, .top = 19, .width = 27, .height = 4, .palette = 4, .baseTile = 0x16D };
#endif

extern BgTemplate ov69_021E76B0;

extern BgTemplate ov69_021E76CC;

void ov69_021E64CC(GeonetGlobe *globe, NARC *narc);

void ov69_021E64CC(GeonetGlobe *globe, NARC *narc) {
    u8 sp14[4];
    u8 sp10[4];
    String *temp_r5;

    InitBgFromTemplate(globe->bgConfig, 6U, &ov69_021E76B0, 0U);
    BgClearTilemapBufferAndCommit(globe->bgConfig, 6U);
    InitBgFromTemplate(globe->bgConfig, 7U, &ov69_021E76CC, 0U);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, globe->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, globe->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_0_OFFSET, 0x80U, globe->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 7, globe->bgConfig, GF_BG_LYR_SUB_3, 0U, 0U, 0, globe->heapID);
    LoadUserFrameGfx2(globe->bgConfig, GF_BG_LYR_SUB_2, 0x1E2U, 6U, (s32)Options_GetFrame(globe->options), globe->heapID);
    LoadUserFrameGfx1(globe->bgConfig, GF_BG_LYR_SUB_2, 0x1D9U, 7U, 0U, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_4_OFFSET, globe->heapID);
    BG_ClearCharDataRange(6U, 0x20U, 0U, globe->heapID);
    BG_SetMaskColor(6U, 0x4753U);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], &ov69_021E766C);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 0xFU, 0U, 0U, 0xD8U, 0x20U);
    DrawFrameAndWindow2(&globe->windows[GEONET_GLOBE_WINDOW_MESSAGE], 0, 0x1E2U, 6U);
    globe->messageState = GEONET_GLOBE_MESSAGE_STATE_START;
    InitBgFromTemplate(globe->bgConfig, 2U, &ov69_021E76B0, 0U);
    BgClearTilemapBufferAndCommit(globe->bgConfig, 2U);
    InitBgFromTemplate(globe->bgConfig, 3U, &ov69_021E76CC, 0U);
    GfGfxLoader_LoadCharDataFromOpenNarc(narc, 5, globe->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, globe->heapID);
    GfGfxLoader_GXLoadPalFromOpenNarc(narc, 6, GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_0_OFFSET, 0x80U, globe->heapID);
    GfGfxLoader_LoadScrnDataFromOpenNarc(narc, 7, globe->bgConfig, GF_BG_LYR_MAIN_3, 0U, 0U, 0, globe->heapID);
    LoadUserFrameGfx1(globe->bgConfig, GF_BG_LYR_SUB_1_F, 0x1D9U, 7U, 0U, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_4_OFFSET, globe->heapID);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_15_OFFSET, globe->heapID);
    BG_ClearCharDataRange(2U, 0x20U, 0U, globe->heapID);
    BG_SetMaskColor(2U, 0U);
    temp_r5 = String_New(0x10U, globe->heapID);
    FontID_Alloc(FONT_UNK_4, globe->heapID);
    *(s16 *)((u8 *)((s32 *)sp10) + 6) = 0x7FFF;
    *(s16 *)((u8 *)((s32 *)sp10) + 4) = 0x1CE7;
    *(s16 *)((u8 *)((s32 *)sp10) + 2) = 0x4E72;
    *(s16 *)((s32 *)sp10) = 0x7FFF;
    BG_LoadPlttData(2U, &sp14[2], 2U, 0x1E2U);
    BG_LoadPlttData(2U, (s32 *)sp14, 2U, 0x1E4U);
    BG_LoadPlttData(2U, &sp10[2], 2U, 0x1E6U);
    BG_LoadPlttData(2U, (s32 *)sp10, 2U, 0x1FEU);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], &ov69_021E765C);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], 0xFU, 0U, 0U, 0xD8U, 0x20U);
    ReadMsgDataIntoString(globe->messageLoader, 0x10, temp_r5);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_EXIT_BUTTON], FONT_UNK_4, temp_r5, 0U, 0U, 0xFFU, NULL);
    AddWindow(globe->bgConfig, &globe->windows[GEONET_GLOBE_WINDOW_LOCATION], &ov69_021E7654);
    FillWindowPixelRect(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], 0xFU, 0U, 0U, 0xD8U, 0x20U);
    ReadMsgDataIntoString(globe->messageLoader, 0xD, temp_r5);
    AddTextPrinterParameterized(&globe->windows[GEONET_GLOBE_WINDOW_LOCATION], FONT_UNK_4, temp_r5, 0U, 0U, 0xFFU, NULL);
    String_Delete(temp_r5);
    FontID_Release(FONT_UNK_4);
}
