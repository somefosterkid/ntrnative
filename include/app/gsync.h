// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/gsync.h, the lines in use so far.
#ifndef BWPORT_APP_GSYNC_H
#define BWPORT_APP_GSYNC_H

#include "types.h"
#include "app/box2.h"
#include "app/wifi_login.h"
#include "gfl/overlay.h"
#include "gfl/proc.h"
#include "struct_decls.h"

// Game Sync and the Dream World account, overlay 199 in Black 2. Game Sync's event (field/event_gsync.h) runs the
// menu, then Game Sync itself
#define OVERLAY_GSYNC OVERLAY_ID(123)

// Results of the Game Sync procs
#define GSYNC_RESULT_NONE 0
#define GSYNC_RESULT_ACCOUNT 1
#define GSYNC_RESULT_CONNECT 2
#define GSYNC_RESULT_WIFI_SETTINGS 3
#define GSYNC_RESULT_NO_POKEMON 4
#define GSYNC_RESULT_SELECT_POKEMON 5
#define GSYNC_RESULT_DONE 6
#define GSYNC_RESULT_RETRY_LOGIN 7

// The answer to Game Sync's account.playstatus request
typedef struct {
    u16 status;
    u16 species;
    u16 unk4;
    u8 form;
    u8 sex;
    u16 items[20];
    u8 itemCounts[20];
} GSyncAccountInfo;

// Game Sync's event work, which its procs also take as their parameter
typedef struct {
    GameSystem *gsys;
    GameData *gameData;
    SaveControl *save;
    GSyncAccountInfo accountInfo;
    BOOL hasAccountInfo;
    u8 loginBuffer[0x174];
    u32 gsyncResult;
    BOOL bgmPushed;
    u16 boxTray;
    u16 boxPosition;
    u32 bgm;
    // A flag per species: set if it may be uploaded to the Dream World
    u8 sendableSpecies[0x80];
    WifiLoginParam login;
    WifiLogoutParam logout;
    Box2Param box;
    // Whether the menu comes back from the Wi-Fi settings
    BOOL fromWifiSettings;
} EventGameSync;

// pokebw2's GSYNC_MENU_PROC_FUNCTIONS, the Game Sync menu: Game Sync, the Wi-Fi settings or back
extern const GameProcFunctions data_ov123_021da674;
// pokebw2's GSYNC_PROC_FUNCTIONS, Game Sync itself
extern const GameProcFunctions data_ov123_021da838;

// Black 2's; twinned code confirms the offsets of disconnected in Black
// PDWACC_PROC_FUNCTIONS's parameter, from the start menu's Game Sync settings
struct PdwAccParam {
    GameData *gameData;
    void *loginBuffer;
    HeapID heapId;
    // PDWACC_MODE_*
    u32 mode;
    // Whether the connection was lost
    BOOL disconnected;
};

// Black 2's, not checked against Black
// Whether the account screens create the account or only show the Game Sync ID
#define PDWACC_MODE_CREATE 0

// Black 2's, not checked against Black
#define PDWACC_MODE_SHOW_ID 1

#endif // BWPORT_APP_GSYNC_H
