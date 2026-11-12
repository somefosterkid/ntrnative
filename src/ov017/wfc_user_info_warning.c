// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov032/wfc_user_info_warning.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "app/wfc_user_info_warning.h"
#include "types.h"
#include "constants/arc.h"
#include "constants/text_banks.h"
#include "gfl/arc_util.h"
#include "gfl/bmp.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/ui.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/memorymap.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "system/dsi.h"
#include "system/gf_font.h"
#include "system/main.h"
#include "system/printsys.h"
#include "nitro/mi.h"

void SystemUpdateVBlank(void);

// The notice that the Wi-Fi connection's user data may have been lost: a window on BG1 of the main
// screen with the message, until A or B is pressed. It draws the screen the way save_error.c does, with the same
// graphics. The file's name is a guess, after pokeheartgold's: the ROM has no string for it

// The messages of the warning in MSG_FILE_WIFI_ERROR, for a DS and for a DSi
#define MSG_USER_INFO_ERASED 16
#define MSG_USER_INFO_ERASED_DSI 42

// The archive of the window's graphics (save_error.c's), and its files
#define ARCID_ERROR_WINDOW_GRA 22
#define ERROR_WINDOW_GRA_PLTT 0
#define ERROR_WINDOW_GRA_CHAR 1
#define ERROR_WINDOW_GRA_SCRN 2

// The message file of Nintendo Wi-Fi Connection's errors in ARCID_SYSTEM_MESSAGE
#define MSG_FILE_WIFI_ERROR TEXT_BANK_WIFI_ERROR

// The text area: the tiles from FIRST_TEXT_TILE, laid out in rows of the screen from TEXT_TOP, TEXT_LEFT
#define TILE_SIZE 0x20
#define FIRST_TEXT_TILE 0x20
#define TEXT_LEFT 1
#define TEXT_TOP 4
#define TEXT_WIDTH 30
#define TEXT_HEIGHT 16
#define TEXT_BACKGROUND 7

// The keys that close the warning
#define CLOSE_KEYS (PAD_BUTTON_A | PAD_BUTTON_B)

void func_ov017_021b61f8(u32 message);
static void func_ov017_021b6298(void);
static void func_ov017_021b6368(u32 message);
static void func_ov017_021b641c(void);

// The keys held, up cancelling down and left cancelling right, as key.c reads them
static inline u16 ReadKeys(void) {
    u16 keys = PAD_Read();

    keys = keys & ~((keys & PAD_KEY_UP) << 1) & ~((keys & PAD_KEY_LEFT) >> 1);
    return keys;
}

// pokebw2's WFCUserInfoWarning_Show
void func_020053ec(void);
void func_ov017_021b6100(void) {
    func_ov017_021b61f8(func_0202eb0c() ? MSG_USER_INFO_ERASED_DSI : MSG_USER_INFO_ERASED);
    // Wait for the keys to be released, then pressed
    while (ReadKeys() & CLOSE_KEYS) {
        func_020053ec();
        GCTX_HIDUpdate();
    }
    while (!(ReadKeys() & CLOSE_KEYS)) {
        func_020053ec();
        GCTX_HIDUpdate();
    }
    func_ov017_021b641c();
}

// pokebw2's WFCUserInfoWarning_Init
void func_ov017_021b61f8(u32 message) {
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GX_ResetBankForBG();
    GX_ResetBankForSubBG();
    GX_SetBankForBG(GX_VRAM_BG_128_C);
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX_BG0_AS_2D);
    GX_SetVisiblePlane(GX_PLANEMASK_BG1);
    GX_SetVisibleWnd(GX_WNDMASK_NONE);
    G2_SetBG1Control(GX_BG_SCRSIZE_TEXT_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x0000, GX_BG_CHARBASE_0x04000,
                     GX_BG_EXTPLTT_01);
    G2_BG1Mosaic(FALSE);
    G2_SetBG1Offset(0, 0);
    G2_BlendNone();
    func_ov017_021b6298();
    func_ov017_021b6368(message);
    GX_SetMasterBrightness(0);
    GXS_SetMasterBrightness(16);
}

// pokebw2's WFCUserInfoWarning_LoadWindow
static void func_ov017_021b6298(void) {
    void *file;
    NNSG2dCharacterData *chars;
    NNSG2dScreenData *screen;
    NNSG2dPaletteData *palette;
    NNSG2dPaletteCompressInfo *compressInfo;

    MIi_CpuClear32(0, G2_GetBG1CharPtr(), 0x4000);

    file = GFL_ArcSysReadHeapNewLZ(ARCID_ERROR_WINDOW_GRA, ERROR_WINDOW_GRA_CHAR, FALSE, HEAPID_USER);
    if (NNS_G2dGetUnpackedBGCharacterData(file, &chars)) {
        DC_FlushRange(chars->pRawData, chars->szByte);
        MI_CpuCopy16(chars->pRawData, G2_GetBG1CharPtr(), chars->szByte);
    }
    GFL_HeapFree(file);

    file = GFL_ArcSysReadHeapNewLZ(ARCID_ERROR_WINDOW_GRA, ERROR_WINDOW_GRA_SCRN, FALSE, HEAPID_USER);
    if (NNS_G2dGetUnpackedScreenData(file, &screen)) {
        DC_FlushRange(screen->rawData, screen->szByte);
        MI_CpuCopy16(screen->rawData, G2_GetBG1ScrPtr(), screen->szByte);
    }
    GFL_HeapFree(file);

    file = GFL_ArcSysReadHeapNewLZ(ARCID_ERROR_WINDOW_GRA, ERROR_WINDOW_GRA_PLTT, FALSE, HEAPID_USER);
    NNS_G2dGetUnpackedPaletteCompressInfo(file, &compressInfo);
    if (NNS_G2dGetUnpackedPaletteData(file, &palette)) {
        DC_FlushRange(palette->pRawData, 0x20);
        MI_CpuCopy16(palette->pRawData, (void *)HW_BG_PLTT, 0x20);
    }
    GFL_HeapFree(file);
}

// Lays the text area's tiles out on the screen, and prints the message in them
// pokebw2's WFCUserInfoWarning_PrintMessage
static void func_ov017_021b6368(u32 message) {
    u16 *screen;
    int x;
    int y;
    u16 tile;
    GFLBitmap *bitmap;
    Font *font;
    MsgData *msgData;
    StrBuf *str;

    screen = G2_GetBG1ScrPtr();
    tile = FIRST_TEXT_TILE;
    for (y = TEXT_TOP; y < TEXT_TOP + TEXT_HEIGHT; y++) {
        for (x = TEXT_LEFT; x < TEXT_LEFT + TEXT_WIDTH; x++) {
            screen[y * 32 + x] = tile;
            tile++;
        }
    }
    MIi_CpuClear16(TEXT_BACKGROUND * 0x1111, (u8 *)G2_GetBG1CharPtr() + FIRST_TEXT_TILE * TILE_SIZE,
                 TEXT_WIDTH * TEXT_HEIGHT * TILE_SIZE);
    bitmap = GFL_BitmapWrapVRAM((u8 *)G2_GetBG1CharPtr() + FIRST_TEXT_TILE * TILE_SIZE, TEXT_WIDTH, TEXT_HEIGHT,
                                TILE_SIZE, HEAPID_USER);
    font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, HEAPID_USER);
    msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, MSG_FILE_WIFI_ERROR, HEAPID_USER);
    str = GFL_MsgDataLoadStrbufNew(msgData, message);
    GFL_TextRendererDrawToBitmapEx(bitmap, 0, 0, str, font, PRINT_COLOR(4, 11, TEXT_BACKGROUND));
    GFL_StrBufFree(str);
    GFL_MsgDataFree(msgData);
    GFL_FontFree(font);
    GFL_BitmapFree(bitmap);
}

// Clears the window and gives the banks back
// pokebw2's WFCUserInfoWarning_Exit
static void func_ov017_021b641c(void) {
    GX_SetMasterBrightness(-16);
    GXS_SetMasterBrightness(-16);
    GFL_BGSysSetEnabledBGsB(0);
    MIi_CpuClear16(0, G2_GetBG1CharPtr(), 0x4000);
    MIi_CpuClear16(0, G2_GetBG1ScrPtr(), 0x800);
    MIi_CpuClear16(GX_RGB(31, 31, 31), (void *)HW_BG_PLTT, 0x20);
    GX_SetVisiblePlane(0);
    GX_ResetBankForSubBG();
    GX_ResetBankForBG();
    GX_SetMasterBrightness(0);
    GXS_SetMasterBrightness(0);
}
