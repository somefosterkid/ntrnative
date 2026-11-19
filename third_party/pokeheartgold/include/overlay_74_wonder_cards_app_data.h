#ifndef POKEHEARTGOLD_OVERLAY_74_WONDER_CARDS_APP_DATA_H
#define POKEHEARTGOLD_OVERLAY_74_WONDER_CARDS_APP_DATA_H

#include "bg_window.h"
#include "font.h"
#include "g2d_renderer.h"
#include "list_menu.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"
#include "options.h"
#include "overlay_manager.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_74_mystery_gift_app_data.h"
#include "overlay_74_wonder_card_flip_anim.h"
#include "player_data.h"
#include "render_window.h"
#include "save.h"
#include "sprite.h"
#include "sprite_resources_header.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"

// WonderCardsAppData: the ov74 Wonder Cards app (OverlayManager data of ov74_0222DDFC, HEAP_ID_85, DS size 0x3d54).
// Layout = the matching DS code (ov74_0222D024..0222EC08). The 0x3d0c second app (WonderCardsApp2Data) shares
// MysteryGiftAppData and WonderCardFlipAnimManager (overlay_74_wonder_card_flip_anim.h).

enum WonderCardsAppWindow {
    WC_FRONT_TEXT_WONDER_CARD = 0,
    WC_FRONT_TEXT_DATE_RECEIVED,
    WC_FRONT_GIFT_STATUS,
    WC_FRONT_WONDERCARD_TITLE,
    WC_FRONT_RECEIVED_DATE,
    WC_BACK_DESCRIPTION,
    WC_BACK_SHARES_LEFT,
    MSG_BOX_TEXT_WONDER_CARD,
    MSG_BOX_WC_ACTIONS_MENU,
    MSG_BOX_CONFIRM_WC_DELETE,
    MSG_BOX_CONFIRM_WC_SHARE,
    WC_SHARING_TEXT_PAL_DISTRIBUTION_REGISTRATION,
    WC_SHARING_TEXT_CURRENT_ENTRIES,
    WC_SHARING_TEXT_SEND,
    WC_SHARING_TEXT_CANCEL,
    WC_SHARING_ENTRIES_COUNT,
    WC_SHARING_PLAYER_INFO,
    MSG_BOX_DISTRIBUTION_UNDER_WAY,
    MSG_BOX_DISTRIBUTION_SUCCESSFUL,
    NUM_WC_APP_WINDOWS,
};

enum WonderCardsAppState {
    WC_APP_STATE_LOAD_FROM_SAVE = 0,
    WC_APP_STATE_INIT_GRAPHICS,
    WC_APP_STATE_LOAD_GRAPHICS,
    WC_APP_STATE_SELECT_WONDERCARD,
    WC_APP_STATE_SHOW_WONDERCARD_ACTIONS,
    WC_APP_STATE_WAIT_FOR_MENU_CHOICE,
    WC_APP_STATE_START_FLIP_WC_TO_BACK,
    WC_APP_STATE_WAIT_WC_FLIP_TO_BACK_HALFWAY,
    WC_APP_STATE_SHOW_WONDERCARD_BACK,
    WC_APP_STATE_START_FLIP_WC_TO_FRONT,
    WC_APP_STATE_WAIT_WC_FLIP_TO_FRONT_HALFWAY,
    WC_APP_STATE_CLOSE_WINDOWS,
    WC_APP_STATE_WAIT_CONFIRM_DELETION,
    WC_APP_STATE_DISMISS_MESSAGE_BOX, // not in Platinum's list: waits for A/B, closes the message box, back to SELECT_WONDERCARD
    WC_APP_STATE_WAIT_CONFIRM_START_WIRELESS,
    WC_APP_STATE_ASK_START_WIRELESS_TO_SHARE_WC,
    WC_APP_STATE_START_TRANSITION_TO_WC_SHARE_SCREEN,
    WC_APP_STATE_SHOW_WC_SHARE_SCREEN,
    WC_APP_STATE_PREPARE_FOR_SHARING,
    WC_APP_STATE_WAIT_FOR_PLAYERS,
    WC_APP_STATE_RETURN_AFTER_COMM_MAN_EXIT,
    WC_APP_STATE_RETURN_TO_WC_ACTIONS,
    WC_APP_STATE_DISTRIBUTION_UNDERWAY,
    WC_APP_STATE_WAIT_FOR_COMM_MAN_EXIT,
    WC_APP_STATE_START_COMM_SYNC,
    WC_APP_STATE_WAIT_FOR_COMM_SYNC,
    WC_APP_STATE_DISTRIBUTION_SUCCESSFUL,
    WC_APP_STATE_NO_MORE_WONDERCARDS,
    WC_APP_STATE_WAIT_FOR_SCREEN_TRANSITION,
    WC_APP_STATE_EXIT,
    WC_APP_STATE_UNUSED,
    NUM_WC_APP_STATES,
};

enum WonderCardsAppScreen {
    WC_SCREEN_WONDERCARD_FRONT = 0,
    WC_SCREEN_WONDERCARD_BACK,
    WC_MESSAGE_BOX,
    WC_SCREEN_WONDERCARD_SHARING,
};

enum WonderCardFlipStage {
    WC_FLIP_STAGE_GROWING = 0,
    WC_FLIP_STAGE_SHRINKING,
};

enum WonderCardShareBtn {
    WC_SHARE_BTN_SEND = 0,
    WC_SHARE_BTN_CANCEL,
};

#define WC_APP_NUM_WINDOWS_PLUS_ONE 20

typedef struct WonderCardsAppData WonderCardsAppData;

typedef BOOL (*WcAppWindowSetupFuncPtr)(WonderCardsAppData *appData, Window *window, u32 color);
typedef void (*WcAppMainCallbackFuncPtr)(WonderCardsAppData *appData);
typedef enum WonderCardsAppState (*StateTransitionFuncPtr)(OverlayManager *man);

// One option of a list menu: an entry of msg_0247 and its value (a WonderCardsAppState, or a StateTransitionFuncPtr).
typedef struct StateTransitionMenuOptionTemplate {
    u32 entryID;
    u32 value;
} StateTransitionMenuOptionTemplate;

typedef struct WonderCardsAppWindowTemplate {
    enum WonderCardsAppScreen screen;
    int tilemapLeft;
    int tilemapTop;
    int width;
    int height;
    enum Font font;
    int textColor;
    union {
        int fontAttribute;
        int colorIndex;
    } bgColor;
    u32 entryID;
    WcAppWindowSetupFuncPtr setup;
    int leftMarginSize;
    int topMarginSize;
} WonderCardsAppWindowTemplate;

struct WonderCardsAppData {
    enum HeapID heapID;                                 // 0x0000
    MysteryGiftAppData mysteryGiftAppData;              // 0x0004
    BgConfig *bgConfig;                                 // 0x29FC
    MessageFormat *strTemplate;                         // 0x2A00
    MsgData *msgLoader;                                 // 0x2A04
    Window windows[WC_APP_NUM_WINDOWS_PLUS_ONE];        // 0x2A08
    int windowBasetiles[WC_APP_NUM_WINDOWS_PLUS_ONE];   // 0x2B48
    int messageBoxBaseTile;                             // 0x2B98
    void *messageBoxTemplate;                           // 0x2B9C
    MysteryGiftSave *mysteryGift;                       // 0x2BA0
    SaveData *saveData;                                 // 0x2BA4
    Options *options;                                   // 0x2BA8
    int msgBoxFrame;                                    // 0x2BAC
    int unused_2BB0;                                    // 0x2BB0
    WonderCard *wonderCards[NUM_SAVED_WONDER_CARDS];    // 0x2BB4
    int selectedWondercardSlot;                         // 0x2BC0
    int currentScreen;                                  // 0x2BC4
    ListMenu *listMenu;                                 // 0x2BC8
    ListMenuItem *strList;                              // 0x2BCC
    Window messageBox;                                  // 0x2BD0
    Window standardWindow;                              // 0x2BE0
    int nextWindowBasetile;                             // 0x2BF0
    int connectedPlayersCount;                          // 0x2BF4
    u32 unused_2BF8[4];                                 // 0x2BF8
    PlayerProfile *connTrainerInfos[1 + 4];             // 0x2C08 (index = network id; 0 = this player)
    int orderNumbers[1 + 4];                            // 0x2C1C
    int prevConnPlayersCount;                           // 0x2C30
    int shouldSendWc;                                   // 0x2C34
    void (*unused_2C38)(void *);                        // 0x2C38
    int numPlayerConnections;                           // 0x2C3C
    int queuedState;                                    // 0x2C40
    SpriteList *spriteList;                             // 0x2C44
    G2dRenderer g2dRenderer;                            // 0x2C48
    SpriteResourceCollection *spriteResourceCollections[6]; // 0x2D70
    SpriteResource *spriteResources[6];                 // 0x2D88
    SpriteResourcesHeader spriteResourceHeader;         // 0x2DA0
    Sprite *shareScreenBtnSprites[2];                   // 0x2DC4
    Sprite *selectedWcSprites[3];                       // 0x2DCC
    WonderCard currentWonderCard;                       // 0x2DD8
    WonderCardFlipAnimManager flipAnimMan;              // 0x3130
    WcAppMainCallbackFuncPtr mainCallback;              // 0x3D48
    int shareScreenSelectedBtn;                         // 0x3D4C
    WaitingIcon *waitDial;                              // 0x3D50
};

// sizeof(WonderCardsAppData) == 0x3d54

#endif // POKEHEARTGOLD_OVERLAY_74_WONDER_CARDS_APP_DATA_H
