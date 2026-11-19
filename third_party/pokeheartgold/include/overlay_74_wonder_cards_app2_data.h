#ifndef POKEHEARTGOLD_OVERLAY_74_WONDER_CARDS_APP2_DATA_H
#define POKEHEARTGOLD_OVERLAY_74_WONDER_CARDS_APP2_DATA_H

#include "bg_window.h"
#include "font.h"
#include "list_menu.h"
#include "list_menu_items.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_74_wonder_card_flip_anim.h"
#include "save.h"
#include "sprite.h"

typedef struct WonderCardsApp2Data WonderCardsApp2Data;

// Window setup callback of a WonderCardsApp2WindowTemplate row; TRUE = print the row's entryID text.
typedef BOOL (*WonderCardsApp2WindowSetupFunc)(WonderCardsApp2Data *appData, Window *window, u32 color);

// One row (0x30 bytes) of the 11-row window table ov74_0223C700 (Platinum: WonderCardsAppWindowTemplate).
typedef struct WonderCardsApp2WindowTemplate {
    int screen;                           // 0x00 0 = card front, 1 = card back, 2 = message box
    int tilemapLeft;                      // 0x04
    int tilemapTop;                       // 0x08
    int width;                            // 0x0C
    int height;                           // 0x10
    enum Font font;                       // 0x14
    u32 textColor;                        // 0x18
    int bgColor;                          // 0x1C font attribute (screen windows) or fill color index (message boxes)
    u32 entryID;                          // 0x20 msg 0xF7 entry, 0 = none
    WonderCardsApp2WindowSetupFunc setup; // 0x24
    int leftMarginSize;                   // 0x28
    int topMarginSize;                    // 0x2C
} WonderCardsApp2WindowTemplate;

// One list menu option {text entry of msg 0xF7, menu value} (ov74_0223C6D0; Platinum: StateTransitionMenuOptionTemplate).
typedef struct WonderCardsApp2MenuOption {
    int textEntryID;
    int value;
} WonderCardsApp2MenuOption;

// State callback stored as a list menu value (>= 31); returns the next state or -1.
typedef int (*WonderCardsApp2StateFunc)(OverlayManager *man);

// WonderCardsApp2Data: the second ov74 wonder cards app (ov74_0222F598 init, OverlayManager_CreateAndGetData(0x3d0c);
// ov74_0222FC50 builds a temporary one to show a card). DS layout from the asm offsets of ov74_0222EC60..ov74_0222FC50;
// the first copy (0x3d54, ov74_0222EC08) is WonderCardsAppData. Unaccessed ranges are pads.
struct WonderCardsApp2Data {
    enum HeapID heapID;                        // 0x0000
    MysteryGiftAppData mysteryGiftAppData;     // 0x0004
    BgConfig *bgConfig;                        // 0x29FC
    MessageFormat *msgFormat;                  // 0x2A00
    MsgData *msgData;                          // 0x2A04
    Window windows[20];                        // 0x2A08
    int windowBasetiles[20];                   // 0x2B48
    int messageBoxBaseTile;                    // 0x2B98
    WonderCardsApp2WindowTemplate *messageBoxTemplate; // 0x2B9C (an ov74_0223C700 row)
    MysteryGiftSave *mysteryGift;              // 0x2BA0
    SaveData *saveData;                        // 0x2BA4
    Options *options;                          // 0x2BA8
    int msgBoxFrame;                           // 0x2BAC
    u8 unk_2BB0[0x4];                          // 0x2BB0
    WonderCard *wonderCard;                    // 0x2BB4
    int currentScreen;                         // 0x2BB8
    ListMenu *listMenu;                        // 0x2BBC
    ListMenuItem *strList;                     // 0x2BC0
    Window messageBox;                         // 0x2BC4
    Window standardWindow;                     // 0x2BD4
    int nextWindowBasetile;                    // 0x2BE4
    u8 unk_2BE8[0xC];                          // 0x2BE8
    int unk_2BF4;                              // 0x2BF4
    int queuedState;                           // 0x2BF8
    SpriteList *spriteList;                    // 0x2BFC
    u8 unk_2C00[0x17C];                        // 0x2C00 (G2dRenderer + sprite resources in Platinum)
    Sprite *shareScreenBtnSprites[2];          // 0x2D7C
    Sprite *selectedWcSprites[3];              // 0x2D84
    WonderCard currentWonderCard;              // 0x2D90
    WonderCardFlipAnimManager flipAnimMan;     // 0x30E8
    void (*mainCallback)(WonderCardsApp2Data *appData); // 0x3D00
    u8 unk_3D04[0x8];                          // 0x3D04
};                                             // size 0x3D0C

extern WonderCardsApp2WindowTemplate ov74_0223C700[11];

#endif // POKEHEARTGOLD_OVERLAY_74_WONDER_CARDS_APP2_DATA_H
