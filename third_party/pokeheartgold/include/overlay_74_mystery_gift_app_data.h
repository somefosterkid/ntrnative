#ifndef POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_APP_DATA_H
#define POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_APP_DATA_H
#include "sprite_resources_header.h"

#include "bg_window.h"
#include "g2d_renderer.h"
#include "message_format.h"
#include "msgdata.h"
#include "mystery_gift.h"

// MysteryGiftAppData: merged by tools/hg/promote.py from 39 m2c local copies (1 variants, base
// src/overlay_74_thumb_022295D0_02229BD0_02229BD0.c) and 59 accesses; fields at every accessed offset, pads elsewhere.

struct ListMenu;
struct MsgData;
struct NNSG2dAnimBankData;
struct NNSG2dCellDataBank;
struct NNSG2dCharacterData;
struct NNSG2dImagePaletteProxy;
struct NNSG2dImageProxy;
struct NNSG2dMultiCellDataBank;
struct NNSG2dPaletteSwapTable;
struct NNSG2dRenderSurface;
struct NNSG2dRndCore2DMtxCache;
struct NNSG2dRndCoreSurface;
struct Options;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct SpriteResourceCollection;
struct String;
struct ListMenuItem;
struct StringList;
struct StringTemplate;

// A received event: its card header (UnkWonderCardSubstruct_104: title, games, id, flag bits) + the card.
typedef struct MysteryGiftEventData {
    UnkWonderCardSubstruct_104 header; // 0x000
    WonderCard wonderCard;              // 0x050
} MysteryGiftEventData;

typedef struct MysteryGiftAppData {
    struct BgConfig *bgConfig;
    struct SaveData *saveData;
    struct Options *options;
    MessageFormat *msgFmt;
    MsgData *msgData;
    struct String *string;
    Window messageBox;
    Window menuWindows[3];
    Window wonderCardTitleWindow;
    int msgBoxPrinterDelay;
    int textPrinterId;
    int unused_70;
    u32 queuedState;
    struct ListMenu *listMenu;
    struct ListMenuItem *listMenuOptions;
    int canReceiveGiftStatus;
    int foundGiftType;
    int unused_88;
    MysteryGiftEventData eventData;
    int wirelessCommsTimeout;
    int unk_438;
    int delay;
    BOOL exitToWondercardsApp;
    struct SpriteList *unusedSpriteList;
    G2dRenderer unusedG2dRenderer;
    struct SpriteResourceCollection *unusedSpriteResourceCollections[6];
    struct SpriteResource *unusedSpriteResources[6];
    SpriteResourcesHeader unusedSpriteResourcesHeader;
    struct Sprite *dowloadArrowSprite;
    int unk_5C8;
    int unk_5CC;
    int unk_5D0;
    u8 wirelessDistributionBuffer[4096];
    int receptionMethod;
    void *statePtr;
    int giftSearchResultDelay;
    int nasAuthState;
    u8 *unused_1648;
    struct { int ac_state; u16 state; u16 online; u16 dmaNo; u16 powerMode; u8 wontExecuteWDS; u8 unk_10_pad[0x3]; u32 processWDSstate; struct { u8 ssid[32]; u8 wep[32]; int wepMode; s8 apnum[10]; u8 unk_4E_tail[0x2]; } nspotInfo; } unk_164C;
    BOOL unused_16AC;
    int unk_16B0;
    int unk_16B4;
    s8 wifiDistributionBuffer[4096];
    int unused_26B8;
    u32 unk_26BC;
    u32 unk_26C0;
    u32 unk_26C4;
    BOOL wifiCommErrored;
    int wifiCommErrorStringID;
    int wifiCommErrorCode;
    u32 wifiCommErrorType;
    BOOL wifiExitRequested;
    void *dwcCallback;
    Window wifiCommErrorWindow;
    WonderCard wonderCard;
    void *mainCallback;
    BOOL cancelSave;
    void *waitDial;
    u32 animationStatus;
} MysteryGiftAppData;

// sizeof(MysteryGiftAppData) == 0x29f8

#endif // POKEHEARTGOLD_OVERLAY_74_MYSTERY_GIFT_APP_DATA_H
