#ifndef POKEHEARTGOLD_OVERLAY_74_RANGER_LINK_MANAGER_H
#define POKEHEARTGOLD_OVERLAY_74_RANGER_LINK_MANAGER_H

// RangerLinkManager: the main menu's "Connect to Pokemon Ranger" app (ov74_02229200 allocates it, 0x34DC bytes on heap 83;
// ov74_02229294 runs it through OverlayManager_GetData). The one definition with its DS layout; the m2c dumps
// 02228D20_02228D64 / 02228D20_02229084 / 02228D20_02229294 / 022295D0_0222962C still carry layout-identical local copies.

#include "constants/heap.h"

#include "bg_window.h"
#include "mystery_gift.h"
#include "overlay_74_main_menu_window.h"

struct OverlayManager;
struct Options;
struct PlayerProfile;
struct Pokedex;
struct SaveData;
struct Sprite;
struct WaitingIcon;

typedef struct RangerLinkManager {
    enum HeapID heapID;
    BgConfig *bgConfig;
    struct SaveData *saveData;
    struct Pokedex *pokedex;
    struct PlayerProfile *trainerInfo;
    struct Options *options;
    Window stdWindow;
    Window messageBoxWindow;
    Window appNumberStdWindow;
    MainMenuWindow messageBox;
    MainMenuWindow manaphyEggTransferWindow;
    MainMenuWindow playerCommsDetails;
    int nextWindowBasetile;
    BOOL wirelessStarted;
    int unused_14C;
    int wirelessStatus;
    int unused_154;
    int wirelessActive;
    u32 wirelessState; // enum RangerLinkRangerCommState
    int timeout;
    int unk_164;
    int unk_168;
    u8 wirelessCommBuffer[12288];
    struct OverlayManager *appMan;
    struct Sprite *downloadArrow;
    MysteryGiftSave *mysteryGift;
    int unused_3178;
    int deliveryManOverloadedPrinterID;
    WonderCard manaphyEggWonderCard;
    struct WaitingIcon *waitDial;
} RangerLinkManager;

#ifdef __MWERKS__
typedef char RangerLinkManager_nextWindowBasetile_check[(offsetof(RangerLinkManager, nextWindowBasetile) == 0x144) ? 1 : -1];
typedef char RangerLinkManager_wirelessState_check[(offsetof(RangerLinkManager, wirelessState) == 0x15C) ? 1 : -1];
typedef char RangerLinkManager_mysteryGift_check[(offsetof(RangerLinkManager, mysteryGift) == 0x3174) ? 1 : -1];
typedef char RangerLinkManager_size_check[(sizeof(RangerLinkManager) == 0x34DC) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_74_RANGER_LINK_MANAGER_H
