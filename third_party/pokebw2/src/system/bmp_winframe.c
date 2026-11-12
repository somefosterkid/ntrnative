#include "types.h"
#include "constants/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmpwin.h"
#include "system/bmp_winframe.h"

// The system's window frames: loading them from the window frame archive and drawing them around windows. The file's
// name is a guess: the ROM has no string for it

static u32 GetSysMsgBoxCharDatID(u8 type);
static void BmpWin_DrawFrameCore(u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 frameChar);

// The window frame archive's files for each frame type
static const u8 SYSTEM_MSGBOX_DATIDS_CHAR[] = { 8, 10 };
static const u8 SYSTEM_MSGBOX_DATIDS_PALETTE[] = { 3, 0 };

static u32 GetSysMsgBoxCharDatID(u8 type) {
    return SYSTEM_MSGBOX_DATIDS_CHAR[type];
}

u32 GetSysMsgBoxPaletteDatID(u8 type) {
    return SYSTEM_MSGBOX_DATIDS_PALETTE[type];
}

void LoadSysMsgBoxBGChar(u8 bg, u16 frameChar, u8 type, HeapID heapId) {
    GFL_BGSysLoadNCGRStatic(ARCID_WINFRAME, GetSysMsgBoxCharDatID(type), bg, frameChar, 0, FALSE, heapId);
}

void LoadSysMsgBoxPalette(u8 bg, u8 framePalette, u8 type, HeapID heapId) {
    u32 fileId = GetSysMsgBoxPaletteDatID(type);

    if (bg < BGSYS_BG_SUB) {
        GFL_BGSysLoadNCLRDefault(ARCID_WINFRAME, fileId, PALTYPE_MAIN_BG, framePalette * 0x20, 0x20, heapId);
    } else {
        GFL_BGSysLoadNCLRDefault(ARCID_WINFRAME, fileId, PALTYPE_SUB_BG, framePalette * 0x20, 0x20, heapId);
    }
}

void LoadSysMsgBox(u8 bg, u16 frameChar, u8 framePalette, u8 type, HeapID heapId) {
    LoadSysMsgBoxBGChar(bg, frameChar, type, heapId);
    LoadSysMsgBoxPalette(bg, framePalette, type, heapId);
}

u32 LoadCursorImageEndOfHeap(u8 bg, u8 framePalette, u8 type, HeapID heapId) {
    u32 chars = GFL_BGSysLoadNCGRDynamic(ARCID_WINFRAME, GetSysMsgBoxCharDatID(type), bg, 0, FALSE, heapId);

    LoadSysMsgBoxPalette(bg, framePalette, type, heapId);
    return chars;
}

void FreeCursorImageEndOfHeap(u32 bg, u32 chars) {
    GFL_BGSysFreeCharMemory(bg, CHAR_POS(chars), CHAR_SIZE(chars));
}

static void BmpWin_DrawFrameCore(u8 bg, u8 x, u8 y, u8 width, u8 height, u8 palette, u16 frameChar) {
    GFL_BGSysFillScrArea(bg, frameChar, x - 1, y - 1, 1, 1, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 1, x, y - 1, width, 1, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 2, x + width, y - 1, 1, 1, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 3, x - 1, y, 1, height, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 5, x + width, y, 1, height, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 6, x - 1, y + height, 1, 1, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 7, x, y + height, width, 1, palette);
    GFL_BGSysFillScrArea(bg, frameChar + 8, x + width, y + height, 1, 1, palette);
}

void BmpWin_DrawFrame(BmpWin *window, u8 transfer, u16 frameChar, u8 framePalette) {
    u8 bg = BmpWin_GetBGIndex(window);

    BmpWin_DrawFrameCore(bg, BmpWin_GetPosX(window), BmpWin_GetPosY(window), BmpWin_GetWidth1(window),
                         BmpWin_GetHeight2(window), framePalette, frameChar);
    if (transfer == WINFRAME_TRANSFER_NOW) {
        GFL_BGSysLoadScr(bg);
    } else if (transfer == WINFRAME_TRANSFER_VBLANK) {
        GFL_BGSysQueueScrLoad(bg);
    }
}

void BmpWin_DrawFrameEndOfHeap(BmpWin *window, u8 transfer, u32 chars, u8 framePalette) {
    BmpWin_DrawFrame(window, transfer, CHAR_POS(chars), framePalette);
}

void BmpWin_ClearFrame(BmpWin *window, u8 transfer) {
    u8 bg = BmpWin_GetBGIndex(window);

    GFL_BGSysFillScrArea(bg, 0, BmpWin_GetPosX(window) - 1, BmpWin_GetPosY(window) - 1, BmpWin_GetWidth1(window) + 2,
                         BmpWin_GetHeight2(window) + 2, 0);
    if (transfer == WINFRAME_TRANSFER_NOW) {
        GFL_BGSysLoadScr(bg);
    } else if (transfer == WINFRAME_TRANSFER_VBLANK) {
        GFL_BGSysQueueScrLoad(bg);
    }
}
