#ifndef POKEHEARTGOLD_OVERLAY_72_WFC_CONNECT_H
#define POKEHEARTGOLD_OVERLAY_72_WFC_CONNECT_H

#include <dwc.h>

#include "global.h"
#include "bg_window.h"
#include "g2d_renderer.h"
#include "message_format.h"
#include "msgdata.h"
#include "overlay_72.h"
#include "overlay_72_wfc_state.h"
#include "pm_string.h"
#include "render_window.h"
#include "sprite.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "yes_no_prompt.h"

typedef struct FrontierWifiUploadRecord {
    u8 team[0xA8];
    u16 *playerName;
    u8 unkAC[0xC];
    u8 gameVersion;
    u8 gameLanguage;
    u8 playerCountry;
    u8 playerRegion;
    u32 trainerId;
    u8 unkC0[0x8];
    u8 unkC8_0 : 1;
    u8 playerGender : 1;
    u8 unkC8_2 : 6;
    u8 trainerClass;
    u8 towerMessages[0x18];
    u16 unkE2;
} FrontierWifiUploadRecord;

typedef struct BattleRoomTrainer {
    u8 team[0xA8];
    u16 name[16];
    u8 useDefaultName : 1, gender : 1, pad : 6;
    u8 trainerClass;
    u8 messages[24];
    u16 score;
} BattleRoomTrainer;

typedef struct BattleRoomLeader {
    u16 name[7 + 1];
    u8 padding_10[2];
    u8 country;
    u8 region;
    u8 padding_14[4];
    u8 greeting[8];
    u8 useDefaultName : 1;
    u8 gender : 1;
    u8 unk_20_2 : 6;
    u8 unk_21;
} BattleRoomLeader;

typedef struct BattleRoomDownloadData {
    BattleRoomTrainer trainers[7];
    BattleRoomLeader leaders[30];
} BattleRoomDownloadData;

typedef struct {
    u8 version;
    u8 language;
    u8 country;
    u8 region;
    u32 otId;
    u16 name[7 + 1];
    u32 unk18;
    u8 macAddress[0x8];
    char wiiAddress[0x33];
    u8 unk57[0x5];
    u32 wiiNotificationsEnabled;
    u16 registrationCodePrefix;
    u16 registrationCode;
} WifiServerProfile;

typedef struct NintendoWfcPaletteCycle {
    SysTask *task;
    BOOL active;
    u16 basePltt[64];
    u16 cyclePltt[21 * 16];
    s16 index;
    s8 direction;
    u8 tick;
    u32 screens;
} NintendoWfcPaletteCycle;

typedef struct NintendoWfcYesNoMenu {
    BgConfig *bgConfig;
    Sprite *cursorSprite;
    Window yesWindow;
    Window noWindow;
    enum HeapID heapID;
    u8 initialChoice;
    u8 state;
    u8 bgPltt;
    u8 bgLayer;
    u8 fontPltt;
    u8 baseTile;
    u8 textBgLayer;
    u8 choice;
    int cursorOffsetY;
} NintendoWfcYesNoMenu;


typedef struct NintendoWfcNumberInput {
    u8 unk_0;
    u8 maxValue;
    s8 value;
    u8 cursor;
    s16 digitPos;
    u8 state;
    u8 nextState;
    s8 digits[3];
    u8 unk_B;
} NintendoWfcNumberInput;

typedef struct NintendoWfcButtonFlash {
    u8 isBgRect;
    u8 state;
    u8 target;
    u8 timer;
    u8 width;
    u8 height;
    u8 x;
    u8 y;
    u16 pressedPltt;
    u16 normalPltt;
} NintendoWfcButtonFlash;

typedef struct NintendoWfcConnect {
    NintendoWifiConnectArgs *args;
    BgConfig *bgConfig;
    u8 padding_0008[8];
    int screen;
    int nextScreen;
    int unk_0018;
    enum NintendoWfcState state;
    enum NintendoWfcState nextState;
    void *heapMem;
    NNSFndHeapHandle expHeap;
    DWCInetControl inetControl;
    u32 ownRank;
    u32 rank;
    int roomCount;
    u32 room;
    BattleRoomDownloadData downloadBuffer;
    FrontierWifiUploadRecord uploadRecord;
    u8 padding_0BBC[0x14];
    MessageFormat *msgFormat;
    MsgData *msgDataTower;
    MsgData *msgDataWfc;
    MsgData *msgDataError;
    String *msgString;
    String *titleString;
    String *roomString;
    String *rankString;
    String *errorString;
    u32 textPrinterId;
    SpriteList *spriteList;
    G2dRenderer renderer;
    GF_2DGfxResMan *resMan[4];
    SpriteResource *resObjs[12];
    SpriteResourcesHeader spriteHeaders[3];
    u8 padding_0DD0[0x24];
    Sprite *sprites[9];
    Window msgWindow;
    Window rankWindow;
    Window roomWindow;
    Window titleWindow;
    Window errorWindow;
    u8 padding_0E68[0xE4];
    WaitingIcon *waitingIcon;
    int dwcError;
    int dwcErrorCode;
    int dwcErrorType;
    int httpResult;
    int messageTimer;
    WifiServerProfile profile;
    u32 profileResponse[2];
    s16 subState;
    s16 subTimer;
    int timeoutCounter;
    NintendoWfcPaletteCycle paletteCycle;
    YesNoPrompt *yesNoPrompt;
    NintendoWfcNumberInput numberInput;
    Window numberInputWindow;
    Window buttonWindows[3];
    NintendoWfcButtonFlash buttonFlash;
    u16 padColumn[2];
    u32 twoDigits;
    NintendoWfcYesNoMenu yesNoMenu;
} NintendoWfcConnect;

#endif // POKEHEARTGOLD_OVERLAY_72_WFC_CONNECT_H
