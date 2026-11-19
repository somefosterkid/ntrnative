#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "font.h"
#include "message_format.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "pm_string.h"
#include "pokedex.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "system.h"
#include "gf_gfx_planes.h"

typedef struct MainMenuWindow MainMenuWindow;

typedef struct RangerLinkManager RangerLinkManager;

typedef u32 TextColor;

struct MainMenuWindow {
    BOOL scrollable;
    BOOL drawFrame;
    BOOL clearBeforePrinting;
    BOOL textRightAligned;
    Window *window;
    MessageFormat *strTemplate;
    int width;
    int height;
    int textXOffset;
    int textYOffset;
    int baseTile;
    enum GFBgLayer bgLayer;
    int textPltt;
    int textBank;
    int frameBaseTile;
    int framePltt;
    enum Font font;
    TextColor textColor;
    u8 backgroundColorIdx;
    u8 pad_49[3];
    int textEntryID;
    int renderDelay;
};

enum RangerLinkRangerCommState {
    RANGER_COMM_STATE_INIT = 0,
    RANGER_COMM_STATE_WAIT_READY,
    RANGER_COMM_STATE_2,
    RANGER_COMM_STATE_3,
    RANGER_COMM_STATE_4,
    RANGER_COMM_STATE_UNUSED_5,
    RANGER_COMM_STATE_UNUSED_6,
    RANGER_COMM_STATE_7,
    RANGER_COMM_STATE_8,
    RANGER_COMM_STATE_9,
    RANGER_COMM_STATE_WAIT_RANGER_RESPONSE,
    RANGER_COMM_STATE_11,
    RANGER_COMM_STATE_12,
    RANGER_COMM_STATE_13,
    RANGER_COMM_STATE_14,
    RANGER_COMM_STATE_15,
    RANGER_COMM_STATE_16,
    RANGER_COMM_STATE_17,
    RANGER_COMM_STATE_18,
    RANGER_COMM_STATE_TRANSFER_COMPLETED,
    RANGER_COMM_STATE_WAIT_TRANSFER_COMPLETED,
    RANGER_COMM_STATE_UNUSED_21,
    RANGER_COMM_STATE_GOTO_CONNECTION_LOST,
    RANGER_COMM_STATE_CONNECTION_LOST,
    RANGER_COMM_STATE_UNUSED_24,
    RANGER_COMM_STATE_UNUSED_25,
    RANGER_COMM_STATE_CONNECTION_CANCELED,
    RANGER_COMM_STATE_DONE,
    RANGER_COMM_STATE_SHUTDOWN_WIRELESS_AND_REBOOT,
    RANGER_COMM_STATE_29,
    RANGER_COMM_STATE_30
};

struct RangerLinkManager {
    enum HeapID heapID;
    BgConfig *bgConfig;
    SaveData *saveData;
    Pokedex *pokedex;
    PlayerProfile *trainerInfo;
    Options *options;
    Window stdWindow;
    Window messageBoxWindow;
    Window appNumberStdWindow;
    struct { BOOL scrollable; BOOL drawFrame; BOOL clearBeforePrinting; BOOL textRightAligned; struct Window *window; struct MessageFormat *strTemplate; int width; int height; int textXOffset; int textYOffset; int baseTile; u32 bgLayer; int textPltt; int textBank; int frameBaseTile; int framePltt; u32 font; u32 textColor; u8 backgroundColorIdx; u8 unk_4C_pad[3]; int textEntryID; int renderDelay; } messageBox;
    struct { BOOL scrollable; BOOL drawFrame; BOOL clearBeforePrinting; BOOL textRightAligned; struct Window *window; struct MessageFormat *strTemplate; int width; int height; int textXOffset; int textYOffset; int baseTile; u32 bgLayer; int textPltt; int textBank; int frameBaseTile; int framePltt; u32 font; u32 textColor; u8 backgroundColorIdx; u8 unk_4C_pad[3]; int textEntryID; int renderDelay; } manaphyEggTransferWindow;
    struct { BOOL scrollable; BOOL drawFrame; BOOL clearBeforePrinting; BOOL textRightAligned; struct Window *window; struct MessageFormat *strTemplate; int width; int height; int textXOffset; int textYOffset; int baseTile; u32 bgLayer; int textPltt; int textBank; int frameBaseTile; int framePltt; u32 font; u32 textColor; u8 backgroundColorIdx; u8 unk_4C_pad[3]; int textEntryID; int renderDelay; } playerCommsDetails;
    int nextWindowBasetile;
    BOOL wirelessStarted;
    int unused_14C;
    int wirelessStatus;
    int unused_154;
    int wirelessActive;
    enum RangerLinkRangerCommState wirelessState;
    int timeout;
    int unk_164;
    int unk_168;
    u8 wirelessCommBuffer[12288];
    OverlayManager *appMan;
    Sprite *downloadArrow;
    MysteryGift *mysteryGift;
    int unused_3178;
    int deliveryManOverloadedPrinterID;
    WonderCard manaphyEggWonderCard;
    WaitingIcon *waitDial;
};

int ov74_02235568(BgConfig *param0, MainMenuWindow *window, int xPos, int yPos, int textEntryID);

s32 ov74_02235414(MainMenuWindow *, Window *, s32, s32, s32, s32);

s32 ov74_02235464(MainMenuWindow *, s32, s32, s32);

s32 ov74_02228F14(RangerLinkManager *param0);

s32 ov74_02228F14(RangerLinkManager *param0) {
    MainMenuWindow v0;

    if (WindowIsInUse(&param0->stdWindow) == 0) {
        ov74_02235414(&v0, &param0->stdWindow, 0, 0x11, 1, 1);
        ov74_02235464(&v0, 0x16, 0x10, param0->nextWindowBasetile);
        ov74_02235568(param0->bgConfig, &v0, 5, 4, 2);
        return 1;
    }
    if (3 & gSystem.newKeys) {
        ClearWindowPixelBuffer(&param0->stdWindow, 0);
        RemoveWindow(&param0->stdWindow);
        return 0;
    }
    return 1;
}
