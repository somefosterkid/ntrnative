// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov173/wifi_p2pmatch.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/ui/touchbar.h"
#include "app/wifi_p2pmatch.h"
#include "app/wifi_p2pmatch_net.h"
#include "app/wifi_p2pmatchroom.h"
#include "app/wifi_status.h"
#include "app/wificlub.h"
#include "battle/regulation.h"
#include "constants/arc.h"
#include "constants/language.h"
#include "constants/pokemon.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/text_banks.h"
#include "constants/version.h"
#include "field/union_msg.h"
#include "field/unity_tower.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/bmp.h"
#include "gfl/bmpwin.h"
#include "gfl/button_man.h"
#include "gfl/clact.h"
#include "gfl/dwc_rap.h"
#include "gfl/dwc_rapcommon.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/key.h"
#include "gfl/msg.h"
#include "gfl/net.h"
#include "gfl/net_handle.h"
#include "gfl/net_state.h"
#include "gfl/proc.h"
#include "gfl/random.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/str.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "nitro/gx.h"
#include "nitro/os.h"
#include "nnsys/g2d.h"
#include "pml/poke_graphic.h"
#include "pml/poke_party.h"
#include "save/box.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "save/trainer_data_savedata.h"
#include "save/wifi_list.h"
#include "struct_decls.h"
#include "system/app_keycursor.h"
#include "system/app_printsys_common.h"
#include "system/bmp_menu.h"
#include "system/bmp_menulist.h"
#include "system/bmp_menuwork.h"
#include "system/bmp_winframe.h"
#include "system/dsi.h"
#include "system/game_data.h"
#include "system/gf_font.h"
#include "system/net_define.h"
#include "system/printsys.h"
#include "system/text_speed.h"
#include "system/time_icon.h"
#include "system/union_view.h"
#include "system/wipe.h"
#include "system/wordset.h"
#include "text/system/0416.h"
#include "text/system/wifi_error.h"
#include "field/beacon_view.h"

// The Wi-Fi Club's match board: the room where the player meets the friends who are online, the friend list of the
// lower screen, and the states of calling a friend, of choosing what to play and of leaving for the game. The ROM has
// no name for this file: it is named after the older games' wifi_p2pmatch.c, and its names are ours

#define WIFIP2PMATCH_HEAP 0x21
// How many friends the friend list holds
#define WIFIP2PMATCH_MEMBER_MAX 32
// The characters of the room: the friends and the player
#define WIFIP2PMATCH_OBJ_MAX 33
#define WIFIP2PMATCH_MENU_CURSOR_NUM 6
// How many friends a page of the lower screen shows, in two columns
#define WIFIP2PMATCH_VIEW_FRIEND_NUM 8

// The status a player shows the friends (func_ov099_021c27d0)
enum {
    // No status to show
    WIFIP2PMATCH_STATUS_NONE,
    WIFIP2PMATCH_STATUS_WAIT,
    // Waits for a friend to join the game mode
    WIFIP2PMATCH_STATUS_RECRUIT,
    WIFIP2PMATCH_STATUS_CALL,
    WIFIP2PMATCH_STATUS_PLAYING,
};

// The game modes of a status (func_ov099_021c27d8) that have a name here. The eight battles follow
#define WIFIP2PMATCH_GAMEMODE_BATTLE 7
// WIFIP2PMATCH_GAMEMODE_BATTLE, by battle mode and rule
#define WIFIP2PMATCH_GAMEMODE_VCHAT 4
#define WIFIP2PMATCH_GAMEMODE_TVT 5
#define WIFIP2PMATCH_GAMEMODE_TRADE 6
#define WIFIP2PMATCH_GAMEMODE_MAX 15

// The timing number both sides wait for before the state they agreed on
#define WIFIP2PMATCH_TIMING_SEQ 0x1b

// The list menus, which index of WIFIP2PMATCH_WORK.menuCursor keeps their cursor
enum {
    WIFIP2PMATCH_MENU_BATTLE_RULES,
    WIFIP2PMATCH_MENU_BATTLE_MODE,
    WIFIP2PMATCH_MENU_BATTLE_RULE,
    WIFIP2PMATCH_MENU_SHOOTER,
    WIFIP2PMATCH_MENU_STATUS,
    // Opens lower on the screen, under the party window
    WIFIP2PMATCH_MENU_PARTY,
};

// What func_ov099_021bbeb4 prints above the rules
enum {
    WIFIP2PMATCH_RULES_BATTLE_TYPE,
    // Nothing, in a window two rows shorter
    WIFIP2PMATCH_RULES_SHORT,
    WIFIP2PMATCH_RULES_PARTY,
    WIFIP2PMATCH_RULES_BATTLE_BOX,
};

// What a button of the lower screen shows (the column of its screen data)
enum {
    WIFIP2PMATCH_VIEW_BUTTON_MALE,
    WIFIP2PMATCH_VIEW_BUTTON_FEMALE,
    WIFIP2PMATCH_VIEW_BUTTON_NONE,
};

// The states, which are indices of data_ov099_021c3c0c
enum {
    WIFIP2PMATCH_SEQ_INIT,
    WIFIP2PMATCH_SEQ_RECONNECT_KEY_WAIT,
    WIFIP2PMATCH_SEQ_RECONNECT_YES_NO_START,
    WIFIP2PMATCH_SEQ_RECONNECT_YES_NO,
    WIFIP2PMATCH_SEQ_CONNECT_START,
    WIFIP2PMATCH_SEQ_BG_PRIORITY,
    WIFIP2PMATCH_SEQ_CONNECT_WAIT,
    WIFIP2PMATCH_SEQ_USER_INFO_NOTICE,
    WIFIP2PMATCH_SEQ_USER_INFO_NOTICE_KEY_WAIT,
    WIFIP2PMATCH_SEQ_LOGIN_WAIT,
    WIFIP2PMATCH_SEQ_ROOM_START,
    WIFIP2PMATCH_SEQ_ROOM,
    WIFIP2PMATCH_SEQ_VCHAT_TOGGLE_KEY_WAIT,
    WIFIP2PMATCH_SEQ_VCHAT_CALL_WAIT,
    WIFIP2PMATCH_SEQ_VCHAT_INVITED,
    WIFIP2PMATCH_SEQ_VCHAT_INVITED_WAIT,
    WIFIP2PMATCH_SEQ_VCHAT_START,
    WIFIP2PMATCH_SEQ_VCHAT_END_YES_NO_START,
    WIFIP2PMATCH_SEQ_VCHAT_END_YES_NO,
    WIFIP2PMATCH_SEQ_CONNECT_FAILED,
    WIFIP2PMATCH_SEQ_CONNECT_FAILED_KEY_WAIT,
    WIFIP2PMATCH_SEQ_CONNECT_ERROR,
    WIFIP2PMATCH_SEQ_NONE16,
    WIFIP2PMATCH_SEQ_NONE17,
    WIFIP2PMATCH_SEQ_NONE18,
    WIFIP2PMATCH_SEQ_RETURN_TO_ROOM,
    WIFIP2PMATCH_SEQ_END,
    WIFIP2PMATCH_SEQ_NO_FRIEND_KEY_WAIT,
    WIFIP2PMATCH_SEQ_RECRUIT_MENU_START,
    WIFIP2PMATCH_SEQ_RECRUIT_MENU,
    WIFIP2PMATCH_SEQ_RECRUIT_MENU_OLD,
    WIFIP2PMATCH_SEQ_RECRUIT_STOP_ASK,
    WIFIP2PMATCH_SEQ_RECRUIT_STOP_YES_NO_START,
    WIFIP2PMATCH_SEQ_RECRUIT_STOP_YES_NO,
    WIFIP2PMATCH_SEQ_FRIEND_SELECT,
    WIFIP2PMATCH_SEQ_FRIEND_MENU_INIT,
    WIFIP2PMATCH_SEQ_RECRUITING_FRIEND_MENU,
    WIFIP2PMATCH_SEQ_JOIN_WAIT,
    WIFIP2PMATCH_SEQ_DUMMY26,
    WIFIP2PMATCH_SEQ_DUMMY27,
    WIFIP2PMATCH_SEQ_FRIEND_CONNECTED,
    WIFIP2PMATCH_SEQ_NEGOTIATION_WAIT,
    WIFIP2PMATCH_SEQ_SEND_PLAYER_INFO,
    WIFIP2PMATCH_SEQ_CONNECT_DATA_WAIT,
    WIFIP2PMATCH_SEQ_CONNECT_TIMING,
    WIFIP2PMATCH_SEQ_CONNECT_END,
    WIFIP2PMATCH_SEQ_FRIEND_DETAIL_INIT,
    WIFIP2PMATCH_SEQ_FRIEND_DETAIL_WAIT,
    WIFIP2PMATCH_SEQ_FRIEND_DETAIL_END,
    WIFIP2PMATCH_SEQ_EXIT_YES_NO_INIT,
    WIFIP2PMATCH_SEQ_EXIT_YES_NO,
    WIFIP2PMATCH_SEQ_DISCONNECT,
    WIFIP2PMATCH_SEQ_DISCONNECT_END,
    WIFIP2PMATCH_SEQ_RETRY_YES_NO_INIT,
    WIFIP2PMATCH_SEQ_RETRY_YES_NO,
    WIFIP2PMATCH_SEQ_VCHAT_CHANGE_YES_NO_INIT,
    WIFIP2PMATCH_SEQ_VCHAT_CHANGE_YES_NO,
    WIFIP2PMATCH_SEQ_DISCONNECT_WAIT,
    WIFIP2PMATCH_SEQ_DUMMY3_A,
    WIFIP2PMATCH_SEQ_DUMMY3_B,
    WIFIP2PMATCH_SEQ_USER_DATA_SAVE_START,
    WIFIP2PMATCH_SEQ_JOIN_TIMEOUT,
    WIFIP2PMATCH_SEQ_USER_DATA_SAVE,
    WIFIP2PMATCH_SEQ_VCHAT_CALL_START,
    WIFIP2PMATCH_SEQ_WAITING_FRIEND_MENU,
    WIFIP2PMATCH_SEQ_CONNECTED,
    WIFIP2PMATCH_SEQ_PLAY_YES_NO_START,
    WIFIP2PMATCH_SEQ_PLAY_YES_NO,
    WIFIP2PMATCH_SEQ_GAME_MODE_MESSAGE,
    WIFIP2PMATCH_SEQ_GAME_MODE_MENU_OPEN,
    WIFIP2PMATCH_SEQ_GAME_MODE_MENU,
    WIFIP2PMATCH_SEQ_GAME_MODE_KEY_WAIT,
    WIFIP2PMATCH_SEQ_SYNC_SEQ_WAIT,
    WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT,
    WIFIP2PMATCH_SEQ_CANCEL_RESET,
    WIFIP2PMATCH_SEQ_ASK_VCHAT,
    WIFIP2PMATCH_SEQ_ASK_TVT,
    WIFIP2PMATCH_SEQ_ASK_TRADE,
    WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START,
    WIFIP2PMATCH_SEQ_ANSWER_YES_NO_START,
    WIFIP2PMATCH_SEQ_ANSWER_YES_NO,
    WIFIP2PMATCH_SEQ_START_GAME,
    WIFIP2PMATCH_SEQ_SEND_START_GAME,
    WIFIP2PMATCH_SEQ_SEND_CANCEL_RESET,
    WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU,
    WIFIP2PMATCH_SEQ_BATTLE_MODE_MENU_START,
    WIFIP2PMATCH_SEQ_BATTLE_MODE_MENU,
    WIFIP2PMATCH_SEQ_BATTLE_RULE_MENU_START,
    WIFIP2PMATCH_SEQ_BATTLE_RULE_MENU,
    WIFIP2PMATCH_SEQ_SHOOTER_MENU_START,
    WIFIP2PMATCH_SEQ_SHOOTER_MENU,
    WIFIP2PMATCH_SEQ_BATTLE_SETUP_DECIDE,
    WIFIP2PMATCH_SEQ_RULES_SHOW,
    WIFIP2PMATCH_SEQ_RULES_SHOW_WAIT,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_SHOW,
    WIFIP2PMATCH_SEQ_UNUSED5_F,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_YES_NO_START,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_YES_NO,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_CHECK_PARTY,
    WIFIP2PMATCH_SEQ_REGULATION_FAIL_PARTY,
    WIFIP2PMATCH_SEQ_PARTY_SELECT_START,
    WIFIP2PMATCH_SEQ_PARTY_MENU_START,
    WIFIP2PMATCH_SEQ_PARTY_MENU,
    WIFIP2PMATCH_SEQ_PARTY_SEND,
    WIFIP2PMATCH_SEQ_PARTY_SEND_WAIT,
    WIFIP2PMATCH_SEQ_BATTLE_START_WAIT,
    WIFIP2PMATCH_SEQ_BATTLE_START,
    WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_PARTY,
    WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_BATTLE_BOX,
    WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_END,
    WIFIP2PMATCH_SEQ_PLAY_AGAIN_ASK,
    WIFIP2PMATCH_SEQ_PLAY_AGAIN_YES_NO_START,
    WIFIP2PMATCH_SEQ_PLAY_AGAIN_YES_NO,
    WIFIP2PMATCH_SEQ_MESSAGE_KEY_WAIT_TO_ROOM,
    WIFIP2PMATCH_SEQ_BACK_TO_ROOM,
    WIFIP2PMATCH_SEQ_GAME_START,
    WIFIP2PMATCH_SEQ_GAME_START_FAIL_KEY_WAIT,
    WIFIP2PMATCH_SEQ_BATTLE_RECRUIT_START,
    WIFIP2PMATCH_SEQ_JOIN_FRIEND,
    WIFIP2PMATCH_SEQ_REGULATION_FAIL_BATTLE_BOX,
    WIFIP2PMATCH_SEQ_REGULATION_FAIL_END,
    WIFIP2PMATCH_SEQ_RETURN_FROM_GAME,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_MSG_WAIT,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_KEY_WAIT,
    WIFIP2PMATCH_SEQ_BATTLE_OFFER_YES_NO_GO,
    WIFIP2PMATCH_SEQ_VCHAT_DIFF_MSG,
    WIFIP2PMATCH_SEQ_VCHAT_DIFF_YES_NO_START,
    WIFIP2PMATCH_SEQ_VCHAT_DIFF_YES_NO,
    WIFIP2PMATCH_SEQ_VCHAT_SEND,
    WIFIP2PMATCH_SEQ_REGULATION_SEND,
    WIFIP2PMATCH_SEQ_REGULATION_SEND_END,
    WIFIP2PMATCH_SEQ_RECRUIT_MENU_MESSAGE,
    WIFIP2PMATCH_SEQ_REFUSED_MSG,
    WIFIP2PMATCH_SEQ_SEND_REFUSED_MSG,
    WIFIP2PMATCH_SEQ_VCHAT_DIFF_NO,
    WIFIP2PMATCH_SEQ_SEND_CONNECT_DATA,
    WIFIP2PMATCH_SEQ_CALL_WAIT,
    WIFIP2PMATCH_SEQ_CALL_CANCEL_YES_NO,
    WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING,
    WIFIP2PMATCH_SEQ_SEND_SEQ,
    WIFIP2PMATCH_SEQ_PLAY_AGAIN_KEY_WAIT,
    WIFIP2PMATCH_SEQ_GO_DISCONNECT,
    WIFIP2PMATCH_SEQ_DISCONNECT_START,
    WIFIP2PMATCH_SEQ_DECLINED_WAIT,
    WIFIP2PMATCH_SEQ_DECLINED,
    WIFIP2PMATCH_SEQ_CUT_CONNECTION,
    WIFIP2PMATCH_SEQ_VCHAT_END_SEND,
    WIFIP2PMATCH_SEQ_VCHAT_END_TIMING,
    WIFIP2PMATCH_SEQ_VCHAT_ENDED_MESSAGE,
    WIFIP2PMATCH_SEQ_VCHAT,
    WIFIP2PMATCH_SEQ_TRADE_REFUSED_KEY_WAIT,
    WIFIP2PMATCH_SEQ_UNABLE_MSG,
    WIFIP2PMATCH_SEQ_PARTY_SEND_TIMING,
    WIFIP2PMATCH_SEQ_CANCEL_MSG,
    WIFIP2PMATCH_SEQ_VCHAT_STOP_WAIT,
    WIFIP2PMATCH_SEQ_TALK_END_TIMING,
    WIFIP2PMATCH_SEQ_TALK_END_TIMING_WAIT,
    WIFIP2PMATCH_SEQ_REGULATION_FAIL_KEY_WAIT,
    WIFIP2PMATCH_SEQ_ROOM_MESSAGE,
    WIFIP2PMATCH_SEQ_VCHAT_DISCONNECT_TIMING,
    WIFIP2PMATCH_SEQ_CAMERA_NOTICE_YES_NO_START,
    WIFIP2PMATCH_SEQ_CAMERA_NOTICE_YES_NO,
    WIFIP2PMATCH_SEQ_CONNECT_FRIEND_YES_NO_START,
    WIFIP2PMATCH_SEQ_CONNECT_FRIEND_YES_NO,
    WIFIP2PMATCH_SEQ_NO_CAMERA_YES_NO_START,
    WIFIP2PMATCH_SEQ_NO_CAMERA_YES_NO,
    WIFIP2PMATCH_SEQ_ANSWER_NO_CAMERA_YES_NO_START,
    WIFIP2PMATCH_SEQ_ANSWER_NO_CAMERA_YES_NO,
    WIFIP2PMATCH_SEQ_DECLINE_RESET,
    WIFIP2PMATCH_SEQ_DECLINE_MSG,
    WIFIP2PMATCH_SEQ_RETURN_YES_NO_START,
    WIFIP2PMATCH_SEQ_RETURN_YES_NO,
    WIFIP2PMATCH_SEQ_RETURN_TIMING_WAIT,
    WIFIP2PMATCH_SEQ_RECRUIT_START,
    WIFIP2PMATCH_SEQ_MAX = 0xb5,
};

// An option of a list menu: its message in the match board's bank and its value
// Black 2's, not checked against Black
typedef struct {
    u32 msg;
    s32 value;
} WIFIP2PMATCH_MENU_OPTION;

// What the child sends the parent when the connection is made (command 0xa03)
typedef struct {
    u16 gameMode;
    // Whether the sender uses voice chat
    u16 vchat;
    u32 regulationCode;
} WIFIP2PMATCH_CONNECT_DATA;

// The graphics of the status icons
typedef struct {
    void *scrnBuf;
    NNSG2dScreenData *scrn;
    void *charBuf;
    NNSG2dCharacterData *chr;
    // The icons' characters on BG 1 and, for the lower screen, on BG 6
    u32 bg1Chars;
    u32 bg6Chars;
} WIFIP2PMATCH_ICON;

// The friend list of the lower screen: a page of eight buttons, and a friend's details when a button is touched
// Black 2's; twinned code confirms the offsets of wordSet, buttonScrnBuf, buttonScrn, backScrnBuf, backScrn, buttonMan,
// friendState, page, touchFriendNo, buttonFrame, detailMode, redrawButtons, buttonAnimCount, lastTouchFriendNo,
// redrawAll, nameWin, statusWin and detailWin in Black
typedef struct {
    WordSet *wordSet;
    void *buttonScrnBuf;
    NNSG2dScreenData *buttonScrn;
    void *backScrnBuf;
    NNSG2dScreenData *backScrn;
    u32 unk14;
    // NULL while the view does not exist
    void *buttonMan;
    // What is known of each friend, by friend number - 1: 0 nothing, 1 registered, 2 in the room
    u8 friendState[WIFIP2PMATCH_MEMBER_MAX];
    u8 page;
    // The friend number of the button that was touched, 0 if none
    u8 touchFriendNo;
    u8 buttonFrame;
    // 0 for the list, 1 when a friend's details were asked for, 2 and 3 while they are shown
    u8 detailMode;
    u8 unk40;
    u8 redrawButtons;
    u8 buttonAnimCount;
    u8 lastTouchFriendNo;
    BOOL redrawAll;
    BmpWin *nameWin[WIFIP2PMATCH_VIEW_FRIEND_NUM];
    BmpWin *statusWin[WIFIP2PMATCH_VIEW_FRIEND_NUM][2];
    BmpWin *detailWin[2];
    u8 unkB0[0x44];
} WIFIP2PMATCH_VIEW;

// Black 2's; twinned code confirms the offsets of vblankTcb, wifiList, myStatus, index2No, index2NoBackup, matchStatus,
// matchGameMode, matchVChat, subMenuOptions, subList, menuOptions, list, save, gameData, party, battleBoxParty,
// wordSet, msgData, sysMsgData, font, smallFont, expStrBuf, talkString, titleString, unk0FC, waitIcon, yesNoMenu,
// clActUnit, clActRenderer, pokeIconChars, pokeIconPalette, pokeIconCellAnims, pokeIcons, trainerActor, trainerChars,
// trainerPalette, trainerCellAnims, msgWin, myInfoWin, titleWin, sysMsgWin, subWin, listWin, unk1A8, listPrintWindow,
// listPrintQueue, printQueue, printStream, tcbManager, callTimer, seq, endSeq, timer, unk1E8, preConnect,
// connectGameMode, menuCursor, menuCursorType, friendNo, unk244, unk246, parent, targetStatus, printCommon, room,
// moveObj, icon, view, viewBgChars, touchBar, keyCursor, roomPaused, friendNum, modeSeq, listFrameChars,
// talkFrameChars, subFrameChars, bgmVolume, bgmVolumeStart, bgmVolumeTarget, bgmFadeFrames, regulationPrintMsg,
// regulation, battleMode, battleRule, unk1350, unk1351, unk1352, sendSeq, unk1354, unk1355, unk1356, recvSeq and
// unk1358 in Black
typedef struct {
    TCB *vblankTcb;
    WifiList *wifiList;
    // The player's status, which event_wificlub.c keeps (WifiClubData.buffer)
    WIFI_STATUS *myStatus;
    u8 unk00C[4];
    // The friend numbers (index + 1) of the friends who are in the room, packed from the start and ended by 0, and
    // what it was a frame ago
    u8 index2No[WIFIP2PMATCH_MEMBER_MAX];
    u8 index2NoBackup[WIFIP2PMATCH_MEMBER_MAX];
    // What each friend's status last said
    u8 matchStatus[WIFIP2PMATCH_MEMBER_MAX];
    u8 matchGameMode[WIFIP2PMATCH_MEMBER_MAX];
    u8 matchVChat[WIFIP2PMATCH_MEMBER_MAX];
    // Nothing in this file creates these two: they are only freed
    ListMenuOption *subMenuOptions;
    BmpMenuList *subList;
    ListMenuOption *menuOptions;
    BmpMenuList *list;
    SaveControl *save;
    GameData *gameData;
    PokeParty *party;
    // The Battle Box as a party, NULL if it is empty
    PokeParty *battleBoxParty;
    u8 unk0D0[8];
    void *trainerData;
    WordSet *wordSet;
    MsgData *msgData;
    // The messages about Nintendo WFC
    MsgData *sysMsgData;
    Font *font;
    Font *smallFont;
    // Where a message is loaded, and where it is expanded for printing
    StrBuf *expStrBuf;
    StrBuf *talkString;
    StrBuf *titleString;
    StrBuf *unk0FC;
    WaitIcon *waitIcon;
    u32 unk104;
    BmpMenu *yesNoMenu;
    u32 unk10C;
    ClActUnit *clActUnit;
    ClActRenderer *clActRenderer;
    // The icons of the party's Pokémon, then of the Battle Box's
    u32 pokeIconChars[12];
    u32 pokeIconPalette;
    u32 pokeIconCellAnims;
    ClActor *pokeIcons[12];
    // The friend's trainer on the details page
    ClActor *trainerActor;
    u32 trainerChars;
    u32 trainerPalette;
    u32 trainerCellAnims;
    BmpWin *msgWin;
    // The player's name and status
    BmpWin *myInfoWin;
    BmpWin *titleWin;
    // The window of the messages about Nintendo WFC, and of a battle's rules
    BmpWin *sysMsgWin;
    BmpWin *subWin;
    BmpWin *listWin;
    BmpWin *unk1A8;
    PrintWindow listPrintWindow;
    PrintQueue *listPrintQueue;
    PrintQueue *printQueue;
    PrintStream *printStream;
    TCBExManager *tcbManager;
    // The frames left for a called friend to answer
    s32 callTimer;
    // The frames the player has been in the room since the last connection
    s32 unk1C8;
    // The state: an index of data_ov099_021c3c0c
    int seq;
    // What the proc returns to event_wificlub.c (WifiClubData.mode)
    u32 endSeq;
    u32 unk1D4;
    u32 initMode;
    s32 unk1DC;
    s32 timer;
    u32 unk1E4;
    u32 unk1E8;
    u32 unk1EC;
    // The net ID of the friend who is connecting, -1 if none
    s32 preConnect;
    // The game mode each machine sent on connecting, by net ID
    u16 connectGameMode[0x20];
    // Each list menu's cursor
    u16 menuCursor[WIFIP2PMATCH_MENU_CURSOR_NUM];
    u16 menuCursorType;
    // The friend number (index + 1) of the partner
    u16 friendNo;
    u16 unk244;
    // What the player chose in the game mode menu
    u16 unk246;
    WifiClubData *parent;
    // A copy of the partner's status
    WIFI_STATUS targetStatus;
    AppPrintsysCommon printCommon;
    WIFI_MATCHROOM room;
    MCR_MOVEOBJ *moveObj[WIFIP2PMATCH_OBJ_MAX];
    WIFIP2PMATCH_ICON icon;
    WIFIP2PMATCH_VIEW view;
    u32 unk9A4;
    u32 viewBgChars;
    TouchBar *touchBar;
    KeyCursor *keyCursor;
    // Whether the player's character is kept from moving
    BOOL roomPaused;
    // Nothing in this file reaches these bytes
    u8 unk9B8[0x970];
    u16 unk1328;
    // How many friends the friend list has
    u16 friendNum;
    // Which of the three states that ask for a game mode ran
    u16 modeSeq;
    u16 unk132E;
    u32 listFrameChars;
    u32 talkFrameChars;
    u32 subFrameChars;
    u32 unk133C;
    // The fade of the music's volume
    u8 bgmVolume;
    u8 bgmVolumeStart;
    u8 bgmVolumeTarget;
    u8 bgmFadeFrames;
    RegulationPrintMsg *regulationPrintMsg;
    Regulation *regulation;
    // The battle chosen: single, double, triple or rotation, and no restrictions or flat
    u16 battleMode;
    u16 battleRule;
    u8 unk1350;
    u8 unk1351;
    u8 unk1352;
    // The state both sides go to next, which state 0x8b sends, and the one the partner sent
    u8 sendSeq;
    u8 unk1354;
    u8 unk1355;
    u8 unk1356;
    u8 recvSeq;
    u8 unk1358;
    u8 unk1359;
    // Black never writes it (Black 2 does)
    u8 unk135A;
    u8 unk135B;
    // How the connection was made: 3 and 4 for the two sides of a pair that chooses what to play
    u32 result;
} WIFIP2PMATCH_WORK;

typedef int (*WifiP2PMatchSeqFunc)(WIFIP2PMATCH_WORK *wk, int seq);

static void func_ov099_021ba828(WIFIP2PMATCH_WORK *wk);
static u32 func_ov099_021ba858(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021ba874(WIFIP2PMATCH_WORK *wk, int friendIndex);
static BmpWin *func_ov099_021ba8bc(BmpWin *window);
static void func_ov099_021ba8d4(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021ba8ec(WIFIP2PMATCH_WORK *wk);
static BOOL func_ov099_021ba928(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021ba978(WIFIP2PMATCH_WORK *wk, u32 msgId, BOOL sysMsg, BOOL stream);
static void func_ov099_021baab8(WIFIP2PMATCH_WORK *wk, u32 msgId, BOOL sysMsg);
static void func_ov099_021baadc(WIFIP2PMATCH_WORK *wk, u32 msgId, BOOL sysMsg);
static void func_ov099_021bab04(WIFIP2PMATCH_WORK *wk, u32 msgId);
static void func_ov099_021babdc(BmpMenuList *list, intptr_t value, u8 y);
static void func_ov099_021bac24(WIFIP2PMATCH_WORK *wk, BmpMenuListHeader *header,
                                      WIFIP2PMATCH_MENU_OPTION *options, int count, int type, int width);
static void func_ov099_021bad50(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bad74(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021baedc(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb050(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb0b4(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb1a8(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb224(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb2b0(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021bb2f0(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021bb374(WIFIP2PMATCH_WORK *wk, int friendIndex, u32 heapId);
static void func_ov099_021bb42c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb460(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021bb4d0(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb4ec(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb528(WIFIP2PMATCH_WORK *wk, u32 heapId);
static int func_ov099_021bb918(u32 status, u32 gameMode, u32 *color);
void func_ov099_021bb954(WIFIP2PMATCH_WORK *wk, BOOL fromParent);
static void func_ov099_021bb9c8(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bb9f4(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bba20(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bba4c(WIFIP2PMATCH_WORK *wk, PokeParty *party);
static void func_ov099_021bbc0c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bbc34(WIFIP2PMATCH_WORK *wk, PokeParty *party, int base, ArcTool *arc);
static void func_ov099_021bbd0c(WIFIP2PMATCH_WORK *wk, PokeParty *party, PokeParty *battleBox);
static void func_ov099_021bbd80(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bbdd0(WIFIP2PMATCH_WORK *wk, BOOL first);
static void func_ov099_021bbe50(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bbe7c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bbeb4(WIFIP2PMATCH_WORK *wk, u32 failBits, BOOL shooter, int kind);
static void func_ov099_021bc06c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bc090(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bc0c4(WIFIP2PMATCH_WORK *wk);
void func_ov099_021bc104(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021bc14c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc1c4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc1e8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc228(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc29c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc2c0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc43c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc468(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc4a8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc4d4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc51c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc544(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc5c0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc5d4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc5e8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc5fc(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc620(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc6b0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc6f4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc73c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc76c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc784(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc79c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc7b4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc850(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc868(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc940(WIFIP2PMATCH_WORK *wk, int seq);
int func_ov099_021bc958(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bc9fc(WIFIP2PMATCH_WORK *wk, int seq);
int func_ov099_021bca14(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcab8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcad0(WIFIP2PMATCH_WORK *wk, int seq);
static u32 func_ov099_021bcb70(Regulation *regulation, GameData *gameData, u32 *flags);
static PokeParty *func_ov099_021bcb8c(GameData *gameData);
static void func_ov099_021bcbb0(PokeParty *party);
static u32 func_ov099_021bcbbc(Regulation *regulation, PokeParty *party, u32 *flags);
u32 func_ov099_021bcbd8(WIFIP2PMATCH_WORK *wk, int which);
static u32 func_ov099_021bcc4c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bcc58(WIFIP2PMATCH_WORK *wk, u32 regulationCode);
static int func_ov099_021bccb8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcd58(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcdb0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bce30(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bce80(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bceac(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcecc(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcf4c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcf6c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcf8c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcf98(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcf9c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bcfb4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd014(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd038(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd0bc(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd0f0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd15c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd1a4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd318(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd380(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd3a8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd41c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd444(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd4a8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd500(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd558(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd584(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd5d8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd5fc(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd63c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd678(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd6a0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd6d0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd71c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd744(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd778(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd7a0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd7d4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd7e0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd814(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd838(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd864(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd888(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd8c0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd8d8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd924(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd97c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd994(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd9b4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bd9f4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bda3c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bdaa4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bdac8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bdb08(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bdb30(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bdc0c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bdc44(WIFIP2PMATCH_WORK *wk, int seq);
static DWCUserData *func_ov099_021bdcd0(void *work);
static DWCFriendData *func_ov099_021bdce0(void *work);
static void func_ov099_021bdcf4(int code, int param, void *work);
static void func_ov099_021bdd0c(WIFIP2PMATCH_WORK *wk, BOOL init);
static u32 func_ov099_021bdd48(WIFIP2PMATCH_WORK *wk, const WIFI_STATUS *status);
static u32 func_ov099_021bdd54(WIFIP2PMATCH_WORK *wk, const WIFI_STATUS *status);
static BOOL func_ov099_021bdd60(u32 gameMode);
static BOOL func_ov099_021bdd98(int gameMode);
static BOOL func_ov099_021bdda8(u32 status);
static BOOL func_ov099_021bddb4(u32 status, u32 gameMode);
static BOOL func_ov099_021bddd0(WIFIP2PMATCH_WORK *wk);
static BOOL func_ov099_021bddf4(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bde30(WIFIP2PMATCH_WORK *wk, u32 status, u32 gameMode);
static void func_ov099_021bde94(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bdf7c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021be028(TCB *tcb, void *data);
static void func_ov099_021be030(void);
static void func_ov099_021be040(HeapID heapId);
static void func_ov099_021be21c(WIFIP2PMATCH_WORK *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021be244(WIFIP2PMATCH_WORK *wk, MCR_MOVEOBJ *obj);
static void func_ov099_021be26c(WIFIP2PMATCH_WORK *wk);
static MCR_MOVEOBJ *func_ov099_021be2a8(WIFIP2PMATCH_WORK *wk, u32 friendNo);
static void func_ov099_021be2dc(WIFIP2PMATCH_ICON *icon, ArcTool *arc, u32 heapId);
static void func_ov099_021be384(WIFIP2PMATCH_ICON *icon);
static void func_ov099_021be3cc(WIFIP2PMATCH_ICON *icon, u32 bg, u8 x, u8 y, u32 iconNo, u32 unused);
static u32 func_ov099_021be448(u32 gameMode, u32 status);
static void func_ov099_021be4bc(WIFIP2PMATCH_ICON *icon, u32 bg, u8 x, u8 y, u32 status, u32 gameMode,
                                         u32 add);
static u32 func_ov099_021be4ec(u32 bg);
static int func_ov099_021be4f0(WIFIP2PMATCH_WORK *wk, BOOL wipe);
static void func_ov099_021be538(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021be53c(WIFIP2PMATCH_WORK *wk, ArcTool *arc);
static void func_ov099_021be5b4(void);
static void func_ov099_021be5b8(WIFIP2PMATCH_WORK *wk, ArcTool *arc);
static int func_ov099_021be600(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021be6b0(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021be6e4(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021be6f0(WIFIP2PMATCH_WORK *wk, u32 status, u32 gameMode);
static int func_ov099_021be788(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021be7ec(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021be900(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021be904(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021be94c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021be96c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021be9c8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021beaec(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021beb68(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bebe8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bec60(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bed08(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bed3c(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021beda0(WIFIP2PMATCH_WORK *wk);
static BOOL func_ov099_021bee64(int friendIndex, void *work);
static int func_ov099_021bee94(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021beebc(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021bf000(WIFIP2PMATCH_WORK *wk, int seq);
static BOOL func_ov099_021bf03c(WIFIP2PMATCH_WORK *wk);
static BOOL func_ov099_021bf050(WIFIP2PMATCH_WORK *wk);
static BOOL func_ov099_021bf068(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021bf084(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bf1cc(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021bf24c(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021bf280(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021bf29c(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021bf2c4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bf6e0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bf950(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfa00(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfa28(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfad0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfb04(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfb90(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021bfbbc(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021bfc04(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfc94(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfcec(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfd68(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfda8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfdd0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfe54(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfe9c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bfed0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bff9c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021bffe0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0078(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0134(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0138(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c013c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0140(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c017c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c01a0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c01cc(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0254(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0274(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021c0290(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021c02c8(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c047c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c04a4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c04d0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0528(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0554(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0618(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0664(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0698(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0750(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0858(WIFIP2PMATCH_WORK *wk, int seq);
static BOOL func_ov099_021c08f8(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021c09c0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0a94(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0aec(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021c0c7c(void);
static int func_ov099_021c0c88(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0e60(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c0f70(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c105c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1060(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1064(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c10a0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c10a4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c10a8(WIFIP2PMATCH_WORK *wk, int seq);
static BOOL func_ov099_021c1154(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021c11a0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1374(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c13d0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c143c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c14a8(WIFIP2PMATCH_WORK *wk, int seq);
static BOOL func_ov099_021c14ec(WIFIP2PMATCH_WORK *wk);
static int func_ov099_021c1510(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c158c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c15b4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c15c0(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c15fc(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1614(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1634(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c169c(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c16ec(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1724(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c1768(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c17d4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c17f4(WIFIP2PMATCH_WORK *wk, int seq);
static int func_ov099_021c18b4(WIFIP2PMATCH_WORK *wk, int seq);
static void func_ov099_021c18c8(WIFIP2PMATCH_WORK *wk, u32 status, int gameMode);
static void func_ov099_021c1908(WIFIP2PMATCH_WORK *wk, u32 status, u32 gameMode);
BOOL func_ov099_021c1968(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c19a8(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c19dc(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021c1a14(WIFIP2PMATCH_WORK *wk, u32 friendNo, HeapID heapId);
static void func_ov099_021c1a30(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static BOOL func_ov099_021c1a44(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c1a58(WIFIP2PMATCH_WORK *wk, ArcTool *arc, u32 heapId);
static void func_ov099_021c1ac0(WIFIP2PMATCH_WORK *wk);
u32 func_ov099_021c1b04(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021c1bec(WIFIP2PMATCH_WORK *wk);
BOOL func_ov099_021c1c10(WIFIP2PMATCH_WORK *wk);
static u32 func_ov099_021c1c64(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c1c70(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c1c9c(u32 button, u32 event, void *work);
void func_ov099_021c1cd4(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c1d1c(WIFIP2PMATCH_WORK *wk, ArcTool *arc, u32 heapId);
static void func_ov099_021c1f74(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c1fe8(NNSG2dScreenData *scrn, u16 add);
static void func_ov099_021c2028(WIFIP2PMATCH_WORK *wk, unsigned long friendNo, u32 state);
static void func_ov099_021c203c(WIFIP2PMATCH_WORK *wk, unsigned long friendNo);
static u8 func_ov099_021c2050(WIFIP2PMATCH_WORK *wk, unsigned long friendNo);
static void func_ov099_021c205c(WIFIP2PMATCH_WORK *wk, HeapID heapId);
static void func_ov099_021c207c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c208c(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c2090(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c2224(WIFIP2PMATCH_WORK *wk, u8 x, u8 y, u8 type, u8 frame);
static void func_ov099_021c2274(WIFIP2PMATCH_WORK *wk, BmpWin *window, u32 friendNo, u32 frame, u32 index);
static void func_ov099_021c22e8(WIFIP2PMATCH_WORK *wk, BmpWin **windows, u32 friendNo, u32 frame, u32 x,
                                             u32 y);
static void func_ov099_021c2370(WIFIP2PMATCH_WORK *wk, int friendIndex);
static WIFI_STATUS *func_ov099_021c23b8(int friendIndex);
static u8 func_ov099_021c23c0(int friendIndex);
static BOOL func_ov099_021c23c8(WIFIP2PMATCH_WORK *wk, int a1);
static void func_ov099_021c23d8(WIFIP2PMATCH_WORK *wk, BOOL message);
static void func_ov099_021c2408(WIFIP2PMATCH_WORK *wk);
static BOOL func_ov099_021c2438(WIFIP2PMATCH_WORK *wk);
static void func_ov099_021c24a4(WIFIP2PMATCH_WORK *wk, u8 volume);
static void func_ov099_021c24d4(WIFIP2PMATCH_WORK *wk, u32 mode);
static BOOL func_ov099_021c24f0(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov099_021c2604(GameProc *proc, u32 *state, void *param, void *work);
static BOOL func_ov099_021c2724(GameProc *proc, u32 *state, void *param, void *work);

// MWCC lays out data objects of one size by where every object of the file is defined, so the order of these
// definitions, and the places of the ones further down (each before its first user), are what gives the ROM's layout

// pokebw2's sWifiP2PMatchFriendMenuOptions
static const WIFIP2PMATCH_MENU_OPTION data_ov099_021c3730[] = {
    { Bank0416_Text_093, 1 },
    { Bank0416_Text_032, BMPMENULIST_CANCEL },
};

// pokebw2's sWifiP2PMatchFriendMenuHeader
static const BmpMenuListHeader data_ov099_021c37a0 = {
    NULL, NULL, NULL, NULL, 2, 2, 0,    16,   0,  0, 1, 15, 2, 0, 0, BMPMENULIST_SKIP_NONE, 0, BMPMENULIST_CURSOR_SHOW,
    NULL, 16, 16, NULL, NULL, NULL, NULL,
};

// The values are the states the options lead to; func_ov099_021bb954 sets the first three messages
// to the mode, the rule and the Wonder Launcher setting chosen so far
// pokebw2's sWifiP2PMatchBattleRulesOptions
static WIFIP2PMATCH_MENU_OPTION data_ov099_021c3ad0[] = {
    { Bank0416_Text_060, WIFIP2PMATCH_SEQ_BATTLE_MODE_MENU_START },
    { Bank0416_Text_064, WIFIP2PMATCH_SEQ_BATTLE_RULE_MENU_START },
    { Bank0416_Text_066, WIFIP2PMATCH_SEQ_SHOOTER_MENU_START },
    { Bank0416_Text_100, WIFIP2PMATCH_SEQ_BATTLE_SETUP_DECIDE },
    { Bank0416_Text_101, WIFIP2PMATCH_SEQ_RULES_SHOW },
    { Bank0416_Text_032, BMPMENULIST_CANCEL },
};

// pokebw2's sWifiP2PMatchBattleRuleOptions
static WIFIP2PMATCH_MENU_OPTION data_ov099_021c3a00[] = {
    { Bank0416_Text_064, 0 },
    { Bank0416_Text_065, 1 },
    { Bank0416_Text_032, BMPMENULIST_CANCEL },
};

// The values are game modes
// pokebw2's sWifiP2PMatchGameModeOptions
static WIFIP2PMATCH_MENU_OPTION data_ov099_021c3a50[] = {
    { Bank0416_Text_027, WIFIP2PMATCH_GAMEMODE_VCHAT },
    { Bank0416_Text_088, WIFIP2PMATCH_GAMEMODE_TVT },
    { Bank0416_Text_022, WIFIP2PMATCH_GAMEMODE_BATTLE },
    { Bank0416_Text_025, WIFIP2PMATCH_GAMEMODE_TRADE },
    { Bank0416_Text_032, BMPMENULIST_CANCEL },
};

static const ConfirmDialogSetup data_ov099_021c371a = { 2, 24, 13, 15, 0x115 };

// pokebw2's sWifiP2PMatchPartyMenuOptions
static WIFIP2PMATCH_MENU_OPTION data_ov099_021c39f0[] = {
    { Bank0416_Text_102, 0 },
    { Bank0416_Text_103, 1 },
};

// pokebw2's sWifiP2PMatchBattleModeOptions
static WIFIP2PMATCH_MENU_OPTION data_ov099_021c3a78[] = {
    { Bank0416_Text_060, 0 },
    { Bank0416_Text_061, 1 },
    { Bank0416_Text_062, 2 },
    { Bank0416_Text_063, 3 },
    { Bank0416_Text_032, BMPMENULIST_CANCEL },
};

// pokebw2's sWifiP2PMatchShooterOptions
static WIFIP2PMATCH_MENU_OPTION data_ov099_021c3a18[] = {
    { Bank0416_Text_066, 0 },
    { Bank0416_Text_067, 1 },
    { Bank0416_Text_032, BMPMENULIST_CANCEL },
};

// pokebw2's sWifiP2PMatchGameModeMenuHeader
static BmpMenuListHeader data_ov099_021c3b00 = {
    NULL,
    NULL,
    func_ov099_021babdc,
    NULL,
    5,
    5,
    0,
    16,
    0,
    0,
    1,
    15,
    2,
    0,
    0,
    BMPMENULIST_SKIP_NONE,
    0,
    BMPMENULIST_CURSOR_SHOW,
    NULL,
    16,
    16,
    NULL,
    NULL,
    NULL,
    NULL,
};

// The menus that use it set its count
// pokebw2's sWifiP2PMatchSubMenuHeader
static BmpMenuListHeader data_ov099_021c3b34 = {
    NULL, NULL, NULL, NULL, 5, 5, 0,    16,   0,  0, 1, 15, 2, 0, 0, BMPMENULIST_SKIP_NONE, 0, BMPMENULIST_CURSOR_SHOW,
    NULL, 16, 16, NULL, NULL, NULL, NULL,
};

// The regulation file of each battle, by battleMode + battleRule * 4
// pokebw2's sWifiP2PMatchRegulationFiles
static u32 data_ov099_021c3a30[8] = { 5, 6, 7, 8, 0x18, 0x19, 0x1a, 0x1b };

// Opens the yes/no menu; func_ov099_021ba858 gives the answer
// pokebw2's WifiP2PMatch_YesNoStart
static void func_ov099_021ba828(WIFIP2PMATCH_WORK *wk) {
    func_0201dd68();
    wk->yesNoMenu = ShopUI_CreateConfirmDialog(&data_ov099_021c371a, wk->talkFrameChars, 14, 0, WIFIP2PMATCH_HEAP);
}

// Returns 0 for yes, BMPMENU_CANCEL for no and BMPMENU_NULL while the menu is open
// pokebw2's WifiP2PMatch_YesNoMain
static u32 func_ov099_021ba858(WIFIP2PMATCH_WORK *wk) {
    u32 ret = func_0201febc(wk->yesNoMenu);

    if (ret != BMPMENU_NULL) {
        wk->yesNoMenu = NULL;
    }
    return ret;
}

// Puts a friend's name in the first word of the messages
// pokebw2's WifiP2PMatch_SetFriendNameWord
static void func_ov099_021ba874(WIFIP2PMATCH_WORK *wk, int friendIndex) {
    if (friendIndex != -1) {
        u32 gender = func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_GENDER);

        func_020099e4(wk->wifiList, friendIndex, wk->expStrBuf);
        func_0201ee0c(wk->wordSet, 0, wk->expStrBuf, gender, 1, 2);
    }
}

// Frees a window and its frame. Returns NULL, for the window's pointer
// pokebw2's WifiP2PMatch_FreeWindow
static BmpWin *func_ov099_021ba8bc(BmpWin *window) {
    if (window != NULL) {
        func_0201f7a8(window, WINFRAME_TRANSFER_NOW);
        BmpWin_Free(window);
    }
    return NULL;
}

// pokebw2's WifiP2PMatch_FreeWaitIcon
static void func_ov099_021ba8d4(WIFIP2PMATCH_WORK *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
}

// Ends the message and frees its window
// pokebw2's WifiP2PMatch_EndMessage
static void func_ov099_021ba8ec(WIFIP2PMATCH_WORK *wk) {
    func_ov099_021ba8d4(wk);
    if (!func_0201c6b4(wk->printQueue)) {
        func_0201c6ec(wk->printQueue);
    }
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
        wk->printStream = NULL;
    }
    wk->msgWin = func_ov099_021ba8bc(wk->msgWin);
}

// Runs the message. Returns whether it has been printed, which is TRUE when there is none
// pokebw2's WifiP2PMatch_IsMessageEnd
static BOOL func_ov099_021ba928(WIFIP2PMATCH_WORK *wk) {
    BOOL ret = TRUE;

    if (wk->printStream != NULL) {
        if (wk->msgWin != NULL) {
            func_020286c4(wk->keyCursor, wk->printStream, wk->msgWin);
        }
        ret = func_02028484(&wk->printCommon, wk->printStream);
        if (ret) {
            if (wk->printStream != NULL) {
                func_0201ce48(wk->printStream);
                wk->printStream = NULL;
            }
        }
    }
    return ret;
}

// Prints a message of the match board's bank, or of the Wi-Fi error bank, in a new message window, a character at a
// time or at once
// pokebw2's WifiP2PMatch_PrintMessageCore
static void func_ov099_021ba978(WIFIP2PMATCH_WORK *wk, u32 msgId, BOOL sysMsg, BOOL stream) {
    func_02012ffc();
    func_ov099_021ba8d4(wk);
    wk->msgWin = func_ov099_021ba8bc(wk->msgWin);
    if (!func_0201c6b4(wk->printQueue)) {
        func_0201c6ec(wk->printQueue);
    }
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
        wk->printStream = NULL;
    }
    wk->msgWin = BmpWin_CreateDynamic(2, 2, 19, 27, 4, 15, 1);
    if (sysMsg) {
        GFL_MsgDataLoadStrbuf(wk->sysMsgData, msgId, wk->expStrBuf);
    } else {
        GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->expStrBuf);
    }
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->talkString, wk->expStrBuf);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->msgWin), 0xff);
    BmpWin_DrawFrame(wk->msgWin, WINFRAME_TRANSFER_VBLANK, wk->talkFrameChars, 14);
    if (!stream) {
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->msgWin), 0, 0, wk->talkString, wk->font);
    } else {
        wk->printStream = func_0201cce4(wk->msgWin, 0, 0, wk->talkString, wk->font, func_02012ffc(), wk->tcbManager, 2,
                                        WIFIP2PMATCH_HEAP, 15);
        func_02028470(&wk->printCommon, 2);
    }
    BmpWin_FlushChar(wk->msgWin);
    BmpWin_FlushMap(wk->msgWin);
    GFL_BGSysQueueScrLoad(2);
}

// Prints a message a character at a time; func_ov099_021ba928 runs it
// pokebw2's WifiP2PMatch_PrintMessage
static void func_ov099_021baab8(WIFIP2PMATCH_WORK *wk, u32 msgId, BOOL sysMsg) {
    wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
    func_ov099_021ba978(wk, msgId, sysMsg, TRUE);
}

// Prints a message at once
// pokebw2's WifiP2PMatch_PrintMessageNow
static void func_ov099_021baadc(WIFIP2PMATCH_WORK *wk, u32 msgId, BOOL sysMsg) {
    wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
    func_0201dd68();
    func_ov099_021ba978(wk, msgId, sysMsg, FALSE);
}

// Prints a message of the Wi-Fi error bank in the large window
// pokebw2's WifiP2PMatch_PrintSystemMessage
static void func_ov099_021bab04(WIFIP2PMATCH_WORK *wk, u32 msgId) {
    func_ov099_021ba8d4(wk);
    wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
    wk->msgWin = func_ov099_021ba8bc(wk->msgWin);
    wk->sysMsgWin = BmpWin_CreateDynamic(2, 4, 4, 23, 16, 15, 1);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 15 * 0x20, 0x20, WIFIP2PMATCH_HEAP);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->sysMsgWin), 0xff);
    func_020456f8(wk->sysMsgWin);
    BmpWin_FlushMap(wk->sysMsgWin);
    BmpWin_DrawFrame(wk->sysMsgWin, WINFRAME_TRANSFER_VBLANK, wk->talkFrameChars, 14);
    GFL_MsgDataLoadStrbuf(wk->sysMsgData, msgId, wk->expStrBuf);
    GFL_WordSetFormatStrbuf(wk->wordSet, wk->talkString, wk->expStrBuf);
    func_0201dd68();
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->sysMsgWin), 4, 4, wk->talkString, wk->font);
    BmpWin_FlushChar(wk->sysMsgWin);
}

// The status menu's print callback: voice chat and the Xtransceiver are in another color while the player's voice
// chat is off
// pokebw2's WifiP2PMatch_GameModeMenuPrintCallback
static void func_ov099_021babdc(BmpMenuList *list, intptr_t value, u8 y) {
    WIFIP2PMATCH_WORK *wk = func_02020d0c(list);

    if (!func_ov099_021c27e0(wk->myStatus)) {
        switch (value) {
        case WIFIP2PMATCH_GAMEMODE_VCHAT:
        case WIFIP2PMATCH_GAMEMODE_TVT:
            func_02020268(list, 2, 15, 2);
            break;
        default:
            func_02020268(list, 1, 15, 2);
            break;
        }
    } else {
        func_02020268(list, 1, 15, 2);
    }
}

// Opens a list menu of count options at the right edge of the screen, width characters wide, with the cursor where
// the menu of this type last had it
static void func_ov099_021bac24(WIFIP2PMATCH_WORK *wk, BmpMenuListHeader *header,
                                      WIFIP2PMATCH_MENU_OPTION *options, int count, int type, int width) {
    int i;
    int y;

    func_0201c6ec(wk->listPrintQueue);
    func_0201dd68();
    wk->menuOptions = ListMenuCore_CreateOptionList(count, WIFIP2PMATCH_HEAP);
    for (i = 0; i < count; i++) {
        ListMenuCore_AppendMsgOption(wk->menuOptions, wk->msgData, options[i].msg, options[i].value, WIFIP2PMATCH_HEAP);
    }
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    y = 5;
    if (type == WIFIP2PMATCH_MENU_PARTY) {
        y = 13;
    }
    wk->listWin = BmpWin_CreateDynamic(2, 31 - width, y, width, count * 2, 15, 1);
    BmpWin_FlushMap(wk->listWin);
    BmpWin_DrawFrame(wk->listWin, WINFRAME_TRANSFER_NONE, wk->listFrameChars, 14);
    header->options = wk->menuOptions;
    header->window = wk->listWin;
    header->queue = wk->listPrintQueue;
    header->printWindow = &wk->listPrintWindow;
    header->font = wk->font;
    header->work = wk;
    wk->list = BmpMenuList_Create(header, 0, wk->menuCursor[type], WIFIP2PMATCH_HEAP);
    while (!func_0201c6b4(wk->listPrintQueue)) {
        func_0201c4e4(wk->listPrintQueue);
    }
    func_02020cf8(wk->list, WIFIP2PMATCH_HEAP);
    GFL_BGSysQueueScrLoad(2);
}

// The menu of what to do with the friend the player is connected to
// pokebw2's WifiP2PMatch_OpenGameModeMenu
static void func_ov099_021bad50(WIFIP2PMATCH_WORK *wk) {
    func_ov099_021bac24(wk, &data_ov099_021c3b00, data_ov099_021c3a50, 5,
                              WIFIP2PMATCH_MENU_STATUS, 17);
}

// The menu of a friend in the room: connecting is left out unless the friend is waiting or recruiting
// pokebw2's WifiP2PMatch_OpenFriendMenu
static void func_ov099_021bad74(WIFIP2PMATCH_WORK *wk) {
    int count = 2;
    BmpMenuListHeader header = data_ov099_021c37a0;
    WIFI_STATUS *status;
    u16 wifiStatus;
    u32 i;

    status = func_ov099_021c23b8(func_ov099_021b97f4(&wk->room) - 1);
    wifiStatus = func_ov099_021bdd48(wk, status);
    func_ov099_021c27e0(status);
    func_ov099_021c27e0(wk->myStatus);
    wk->menuOptions = ListMenuCore_CreateOptionList(2, WIFIP2PMATCH_HEAP);
    for (i = 0; i < 2; i++) {
        if (i == 0 && wifiStatus != WIFIP2PMATCH_STATUS_RECRUIT && wifiStatus != 5 &&
            wifiStatus != WIFIP2PMATCH_STATUS_WAIT) {
            header.maxShown--;
            header.count--;
            count--;
        } else {
            ListMenuCore_AppendMsgOption(wk->menuOptions, wk->msgData, data_ov099_021c3730[i].msg,
                                         data_ov099_021c3730[i].value, WIFIP2PMATCH_HEAP);
        }
    }
    wk->listWin = BmpWin_CreateDynamic(2, 16, (3 - count) * 2 + 11, 15, count * 2, 15, 1);
    BmpWin_DrawFrame(wk->listWin, WINFRAME_TRANSFER_NONE, wk->talkFrameChars, 14);
    header.options = wk->menuOptions;
    header.window = wk->listWin;
    header.queue = wk->listPrintQueue;
    header.printWindow = &wk->listPrintWindow;
    header.font = wk->font;
    wk->list = BmpMenuList_Create(&header, 0, 0, WIFIP2PMATCH_HEAP);
    while (!func_0201c6b4(wk->listPrintQueue)) {
        func_0201c4e4(wk->listPrintQueue);
    }
    func_02020cf8(wk->list, WIFIP2PMATCH_HEAP);
    BmpWin_FlushMap(wk->listWin);
    GFL_BGSysQueueScrLoad(2);
}

// Draws the player's name, in the color of their gender, and what they are doing: the status, or the name of the
// friend they are in a voice chat with
// pokebw2's WifiP2PMatch_DrawMyInfo
static void func_ov099_021baedc(WIFIP2PMATCH_WORK *wk) {
    PlayerInfo *info = func_02008730(wk->save);
    StrBuf *name;
    u32 status;
    u32 gameMode;
    int msgId;
    u32 color;
    u32 icon;

    if (wk->myInfoWin != NULL) {
        func_0201dd68();
        name = copyTrainerNameToNewStrbuf(info->name, WIFIP2PMATCH_HEAP);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->myInfoWin), 0xff);
        if (getTrainerGender(info) == 0) {
            GFL_TextRndUpdateColorIndexLUT(5, 6, 0);
        } else {
            GFL_TextRndUpdateColorIndexLUT(3, 4, 0);
        }
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->myInfoWin), 32, 0, name, wk->font);
        GFL_StrBufFree(name);
        status = func_ov099_021bdd48(wk, wk->myStatus);
        gameMode = func_ov099_021bdd54(wk, wk->myStatus);
        msgId = func_ov099_021bb918(status, gameMode, &color);
        if (gameMode == WIFIP2PMATCH_GAMEMODE_VCHAT && status == WIFIP2PMATCH_STATUS_PLAYING) {
            if (func_ov009_02156a3c() == -1) {
                GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->expStrBuf);
            } else {
                u32 gender = func_02009864(wk->wifiList, func_ov009_02156a3c(), WIFI_LIST_FRIEND_GENDER);

                if (gender == 0) {
                    GFL_TextRndUpdateColorIndexLUT(5, 6, 0);
                } else if (gender == 1) {
                    GFL_TextRndUpdateColorIndexLUT(3, 4, 0);
                }
                func_020099e4(wk->wifiList, func_ov009_02156a3c(), wk->expStrBuf);
            }
        } else {
            GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->expStrBuf);
        }
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->myInfoWin), 102, 0, wk->expStrBuf, wk->font);
        func_ov099_021be4bc(&wk->icon, 1, 2, 1, status, gameMode, 0x30);
        if (func_ov099_021c27e0(wk->myStatus)) {
            icon = 15;
        } else {
            icon = 11;
        }
        func_ov099_021be3cc(&wk->icon, 1, 26, 1, icon + 0x30, 0);
        func_020456f8(wk->myInfoWin);
        BmpWin_FlushMap(wk->myInfoWin);
        BmpWin_FlushChar(wk->myInfoWin);
        GFL_BGSysLoadScr(3);
    }
}

// The title of the upper screen
// pokebw2's WifiP2PMatch_DrawTitle
static void func_ov099_021bb050(WIFIP2PMATCH_WORK *wk) {
    func_ov099_021ba8bc(wk->titleWin);
    wk->titleWin = BmpWin_CreateDynamic(3, 5, 1, 22, 2, 15, 1);
    BmpWin_FlushMap(wk->titleWin);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_018, wk->titleString);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->titleWin), 0, 0, wk->titleString, wk->font);
    BmpWin_FlushChar(wk->titleWin);
}

// Frees the message, every window, the menus and the yes/no menu
// pokebw2's WifiP2PMatch_FreeWindows
static void func_ov099_021bb0b4(WIFIP2PMATCH_WORK *wk) {
    func_ov099_021ba8ec(wk);
    if (wk->listWin != NULL) {
        BmpWin_Free(wk->listWin);
        BmpMenuList_Free(wk->list, NULL, NULL);
        ListMenuCore_FreeOptionList(wk->menuOptions);
        wk->menuOptions = NULL;
    }
    if (wk->titleWin != NULL) {
        BmpWin_Free(wk->titleWin);
        wk->titleWin = NULL;
    }
    if (wk->msgWin != NULL) {
        BmpWin_Free(wk->msgWin);
        wk->msgWin = NULL;
    }
    if (wk->unk1A8 != NULL) {
        BmpWin_Free(wk->unk1A8);
        wk->unk1A8 = NULL;
    }
    if (wk->myInfoWin != NULL) {
        BmpWin_Free(wk->myInfoWin);
        wk->myInfoWin = NULL;
    }
    if (wk->subWin != NULL) {
        BmpWin_Free(wk->subWin);
        wk->subWin = NULL;
    }
    if (wk->sysMsgWin != NULL) {
        func_020456f8(wk->sysMsgWin);
        GFL_BGSysQueueScrLoad(2);
        func_ov099_021ba8bc(wk->sysMsgWin);
        wk->sysMsgWin = NULL;
    }
    if (wk->subList != NULL) {
        BmpMenuList_Free(wk->subList, NULL, NULL);
        wk->subList = NULL;
    }
    if (wk->subMenuOptions != NULL) {
        ListMenuCore_FreeOptionList(wk->subMenuOptions);
        wk->subMenuOptions = NULL;
    }
    if (wk->yesNoMenu != NULL) {
        func_0201fef4(wk->yesNoMenu);
        wk->yesNoMenu = NULL;
    }
}

// The strings, print queues and cursor of the messages
// pokebw2's WifiP2PMatch_InitMessages
static void func_ov099_021bb1a8(WIFIP2PMATCH_WORK *wk) {
    wk->talkString = GFL_StrBufCreate(380, WIFIP2PMATCH_HEAP);
    wk->unk0FC = GFL_StrBufCreate(380, WIFIP2PMATCH_HEAP);
    wk->expStrBuf = GFL_StrBufCreate(380, WIFIP2PMATCH_HEAP);
    wk->titleString = GFL_StrBufCreate(180, WIFIP2PMATCH_HEAP);
    wk->printQueue = func_0201c440(WIFIP2PMATCH_HEAP);
    wk->listPrintQueue = func_0201c440(WIFIP2PMATCH_HEAP);
    wk->tcbManager = GFL_TCBExMgrCreate(WIFIP2PMATCH_HEAP, WIFIP2PMATCH_HEAP, 2, 0);
    wk->keyCursor = func_02028590(15, TRUE, FALSE, WIFIP2PMATCH_HEAP);
}

// pokebw2's WifiP2PMatch_ExitMessages
static void func_ov099_021bb224(WIFIP2PMATCH_WORK *wk) {
    func_ov099_021bb4d0(wk);
    func_ov099_021bb42c(wk);
    func_02028604(wk->keyCursor);
    GFL_StrBufFree(wk->titleString);
    GFL_StrBufFree(wk->talkString);
    GFL_StrBufFree(wk->unk0FC);
    if (wk->printStream != NULL) {
        func_0201ce48(wk->printStream);
        wk->printStream = NULL;
    }
    GFL_StrBufFree(wk->expStrBuf);
    GFL_TCBExMgrFree(wk->tcbManager);
    func_0201c6ec(wk->printQueue);
    func_0201c6ec(wk->listPrintQueue);
    func_0201c4c0(wk->printQueue);
    func_0201c4c0(wk->listPrintQueue);
    wk->printQueue = NULL;
}

// Draws the detail page of the friend whose button was touched again
// pokebw2's WifiP2PMatch_RedrawFriendDetail
static void func_ov099_021bb2b0(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    func_02009864(wk->wifiList, wk->view.touchFriendNo - 1, WIFI_LIST_FRIEND_GENDER);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->view.detailWin[0]), 0);
    func_ov099_021bb528(wk, heapId);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, TRUE);
}

// Opens the detail page of the friend whose button was touched, in place of the buttons
// pokebw2's WifiP2PMatch_OpenFriendDetail
static void func_ov099_021bb2f0(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    func_02009864(wk->wifiList, wk->view.touchFriendNo - 1, WIFI_LIST_FRIEND_GENDER);
    GFL_BGSysBufferScr(6, wk->view.backScrn->rawData, wk->view.backScrn->szByte, 0);
    GFL_BGSysClearBG(7);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->view.detailWin[1]), 0);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->view.detailWin[0]), 0);
    func_ov099_021bb528(wk, heapId);
    GFL_BGSysQueueScrLoad(7);
    BmpWin_FlushChar(wk->view.detailWin[1]);
    BmpWin_FlushChar(wk->view.detailWin[0]);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG1, FALSE);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_BG3, TRUE);
}

// The sprite of a friend's trainer on the detail page
// pokebw2's WifiP2PMatch_CreateTrainerActor
static void func_ov099_021bb374(WIFIP2PMATCH_WORK *wk, int friendIndex, u32 heapId) {
    ClActorSetup setup = { 56, 80, 0, 0, 0 };
    u32 trainerClass;
    ArcTool *arc;

    if (wk->trainerActor != NULL) {
        func_ov099_021bb42c(wk);
    }
    trainerClass = func_02025c88(func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_TRAINER_VIEW));
    arc = func_0202da04(heapId);
    wk->trainerChars = func_0202da48(arc, trainerClass, 1, heapId);
    wk->trainerPalette = func_0202da18(arc, trainerClass, 1, 0, heapId);
    wk->trainerCellAnims = func_0202da6c(trainerClass, 2, 1, heapId);
    GFL_ArcToolFree(arc);
    wk->trainerActor =
        func_0204b294(wk->clActUnit, wk->trainerChars, wk->trainerPalette, wk->trainerCellAnims, &setup, 1, heapId);
}

// pokebw2's WifiP2PMatch_FreeTrainerActor
static void func_ov099_021bb42c(WIFIP2PMATCH_WORK *wk) {
    if (wk->trainerActor != NULL) {
        func_0204b3b4(wk->trainerActor);
        func_0204a8d4(wk->trainerChars);
        func_0204ada4(wk->trainerPalette);
        func_0204afd8(wk->trainerCellAnims);
        wk->trainerActor = NULL;
    }
}

// pokebw2's WIFIP2PMATCH_UNK_100. Declared in app/wifi_p2pmatch.h, which says why. Nothing reads it
const u16 data_ov099_021c3718 = 100;

// The detail page's touch bar, with the return button
// pokebw2's WifiP2PMatch_CreateTouchBar
static void func_ov099_021bb460(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    TouchBarItem items[] = {
        { 1, { 232, 168 } },
    };
    TouchBarSetup setup;

    if (wk->touchBar == NULL) {
        MI_CpuFill8(&setup, 0, sizeof(setup));
        setup.items = items;
        setup.count = NELEMS(items);
        setup.unit = wk->clActUnit;
        setup.unk1C = FALSE;
        setup.bg = 4;
        setup.bgPalette = 12;
        setup.objPalette = 12;
        setup.vramType = 2;
        wk->touchBar = func_ov067_021b62e0(&setup, heapId);
        func_ov067_021b6578(wk->touchBar, 1, 0);
    }
}

// pokebw2's WifiP2PMatch_FreeTouchBar
static void func_ov099_021bb4d0(WIFIP2PMATCH_WORK *wk) {
    if (wk->touchBar != NULL) {
        func_ov067_021b639c(wk->touchBar);
        wk->touchBar = NULL;
    }
}

// Loads the friend buttons' screen again
// pokebw2's WifiP2PMatch_ReloadViewScreen
static void func_ov099_021bb4ec(WIFIP2PMATCH_WORK *wk) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_WIFIP2PMATCH, WIFIP2PMATCH_HEAP);

    GFL_G2DIOLoadNSCRSync(arc, 22, 4, 0, (u16)wk->viewBgChars, 0, FALSE, WIFIP2PMATCH_HEAP);
    GFL_ArcToolFree(arc);
}

// Draws the detail page of the friend whose button was touched: the name, the status, the battle record, the number
// of trades, the date of the last game, the status icons, the trainer and the touch bar
// pokebw2's WifiP2PMatch_DrawFriendDetail
static void func_ov099_021bb528(WIFIP2PMATCH_WORK *wk, u32 heapId) {
    int friendIndex;
    WIFI_STATUS *friendStatus;
    u32 status;
    u32 gameMode;
    int msgId;
    u32 color;
    u32 day;
    u32 icon;

    func_0201dd68();
    friendIndex = wk->view.touchFriendNo - 1;
    func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_GENDER);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_ov099_021c2370(wk, friendIndex);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_033, wk->expStrBuf);
    GFL_WordSetFormatStrbuf(wk->view.wordSet, wk->titleString, wk->expStrBuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->view.detailWin[0]), 25, 0, wk->titleString, wk->font);

    friendStatus = func_ov099_021c23b8(friendIndex);
    status = func_ov099_021bdd48(wk, friendStatus);
    gameMode = func_ov099_021bdd54(wk, friendStatus);
    msgId = func_ov099_021bb918(status, gameMode, &color);
    // The battles' statuses have short names for this page
    if (msgId >= Bank0416_Text_141 && msgId <= Bank0416_Text_144) {
        msgId -= Bank0416_Text_141 - Bank0416_Text_060;
    }
    GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->expStrBuf);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[0]), 96, 0, wk->expStrBuf, wk->font,
                                   PRINT_COLOR(15, 2, 0));

    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_035, wk->titleString);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 104, 0, wk->titleString, wk->font,
                                   PRINT_COLOR(15, 2, 0));
    WordSetNumber(wk->view.wordSet, 0, func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_WINS), 4, 1, TRUE);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_036, wk->expStrBuf);
    GFL_WordSetFormatStrbuf(wk->view.wordSet, wk->titleString, wk->expStrBuf);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 176, 24, wk->titleString, wk->font,
                                   PRINT_COLOR(15, 2, 0));
    WordSetNumber(wk->view.wordSet, 0, func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_LOSSES), 4, 1, TRUE);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_037, wk->expStrBuf);
    GFL_WordSetFormatStrbuf(wk->view.wordSet, wk->titleString, wk->expStrBuf);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 176, 40, wk->titleString, wk->font,
                                   PRINT_COLOR(15, 2, 0));

    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_038, wk->titleString);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 104, 64, wk->titleString, wk->font,
                                   PRINT_COLOR(15, 2, 0));
    WordSetNumber(wk->view.wordSet, 0, func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_DRAWS), 4, 1, TRUE);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_039, wk->expStrBuf);
    GFL_WordSetFormatStrbuf(wk->view.wordSet, wk->titleString, wk->expStrBuf);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 188, 64, wk->titleString, wk->font,
                                   PRINT_COLOR(15, 2, 0));

    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_042, wk->titleString);
    GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 8, 96, wk->titleString, wk->font,
                                   PRINT_COLOR(15, 2, 0));
    day = func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_LAST_DAY);
    if (day != 0) {
        WordSetNumber(wk->view.wordSet, 2, day, 2, 0, TRUE);
        WordSetNumber(wk->view.wordSet, 0, func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_LAST_YEAR), 4, 1,
                      TRUE);
        WordSetNumber(wk->view.wordSet, 1, func_02009864(wk->wifiList, friendIndex, WIFI_LIST_FRIEND_LAST_MONTH), 2, 0,
                      TRUE);
        GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_044, wk->expStrBuf);
        GFL_WordSetFormatStrbuf(wk->view.wordSet, wk->titleString, wk->expStrBuf);
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->view.detailWin[1]), 152, 96, wk->titleString, wk->font,
                                       PRINT_COLOR(15, 2, 0));
    }
    BmpWin_FlushMap(wk->view.detailWin[1]);
    BmpWin_FlushChar(wk->view.detailWin[1]);
    BmpWin_FlushMap(wk->view.detailWin[0]);
    BmpWin_FlushChar(wk->view.detailWin[0]);
    GFL_BGSysQueueScrLoad(7);

    func_ov099_021be4bc(&wk->icon, 6, 2, 1, status, gameMode, 0x20);
    if (func_ov099_021c285c(friendStatus)) {
        icon = 0x2f;
    } else {
        icon = 0x2b;
    }
    func_ov099_021be3cc(&wk->icon, 6, 28, 1, icon, 0);
    func_ov099_021bb374(wk, friendIndex, heapId);
    func_ov099_021bb460(wk, heapId);
    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
}

// The message of a status, by its game mode. Sets the text colors
// pokebw2's WifiP2PMatch_GetStatusMessage
static int func_ov099_021bb918(u32 status, u32 gameMode, u32 *color) {
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    GFL_TextRndUpdateColorIndexLUT(1, 2, 0);
    {
        int msgs[] = {
            Bank0416_Text_049, Bank0416_Text_049,   Bank0416_Text_049, Bank0416_Text_122,
            Bank0416_Text_027,  Bank0416_Text_088, Bank0416_Text_025,      Bank0416_Text_141,
            Bank0416_Text_141,     Bank0416_Text_142,       Bank0416_Text_142,     Bank0416_Text_143,
            Bank0416_Text_143,     Bank0416_Text_144,     Bank0416_Text_144,   Bank0416_Text_059,
        };

        if (gameMode > WIFIP2PMATCH_GAMEMODE_MAX) {
            gameMode = WIFIP2PMATCH_GAMEMODE_MAX;
        }
        return msgs[gameMode];
    }
}

// pokebw2's sWifiP2PMatchBattleRulesMenuHeader
static BmpMenuListHeader data_ov099_021c3b68 = {
    NULL, NULL, NULL, NULL, 6, 6, 0,    16,   0,  0, 1, 15, 2, 0, 0, BMPMENULIST_SKIP_NONE, 0, BMPMENULIST_CURSOR_SHOW,
    NULL, 16, 16, NULL, NULL, NULL, NULL,
};

void func_ov099_021bb954(WIFIP2PMATCH_WORK *wk, BOOL fromParent) {
    if (fromParent) {
        WifiClubData *parent = wk->parent;
        data_ov099_021c3ad0[0].msg = Bank0416_Text_131 + parent->unk21;
        data_ov099_021c3ad0[1].msg = Bank0416_Text_135 + parent->unk22;
        data_ov099_021c3ad0[2].msg = Bank0416_Text_138 - parent->unk20;
    } else {
        data_ov099_021c3ad0[0].msg = Bank0416_Text_131 + wk->battleMode;
        data_ov099_021c3ad0[1].msg = Bank0416_Text_135 + wk->battleRule;
        data_ov099_021c3ad0[2].msg = Bank0416_Text_138 - wk->parent->unk48;
    }
    func_ov099_021bac24(wk, &data_ov099_021c3b68, data_ov099_021c3ad0, 6, WIFIP2PMATCH_MENU_BATTLE_RULES, 30);
}


// pokebw2's WifiP2PMatch_OpenBattleModeMenu
static void func_ov099_021bb9c8(WIFIP2PMATCH_WORK *wk) {
    data_ov099_021c3b34.count = 5;
    data_ov099_021c3b34.maxShown = 5;
    func_ov099_021bac24(wk, &data_ov099_021c3b34, data_ov099_021c3a78, 5,
                              WIFIP2PMATCH_MENU_BATTLE_MODE, 17);
}

// pokebw2's WifiP2PMatch_OpenBattleRuleMenu
static void func_ov099_021bb9f4(WIFIP2PMATCH_WORK *wk) {
    data_ov099_021c3b34.count = 3;
    data_ov099_021c3b34.maxShown = 3;
    func_ov099_021bac24(wk, &data_ov099_021c3b34, data_ov099_021c3a00, 3,
                              WIFIP2PMATCH_MENU_BATTLE_RULE, 17);
}

// pokebw2's WifiP2PMatch_OpenShooterMenu
static void func_ov099_021bba20(WIFIP2PMATCH_WORK *wk) {
    data_ov099_021c3b34.count = 3;
    data_ov099_021c3b34.maxShown = 3;
    func_ov099_021bac24(wk, &data_ov099_021c3b34, data_ov099_021c3a18, 3,
                              WIFIP2PMATCH_MENU_SHOOTER, 20);
}

// The window over the party's icons: each Pokémon's level and gender
// pokebw2's WifiP2PMatch_DrawPartyWindow
static void func_ov099_021bba4c(WIFIP2PMATCH_WORK *wk, PokeParty *party) {
    StrBuf *levelString;
    StrBuf *string;
    int count;
    int i;

    if (wk->subWin == NULL) {
        wk->subWin = BmpWin_CreateDynamic(2, 1, 5, 30, 6, 15, 1);
    }
    BmpWin_DrawFrame(wk->subWin, WINFRAME_TRANSFER_NONE, wk->talkFrameChars, 14);
    levelString = GFL_StrBufCreate(64, WIFIP2PMATCH_HEAP);
    string = GFL_StrBufCreate(64, WIFIP2PMATCH_HEAP);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->subWin), 0xff);
    func_020456f8(wk->subWin);
    count = PokeParty_GetPkmCount(party);
    for (i = 0; i < count; i++) {
        PartyPkm *pkm = PokeParty_GetPkm(party, i);
        u32 species;
        u32 sex;
        u32 level;
        u32 nidoranNickname;

        if (PokeParty_GetParam(pkm, PKM_PARAM_IS_EGG, NULL)) {
            continue;
        }
        // BUG: the species is meant, and the personality value is read, so the gender of a Nidoran whose name shows
        // it is printed as well
#ifdef BUGFIX
        species = PokeParty_GetParam(pkm, PKM_PARAM_SPECIES, NULL);
#else
        species = PokeParty_GetParam(pkm, PKM_PARAM_PID, NULL);
#endif
        nidoranNickname = PokeParty_GetParam(pkm, PKM_PARAM_NIDORAN_NICKNAME, NULL);
        level = PokeParty_GetParam(pkm, PKM_PARAM_LEVEL, NULL);
        sex = PokeParty_GetParam(pkm, PKM_PARAM_SEX, NULL);
        GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_116, string);
        WordSetNumber(wk->wordSet, 0, level, 3, 0, TRUE);
        GFL_WordSetFormatStrbuf(wk->wordSet, levelString, string);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->subWin), i * 40 + 1, 32, levelString, wk->smallFont);
        if (sex == GENDER_UNKNOWN) {
            continue;
        }
        if ((species == SPECIES_NIDORAN_F || species == SPECIES_NIDORAN_M) && !nidoranNickname) {
            continue;
        }
        if (sex == GENDER_MALE) {
            GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_117, string);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->subWin), i * 40 + 30, 16, string, wk->font);
        } else {
            GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_118, string);
            GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->subWin), i * 40 + 30, 16, string, wk->font);
        }
    }
    GFL_StrBufFree(levelString);
    GFL_StrBufFree(string);
    BmpWin_FlushMap(wk->subWin);
    BmpWin_FlushChar(wk->subWin);
    GFL_BGSysQueueScrLoad(2);
}

// pokebw2's WifiP2PMatch_FreePartyWindow
static void func_ov099_021bbc0c(WIFIP2PMATCH_WORK *wk) {
    if (wk->subWin != NULL) {
        func_020456f8(wk->subWin);
        func_0201f7a8(wk->subWin, WINFRAME_TRANSFER_VBLANK);
        BmpWin_Free(wk->subWin);
        wk->subWin = NULL;
    }
}

// The icons of a party's Pokémon, from icon base on
// pokebw2's WifiP2PMatch_CreatePartyIcons
static void func_ov099_021bbc34(WIFIP2PMATCH_WORK *wk, PokeParty *party, int base, ArcTool *arc) {
    int count = PokeParty_GetPkmCount(party);
    int i;

    for (i = 0; i < count; i++) {
        BoxPkm *pkm = func_0201d624(PokeParty_GetPkm(party, i));
        ClActorSetup setup;

        setup.x = i * 40 + 22;
        setup.y = 54;
        setup.sequence = 0;
        setup.priority = 0;
        setup.bgPriority = 0;
        wk->pokeIconChars[base + i] = func_0204a6c8(arc, func_0201ba68(pkm), FALSE, 0, WIFIP2PMATCH_HEAP);
        wk->pokeIcons[base + i] = func_0204b294(wk->clActUnit, wk->pokeIconChars[base + i], wk->pokeIconPalette,
                                                wk->pokeIconCellAnims, &setup, 0, WIFIP2PMATCH_HEAP);
        func_0204b6f4(wk->pokeIcons[base + i], func_0201bbe8(pkm), 1);
    }
}

// The icons of the party and of the Battle Box, hidden
// pokebw2's WifiP2PMatch_CreatePokeIcons
static void func_ov099_021bbd0c(WIFIP2PMATCH_WORK *wk, PokeParty *party, PokeParty *battleBox) {
    ArcTool *arc = GFL_ArcSysCreateFileHandle(ARCID_POKEICON, WIFIP2PMATCH_HEAP);

    wk->pokeIconPalette = func_0204acec(arc, func_0201bc3c(), 0, 11 * 0x20, WIFIP2PMATCH_HEAP);
    wk->pokeIconCellAnims =
        func_0204af28(arc, func_0201bc78(), getOBJTileMapping_MainEng(), WIFIP2PMATCH_HEAP);
    func_ov099_021bbc34(wk, party, 0, arc);
    func_ov099_021bbc34(wk, battleBox, 6, arc);
    func_ov099_021bbe50(wk);
    GFL_ArcToolFree(arc);
}

// pokebw2's WifiP2PMatch_FreePokeIcons
static void func_ov099_021bbd80(WIFIP2PMATCH_WORK *wk) {
    int i;

    if (wk->pokeIcons[0] != NULL) {
        for (i = 0; i < 12; i++) {
            if (wk->pokeIcons[i] != NULL) {
                func_0204b3b4(wk->pokeIcons[i]);
                func_0204a8d4(wk->pokeIconChars[i]);
            }
            wk->pokeIcons[i] = NULL;
        }
        func_0204ada4(wk->pokeIconPalette);
        func_0204afd8(wk->pokeIconCellAnims);
    }
}

// Shows the first six icons and hides the other six, or the other way around
// pokebw2's WifiP2PMatch_ShowPokeIcons
static void func_ov099_021bbdd0(WIFIP2PMATCH_WORK *wk, BOOL first) {
    int i;

    for (i = 0; i < 6; i++) {
        if (wk->pokeIcons[i] != NULL) {
            if (first) {
                func_0204b92c(wk->pokeIcons[i], TRUE);
                func_0204b3dc(wk->pokeIcons[i], TRUE);
            } else {
                func_0204b92c(wk->pokeIcons[i], FALSE);
                func_0204b3dc(wk->pokeIcons[i], FALSE);
            }
        }
    }
    for (i = 6; i < 12; i++) {
        if (wk->pokeIcons[i] != NULL) {
            if (first) {
                func_0204b92c(wk->pokeIcons[i], FALSE);
                func_0204b3dc(wk->pokeIcons[i], FALSE);
            } else {
                func_0204b92c(wk->pokeIcons[i], TRUE);
                func_0204b3dc(wk->pokeIcons[i], TRUE);
            }
        }
    }
}

// pokebw2's WifiP2PMatch_HidePokeIcons
static void func_ov099_021bbe50(WIFIP2PMATCH_WORK *wk) {
    int i;

    for (i = 0; i < 12; i++) {
        if (wk->pokeIcons[i] != NULL) {
            func_0204b92c(wk->pokeIcons[i], FALSE);
            func_0204b3dc(wk->pokeIcons[i], FALSE);
        }
    }
}

// The menu of the party and the Battle Box, which B doesn't close
// pokebw2's WifiP2PMatch_OpenPartyMenu
static void func_ov099_021bbe7c(WIFIP2PMATCH_WORK *wk) {
    data_ov099_021c3b34.count = 2;
    data_ov099_021c3b34.maxShown = 2;
    func_ov099_021bac24(wk, &data_ov099_021c3b34, data_ov099_021c39f0, 2,
                              WIFIP2PMATCH_MENU_PARTY, 18);
    func_02020d10(wk->list, TRUE);
}

// The window of the regulation's six rules, under a title (a WIFIP2PMATCH_RULES_*). The rules whose bit of failBits
// is set, those the party breaks, are in another color
// pokebw2's WifiP2PMatch_DrawRulesWindow
static void func_ov099_021bbeb4(WIFIP2PMATCH_WORK *wk, u32 failBits, BOOL shooter, int kind) {
    int top = 4;
    int i;

    if (shooter == TRUE) {
        shooter = TRUE;
    } else {
        shooter = FALSE;
    }
    wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
    if (kind == WIFIP2PMATCH_RULES_SHORT) {
        wk->sysMsgWin = BmpWin_CreateDynamic(2, 2, 5, 26, 12, 15, 1);
    } else {
        wk->sysMsgWin = BmpWin_CreateDynamic(2, 2, 5, 26, 16, 15, 1);
    }
    GFL_BitmapFill(BmpWin_GetBitmap(wk->sysMsgWin), 0xff);
    func_020456f8(wk->sysMsgWin);
    BmpWin_FlushMap(wk->sysMsgWin);
    BmpWin_DrawFrame(wk->sysMsgWin, WINFRAME_TRANSFER_VBLANK, wk->talkFrameChars, 14);
    func_0201dd68();
    wk->regulationPrintMsg = func_0201a4d4(wk->regulation, wk->wordSet, WIFIP2PMATCH_HEAP, shooter);
    if (kind == WIFIP2PMATCH_RULES_BATTLE_TYPE) {
        StrBuf *title = makeStrbufLoadBattleTypeText(wk->regulation, WIFIP2PMATCH_HEAP);

        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->sysMsgWin), 0, 0, title, wk->font);
        GFL_StrBufFree(title);
    } else if (kind != WIFIP2PMATCH_RULES_SHORT) {
        u32 msgId = Bank0416_Text_102;

        if (kind != WIFIP2PMATCH_RULES_PARTY) {
            msgId = Bank0416_Text_103;
        }
        GFL_MsgDataLoadStrbuf(wk->msgData, msgId, wk->expStrBuf);
        GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(wk->sysMsgWin), 0, 0, wk->expStrBuf, wk->font);
    }
    if (kind == WIFIP2PMATCH_RULES_SHORT) {
        top = 0;
    }
    for (i = 0; i < 6; i++) {
        u16 color;

        if (failBits & (1 << i)) {
            color = PRINT_COLOR(4, 2, 0);
        } else {
            color = PRINT_COLOR(1, 2, 0);
        }
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->sysMsgWin), 0, (top + i * 2) * 8,
                                       wk->regulationPrintMsg->rule[i], wk->font, color);
        GFL_TextRendererDrawToBitmapEx(BmpWin_GetBitmap(wk->sysMsgWin), 112, (top + i * 2) * 8,
                                       wk->regulationPrintMsg->value[i], wk->font, color);
    }
    BmpWin_FlushChar(wk->sysMsgWin);
}

// pokebw2's WifiP2PMatch_FreeRulesWindow
static void func_ov099_021bc06c(WIFIP2PMATCH_WORK *wk) {
    func_0201a570(wk->regulationPrintMsg);
    wk->regulationPrintMsg = NULL;
    wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
}

// Shows the wait icon in the message window
// pokebw2's WifiP2PMatch_StartWaitIcon
static void func_ov099_021bc090(WIFIP2PMATCH_WORK *wk) {
    if (wk->waitIcon != NULL) {
        func_0202f038(wk->waitIcon);
        wk->waitIcon = NULL;
    }
    wk->waitIcon = func_0202ee8c(wk->tcbManager, wk->msgWin, 15, 16, WIFIP2PMATCH_HEAP);
}

// Closes the party menu: the icons, the party window and the list
// pokebw2's WifiP2PMatch_ClosePartyMenu
static void func_ov099_021bc0c4(WIFIP2PMATCH_WORK *wk) {
    if (wk->listWin != NULL) {
        func_ov099_021bbd80(wk);
        func_ov099_021bbc0c(wk);
        wk->listWin = func_ov099_021ba8bc(wk->listWin);
        BmpMenuList_Free(wk->list, NULL, NULL);
        ListMenuCore_FreeOptionList(wk->menuOptions);
        wk->menuOptions = NULL;
    }
}

void func_ov099_021bc104(WIFIP2PMATCH_WORK *wk) {
    WifiClubData *parent = wk->parent;
    if (parent->unk44[0] && parent->unk44[1]) {
        func_ov009_02156bc4(1);
        func_ov009_02156330();
        func_ov009_021563c0();
        func_ov099_021c24a4(wk, 42);
        func_ov099_021c2864(wk->myStatus, TRUE);
    } else {
        func_ov099_021c2864(wk->myStatus, FALSE);
    }
}


// State 0x41: connected. The side that called asks what to do; the other waits
// pokebw2's WifiP2PMatch_StateConnected
static int func_ov099_021bc14c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_0203d1e8(func_020399a8(), 0);
    if (!func_0203d378()) {
        wk->result = 3;
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_095, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_PLAY_YES_NO_START;
    } else {
        wk->result = 4;
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_126, FALSE);
        func_ov099_021bc090(wk);
        func_02039c68(func_020399a8(), WIFIP2PMATCH_TIMING_SEQ, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    }
    return seq;
}

// State 0x42
// pokebw2's WifiP2PMatch_StatePlayYesNoStart
static int func_ov099_021bc1c4(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    func_ov099_021ba828(wk);
    wk->seq = WIFIP2PMATCH_SEQ_PLAY_YES_NO;
    return seq;
}

// State 0x43: play with the friend, or leave them
// pokebw2's WifiP2PMatch_StatePlayYesNo
static int func_ov099_021bc1e8(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_GAME_MODE_MESSAGE;
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_CANCEL_RESET;
    }
    func_ov099_021ba8ec(wk);
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x44: the parent asks what to play; the child waits
// pokebw2's WifiP2PMatch_StateGameModeMessage
static int func_ov099_021bc228(WIFIP2PMATCH_WORK *wk, int seq) {
    func_0203d1e8(func_020399a8(), 0);
    if (func_0203d378()) {
        wk->result = 3;
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_096, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_MENU_OPEN;
    } else {
        wk->result = 4;
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
        func_ov099_021bc090(wk);
        func_02039c68(func_020399a8(), WIFIP2PMATCH_TIMING_SEQ, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    }
    return seq;
}

// State 0x45
// pokebw2's WifiP2PMatch_StateGameModeMenuOpen
static int func_ov099_021bc29c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    func_ov099_021bad50(wk);
    wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_MENU;
    return seq;
}

static int func_ov099_021bc2c0(WIFIP2PMATCH_WORK *wk, int seq) {
    intptr_t ret;

    PrintWindow_Flush(&wk->listPrintWindow, wk->printQueue);
    ret = BmpMenuList_Update(wk->list);
    switch (ret) {
    case 4:
    case 5:
        if (!wk->parent->unk46) {
            return seq;
        }
        break;
    }
    if (ret != BMPMENULIST_NULL) {
        func_ov099_021ba8ec(wk);
    }
    switch (ret) {
    case BMPMENULIST_NULL:
        return seq;
    case BMPMENULIST_CANCEL:
        wk->sendSeq = 0x9b;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
        break;
    case 4:
        if (wk->parent->unk44[1] == 0) {
            func_ov099_021baab8(wk, Bank0416_Text_140, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_KEY_WAIT;
        } else {
            wk->sendSeq = WIFIP2PMATCH_SEQ_ASK_VCHAT;
            wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
        }
        break;
    case 5:
        if (wk->parent->unk44[1] == 0) {
            func_ov099_021baab8(wk, Bank0416_Text_140, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_KEY_WAIT;
        } else if (!canPlayerExchangePhotos() && func_0202eb0c()) {
            func_ov099_021baab8(wk, Bank0416_Text_149, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_NO_CAMERA_YES_NO_START;
        } else {
            wk->sendSeq = WIFIP2PMATCH_SEQ_ASK_TVT;
            wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
        }
        break;
    case 6:
        if (!func_ov099_021bddd0(wk)) {
            func_ov099_021baab8(wk, Bank0416_Text_121, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_KEY_WAIT;
        } else if (wk->parent->unk44[0] != wk->parent->unk44[1]) {
            wk->unk1352 = 6;
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_DIFF_MSG;
        } else {
            wk->sendSeq = WIFIP2PMATCH_SEQ_ASK_TRADE;
            wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
        }
        break;
    case 7:
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
        break;
    }
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[WIFIP2PMATCH_MENU_STATUS]);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    return seq;
}


// State 0xa4
// pokebw2's WifiP2PMatch_StateNoCameraYesNoStart
static int func_ov099_021bc43c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021b9b64(&wk->room);
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_NO_CAMERA_YES_NO;
    }
    return seq;
}

// State 0xa5: use the Xtransceiver without the camera?
// pokebw2's WifiP2PMatch_StateNoCameraYesNo
static int func_ov099_021bc468(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret = func_ov099_021ba858(wk);

    func_ov099_021b9b64(&wk->room);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_ASK_TVT;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_TALK_END_TIMING;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0xa6
// pokebw2's WifiP2PMatch_StateAnswerNoCameraYesNoStart
static int func_ov099_021bc4a8(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021b9b64(&wk->room);
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ANSWER_NO_CAMERA_YES_NO;
    }
    return seq;
}

// State 0xa7: the same question for the side that was asked
// pokebw2's WifiP2PMatch_StateAnswerNoCameraYesNo
static int func_ov099_021bc4d4(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret = func_ov099_021ba858(wk);

    func_ov099_021b9b64(&wk->room);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->seq = WIFIP2PMATCH_SEQ_SEND_START_GAME;
        func_ov099_021ba8ec(wk);
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINED;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0x47: a key after a message, then the question of what to play again
// pokebw2's WifiP2PMatch_StateGameModeKeyWait
static int func_ov099_021bc51c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys()) {
        wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_MESSAGE;
    }
    return seq;
}

// States 0x4b to 0x4d: the child is asked whether to play what the parent chose; the parent waits
// pokebw2's WifiP2PMatch_AskGameMode
static int func_ov099_021bc544(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 msgs[] = { Bank0416_Text_097, Bank0416_Text_098, Bank0416_Text_099 };

    if (!func_0203d378()) {
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, msgs[wk->modeSeq - WIFIP2PMATCH_SEQ_ASK_VCHAT], FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_ANSWER_YES_NO_START;
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
        func_ov099_021bc090(wk);
        func_02039c68(func_020399a8(), WIFIP2PMATCH_TIMING_SEQ, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    }
    return seq;
}

// State 0x4b: voice chat
// pokebw2's WifiP2PMatch_StateAskVChat
static int func_ov099_021bc5c0(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->modeSeq = WIFIP2PMATCH_SEQ_ASK_VCHAT;
    return func_ov099_021bc544(wk, seq);
}

// State 0x4c: the Xtransceiver
// pokebw2's WifiP2PMatch_StateAskTVT
static int func_ov099_021bc5d4(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->modeSeq = WIFIP2PMATCH_SEQ_ASK_TVT;
    return func_ov099_021bc544(wk, seq);
}

// State 0x4d: a trade
// pokebw2's WifiP2PMatch_StateAskTrade
static int func_ov099_021bc5e8(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->modeSeq = WIFIP2PMATCH_SEQ_ASK_TRADE;
    return func_ov099_021bc544(wk, seq);
}

// State 0x4f
// pokebw2's WifiP2PMatch_StateAnswerYesNoStart
static int func_ov099_021bc5fc(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    func_ov099_021ba828(wk);
    wk->seq = WIFIP2PMATCH_SEQ_ANSWER_YES_NO;
    return seq;
}

// State 0x50: the child's answer. A trade needs two Pokémon, and the Xtransceiver asks first on a DSi that can't
// send photos
// pokebw2's WifiP2PMatch_StateAnswerYesNo
static int func_ov099_021bc620(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        if (!func_ov099_021bddd0(wk) && wk->modeSeq == WIFIP2PMATCH_SEQ_ASK_TRADE) {
            func_ov099_021baab8(wk, Bank0416_Text_121, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_TRADE_REFUSED_KEY_WAIT;
        } else if (!canPlayerExchangePhotos() && func_0202eb0c() && wk->modeSeq == WIFIP2PMATCH_SEQ_ASK_TVT) {
            func_ov099_021baab8(wk, Bank0416_Text_149, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_ANSWER_NO_CAMERA_YES_NO_START;
        } else {
            wk->seq = WIFIP2PMATCH_SEQ_SEND_START_GAME;
            func_ov099_021ba8ec(wk);
        }
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINED;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0x96: a key after the message that a trade needs two Pokémon
// pokebw2's WifiP2PMatch_StateTradeRefusedKeyWait
static int func_ov099_021bc6b0(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (!GCTX_HIDGetPressedKeys()) {
        return seq;
    }
    if (wk->result == 3 || wk->result == 4) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINED;
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_UNABLE_MSG;
    }
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x90: the offer was declined
// pokebw2's WifiP2PMatch_StateDeclined
static int func_ov099_021bc6f4(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_0203d378()) {
        func_ov099_021baab8(wk, Bank0416_Text_115, FALSE);
        func_02039c68(func_020399a8(), WIFIP2PMATCH_TIMING_SEQ, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_114, FALSE);
        wk->timer = 120;
        wk->seq = WIFIP2PMATCH_SEQ_DECLINED_WAIT;
    }
    return seq;
}

// State 0x8f
// pokebw2's WifiP2PMatch_StateDeclinedWait
static int func_ov099_021bc73c(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->timer--;
    func_ov099_021ba928(wk);
    if (wk->timer < 0) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_RETURN_FROM_GAME;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0x52
// pokebw2's WifiP2PMatch_StateSendStartGame
static int func_ov099_021bc76c(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->sendSeq = WIFIP2PMATCH_SEQ_START_GAME;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x53
// pokebw2's WifiP2PMatch_StateSendCancelReset
static int func_ov099_021bc784(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->sendSeq = WIFIP2PMATCH_SEQ_CANCEL_RESET;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x85
// pokebw2's WifiP2PMatch_StateSendRefusedMsg
static int func_ov099_021bc79c(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->sendSeq = WIFIP2PMATCH_SEQ_REFUSED_MSG;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x51: both agreed. The voice chat goes on in the room; the Xtransceiver and a trade end the proc with their
// game mode as the Wi-Fi Club's next mode
// pokebw2's WifiP2PMatch_StateStartGame
static int func_ov099_021bc7b4(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 gameMode;

    func_ov099_021c282c(wk->myStatus);
    wk->unk1351 = 0;
    switch (wk->modeSeq) {
    case WIFIP2PMATCH_SEQ_ASK_VCHAT:
        gameMode = WIFIP2PMATCH_GAMEMODE_VCHAT;
        break;
    case WIFIP2PMATCH_SEQ_ASK_TVT:
        gameMode = WIFIP2PMATCH_GAMEMODE_TVT;
        break;
    case WIFIP2PMATCH_SEQ_ASK_TRADE:
        gameMode = WIFIP2PMATCH_GAMEMODE_TRADE;
        break;
    }
    switch (wk->modeSeq) {
    case WIFIP2PMATCH_SEQ_ASK_VCHAT:
        func_ov099_021bc104(wk);
        func_ov099_021baadc(wk, Bank0416_Text_123, FALSE);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, WIFIP2PMATCH_GAMEMODE_VCHAT);
        func_ov099_021c24a4(wk, 0);
        wk->unk1355 = 0;
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_START;
        break;
    case WIFIP2PMATCH_SEQ_ASK_TVT:
        func_ov099_021ba8ec(wk);
        wk->endSeq = gameMode;
        seq = 2;
        break;
    case WIFIP2PMATCH_SEQ_ASK_TRADE:
        func_ov099_021bc104(wk);
        func_ov099_021ba8ec(wk);
        wk->endSeq = gameMode;
        seq = 2;
        break;
    }
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, gameMode);
    return seq;
}

// State 0x4e: opens the battle rule menu
// pokebw2's WifiP2PMatch_StateBattleSetupMenuStart
static int func_ov099_021bc850(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bb954(wk, TRUE);
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU;
    return seq;
}

// State 0x54: the battle rule menu. Its options' values are the states to go to
// pokebw2's WifiP2PMatch_StateBattleSetupMenu
static int func_ov099_021bc868(WIFIP2PMATCH_WORK *wk, int seq) {
    s32 ret;

    PrintWindow_Flush(&wk->listPrintWindow, wk->printQueue);
    ret = (s32)BmpMenuList_Update(wk->list);
    if (ret == BMPMENULIST_NULL) {
        return seq;
    }
    func_ov099_021ba8ec(wk);
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[0]);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    if (ret == BMPMENULIST_CANCEL) {
        if (wk->result == 3 || wk->result == 4) {
            func_ov099_021ba874(wk, wk->friendNo - 1);
            func_ov099_021baab8(wk, Bank0416_Text_096, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_GAME_MODE_MENU_OPEN;
        } else {
            wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_MENU_START;
        }
    } else {
        if (wk->result == 3 || wk->result == 4) {
            wk->seq = ret;
        } else if (ret == WIFIP2PMATCH_SEQ_BATTLE_SETUP_DECIDE) {
            wk->seq = WIFIP2PMATCH_SEQ_BATTLE_RECRUIT_START;
        } else {
            wk->seq = ret;
        }
    }
    return seq;
}

// State 0x55: opens the battle mode menu
// pokebw2's WifiP2PMatch_StateBattleModeMenuStart
static int func_ov099_021bc940(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bb9c8(wk);
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_MODE_MENU;
    return seq;
}

int func_ov099_021bc958(WIFIP2PMATCH_WORK *wk, int seq) {
    intptr_t ret;

    PrintWindow_Flush(&wk->listPrintWindow, wk->printQueue);
    ret = BmpMenuList_Update(wk->list);
    if (ret != BMPMENULIST_CANCEL) {
        if (ret == BMPMENULIST_NULL) {
            return seq;
        }
        wk->battleMode = ret;
        wk->parent->unk21 = ret;
    }
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
    func_ov099_021ba8ec(wk);
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[1]);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    return seq;
}


// State 0x57: opens the battle rule (no restrictions or flat) menu
// pokebw2's WifiP2PMatch_StateBattleRuleMenuStart
static int func_ov099_021bc9fc(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bb9f4(wk);
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_RULE_MENU;
    return seq;
}

int func_ov099_021bca14(WIFIP2PMATCH_WORK *wk, int seq) {
    intptr_t ret;

    PrintWindow_Flush(&wk->listPrintWindow, wk->printQueue);
    ret = BmpMenuList_Update(wk->list);
    if (ret != BMPMENULIST_CANCEL) {
        if (ret == BMPMENULIST_NULL) {
            return seq;
        }
        wk->battleRule = ret;
        wk->parent->unk22 = ret;
    }
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
    func_ov099_021ba8ec(wk);
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[2]);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    return seq;
}


// State 0x59: opens the Wonder Launcher menu
// pokebw2's WifiP2PMatch_StateShooterMenuStart
static int func_ov099_021bcab8(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bba20(wk);
    wk->seq = WIFIP2PMATCH_SEQ_SHOOTER_MENU;
    return seq;
}

// State 0x5a: the Wonder Launcher menu
// pokebw2's WifiP2PMatch_StateShooterMenu
static int func_ov099_021bcad0(WIFIP2PMATCH_WORK *wk, int seq) {
    intptr_t ret;

    PrintWindow_Flush(&wk->listPrintWindow, wk->printQueue);
    ret = BmpMenuList_Update(wk->list);
    switch (ret) {
    case BMPMENULIST_NULL:
        return seq;
    default:
        wk->parent->unk20 = 1 - ret;
        break;
    case BMPMENULIST_CANCEL:
        break;
    }
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
    func_ov099_021ba8ec(wk);
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[3]);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    return seq;
}

// Checks the player's party against a regulation
// pokebw2's WifiP2PMatch_CheckParty
static u32 func_ov099_021bcb70(Regulation *regulation, GameData *gameData, u32 *flags) {
    *flags = 0;
    return func_0201a090(regulation, GameData_GetParty(gameData), flags);
}

// The Battle Box as a new party, or NULL when it is empty
// pokebw2's WifiP2PMatch_CreateBattleBoxParty
static PokeParty *func_ov099_021bcb8c(GameData *gameData) {
    BattleBoxSave *battleBox = getBattleBox(GameData_GetSaveControl(gameData));

    if (func_0200bba8(battleBox) == FALSE) {
        return NULL;
    }
    return convertBoxedPokeSetToParty(battleBox, WIFIP2PMATCH_HEAP);
}

// pokebw2's WifiP2PMatch_FreeBattleBoxParty
static void func_ov099_021bcbb0(PokeParty *party) {
    if (party != NULL) {
        GFL_HeapFree(party);
    }
}

// Checks the Battle Box's party against a regulation; every flag is set when there is none
// pokebw2's WifiP2PMatch_CheckBattleBoxParty
static u32 func_ov099_021bcbbc(Regulation *regulation, PokeParty *party, u32 *flags) {
    u32 ret = 4;

    *flags = 0;
    if (party != NULL) {
        ret = func_0201a090(regulation, party, flags);
    } else {
        *flags = 0xffffffff;
    }
    return ret;
}

u32 func_ov099_021bcbd8(WIFIP2PMATCH_WORK *wk, int which) {
    u32 battleMode;
    u32 battleRule;
    u32 shooter;
    u32 code;

    switch (which) {
    case 0:
    default:
        battleMode = wk->battleMode;
        battleRule = wk->battleRule;
        shooter = wk->parent->unk48;
        break;
    case 1:
        battleMode = wk->parent->unk21;
        battleRule = wk->parent->unk22;
        shooter = wk->parent->unk20;
        break;
    }
    code = data_ov099_021c3a30[battleMode + battleRule * 4];
    if (wk->regulation != NULL) {
        GFL_HeapFree(wk->regulation);
    }
    wk->regulation = func_0201a354(code, WIFIP2PMATCH_HEAP);
    if (shooter) {
        code |= 0x80000000;
    }
    return code;
}


// pokebw2's WifiP2PMatch_LoadMyRegulation
static u32 func_ov099_021bcc4c(WIFIP2PMATCH_WORK *wk) {
    return func_ov099_021bcbd8(wk, 0);
}

// Sets the battle mode, the rule and the Wonder Launcher flag from a regulation code
// pokebw2's WifiP2PMatch_SetRegulationCode
static void func_ov099_021bcc58(WIFIP2PMATCH_WORK *wk, u32 regulationCode) {
    int i;

    wk->parent->unk48 = (regulationCode & 0x80000000) ? TRUE : FALSE;
    regulationCode &= 0xfffffff;
    for (i = 0; i < NELEMS(data_ov099_021c3a30); i++) {
        if (regulationCode == data_ov099_021c3a30[i]) {
            wk->battleMode = i % 4;
            wk->battleRule = i / 4;
        }
    }
}

static int func_ov099_021bccb8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->result == 3 || wk->result == 1) {
        func_ov099_021bcbd8(wk, 1);
    } else {
        func_ov099_021bcbd8(wk, 0);
    }
    if (func_ov099_021bddf4(wk) == 0) {
        func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
        wk->seq = 0x63;
        return seq;
    }
    wk->unk1352 = 7;
    if (wk->parent->unk44[0] != wk->parent->unk44[1]) {
        wk->seq = 0x7d;
    } else {
        wk->seq = 0x81;
    }
    func_ov099_021ba874(wk, wk->friendNo - 1);
    if (wk->result == 3 || wk->result == 4) {
        func_ov099_021baab8(wk, Bank0416_Text_014, FALSE);
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
    }
    func_ov099_021bc090(wk);
    return seq;
}


// State 0x63: shows what of the party breaks the rules
// pokebw2's WifiP2PMatch_StateRegulationFailParty
static int func_ov099_021bcd58(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 flags;

    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021ba8ec(wk);
        func_ov099_021bcb70(wk->regulation, wk->gameData, &flags);
        func_ov099_021bbeb4(wk, flags, wk->parent->unk48, 2);
        wk->seq = WIFIP2PMATCH_SEQ_REGULATION_FAIL_BATTLE_BOX;
    }
    return seq;
}

// State 0x77: shows what of the Battle Box breaks the rules, if there is one
// pokebw2's WifiP2PMatch_StateRegulationFailBattleBox
static int func_ov099_021bcdb0(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 flags;

    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021bc06c(wk);
        if (wk->battleBoxParty != NULL) {
            func_ov099_021bcbbc(wk->regulation, wk->battleBoxParty, &flags);
            func_ov099_021bbeb4(wk, flags, wk->parent->unk48, 3);
            wk->seq = WIFIP2PMATCH_SEQ_REGULATION_FAIL_END;
        } else {
            switch (wk->result) {
            case 3:
                wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
                break;
            case 2:
                wk->sendSeq = WIFIP2PMATCH_SEQ_UNABLE_MSG;
                wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
                break;
            case 4:
                wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINED;
                wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
                break;
            default:
                wk->seq = WIFIP2PMATCH_SEQ_BACK_TO_ROOM;
                break;
            }
        }
    }
    return seq;
}

// State 0x78: closes the rules window and goes back by who asked
// pokebw2's WifiP2PMatch_StateRegulationFailEnd
static int func_ov099_021bce30(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021bc06c(wk);
        func_ov099_021ba8ec(wk);
        switch (wk->result) {
        case 3:
            wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
            break;
        case 2:
            wk->sendSeq = WIFIP2PMATCH_SEQ_UNABLE_MSG;
            wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
            break;
        case 4:
            wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINED;
            wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
            break;
        default:
            wk->seq = WIFIP2PMATCH_SEQ_BACK_TO_ROOM;
            break;
        }
    }
    return seq;
}

// State 0x5c: shows the rules of the battle
// pokebw2's WifiP2PMatch_StateRulesShow
static int func_ov099_021bce80(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bcbd8(wk, 1);
    func_ov099_021bbeb4(wk, 0, wk->parent->unk20, 0);
    wk->seq = WIFIP2PMATCH_SEQ_RULES_SHOW_WAIT;
    return seq;
}

// State 0x5d
// pokebw2's WifiP2PMatch_StateRulesShowWait
static int func_ov099_021bceac(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021bc06c(wk);
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU_START;
    }
    return seq;
}

// State 0x5e: the child sees the battle the parent chose, the parent waits
// pokebw2's WifiP2PMatch_StateBattleOfferShow
static int func_ov099_021bcecc(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_0203d378()) {
        func_ov099_021bcc4c(wk);
        func_ov099_021bbeb4(wk, 0, wk->parent->unk48, 1);
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021ba978(wk, Bank0416_Text_105 + wk->battleMode + wk->battleRule * 4,
                                      FALSE, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_MSG_WAIT;
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
        func_ov099_021bc090(wk);
        func_02039c68(func_020399a8(), 0x1b, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    }
    return seq;
}

// State 0x7a
// pokebw2's WifiP2PMatch_StateBattleOfferMsgWait
static int func_ov099_021bcf4c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_KEY_WAIT;
    return seq;
}

// State 0x7b
// pokebw2's WifiP2PMatch_StateBattleOfferKeyWait
static int func_ov099_021bcf6c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021bc06c(wk);
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_YES_NO_GO;
    }
    return seq;
}

// State 0x7c
// pokebw2's WifiP2PMatch_StateBattleOfferYesNoGo
static int func_ov099_021bcf8c(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_YES_NO_START;
    return seq;
}

// State 0x5f
// pokebw2's WifiP2PMatch_StateUnused5F
static int func_ov099_021bcf98(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// State 0x60
// pokebw2's WifiP2PMatch_StateBattleOfferYesNoStart
static int func_ov099_021bcf9c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba828(wk);
    wk->seq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_YES_NO;
    return seq;
}

// State 0x61: whether the child accepts the battle
// pokebw2's WifiP2PMatch_StateBattleOfferYesNo
static int func_ov099_021bcfb4(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_CHECK_PARTY;
        func_ov099_021ba8ec(wk);
    } else {
        if (wk->result == 1 || wk->result == 2) {
            wk->sendSeq = WIFIP2PMATCH_SEQ_CANCEL_RESET;
            func_ov099_021baab8(wk, Bank0416_Text_074, FALSE);
        } else {
            wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINED;
        }
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0x8c
// pokebw2's WifiP2PMatch_StatePlayAgainKeyWait
static int func_ov099_021bd014(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys()) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_PLAY_AGAIN_ASK;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0x62: the child's parties against the rules
// pokebw2's WifiP2PMatch_StateBattleOfferCheckParty
static int func_ov099_021bd038(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bcc4c(wk);
    if (!func_ov099_021bddf4(wk)) {
        switch (wk->result) {
        case 3:
            wk->seq = WIFIP2PMATCH_SEQ_REGULATION_FAIL_KEY_WAIT;
            func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
            return seq;
        case 2:
        case 4:
            wk->seq = WIFIP2PMATCH_SEQ_REGULATION_FAIL_PARTY;
            func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
            return seq;
        default:
            wk->seq = WIFIP2PMATCH_SEQ_TRADE_REFUSED_KEY_WAIT;
            func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
            return seq;
        }
    }
    wk->sendSeq = WIFIP2PMATCH_SEQ_PARTY_SELECT_START;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x9d
// pokebw2's WifiP2PMatch_StateRegulationFailKeyWait
static int func_ov099_021bd0bc(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (!GCTX_HIDGetPressedKeys()) {
        return seq;
    }
    wk->sendSeq = WIFIP2PMATCH_SEQ_PLAY_AGAIN_ASK;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x64: the party is chosen at once when only it passes the rules and there is no Battle Box
// pokebw2's WifiP2PMatch_StatePartySelectStart
static int func_ov099_021bd0f0(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 flags;
    int id = 0;

    if (!func_0203d378()) {
        id = 1;
    }
    if (func_ov099_021bcbbc(wk->regulation, wk->battleBoxParty, &flags) != 0 &&
        wk->battleBoxParty == NULL) {
        PokeParty_Copy(GameData_GetParty(wk->gameData), wk->parent->parties[id]);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_SEND;
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_113, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU_START;
    }
    return seq;
}

// State 0x65: opens the party or Battle Box menu
// pokebw2's WifiP2PMatch_StatePartyMenuStart
static int func_ov099_021bd15c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    wk->unk1354 = 2;
    func_ov099_021bbe7c(wk);
    func_ov099_021bbd0c(wk, GameData_GetParty(wk->gameData), wk->battleBoxParty);
    wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU;
    return seq;
}

// State 0x66: the party or Battle Box menu
// pokebw2's WifiP2PMatch_StatePartyMenu
static int func_ov099_021bd1a4(WIFIP2PMATCH_WORK *wk, int seq) {
    intptr_t ret;
    u32 flags;
    u16 listTop;
    u16 cursorRow;
    int id = 0;

    PrintWindow_Flush(&wk->listPrintWindow, wk->printQueue);
    ret = BmpMenuList_Update(wk->list);
    func_020202fc(wk->list, &listTop, &cursorRow);
    if (cursorRow != wk->unk1354) {
        wk->unk1354 = cursorRow;
        if (cursorRow == 0) {
            func_ov099_021bba4c(wk, GameData_GetParty(wk->gameData));
            func_ov099_021bbdd0(wk, TRUE);
        } else {
            func_ov099_021bba4c(wk, wk->battleBoxParty);
            func_ov099_021bbdd0(wk, FALSE);
        }
    }
    if (!func_0203d378()) {
        id = 1;
    }
    switch (ret) {
    case BMPMENULIST_NULL:
        return seq;
    case BMPMENULIST_CANCEL:
        return seq;
    case 0:
        if (func_ov099_021bcb70(wk->regulation, wk->gameData, &flags) != 0) {
            func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_PARTY;
            func_ov099_021bc0c4(wk);
            return seq;
        }
        PokeParty_Copy(GameData_GetParty(wk->gameData), wk->parent->parties[id]);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_SEND;
        break;
    case 1:
        if (func_ov099_021bcbbc(wk->regulation, wk->battleBoxParty, &flags) != 0) {
            func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_BATTLE_BOX;
            func_ov099_021bc0c4(wk);
            return seq;
        }
        PokeParty_Copy(wk->battleBoxParty, wk->parent->parties[id]);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_SEND;
        break;
    }
    func_ov099_021bc0c4(wk);
    func_ov099_021ba8ec(wk);
    return seq;
}

// State 0x67: sends the party
// pokebw2's WifiP2PMatch_StatePartySend
static int func_ov099_021bd318(WIFIP2PMATCH_WORK *wk, int seq) {
    int id = 0;

    if (!func_0203d378()) {
        id = 1;
    }
    if (func_0203d3f8(func_020399a8(), 0xff, 0xa07, PokeParty_GetSaveDataSize(), wk->parent->parties[id], 0, FALSE,
                      TRUE)) {
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
        func_ov099_021bc090(wk);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_SEND_TIMING;
    }
    return seq;
}

// State 0x98
// pokebw2's WifiP2PMatch_StatePartySendTiming
static int func_ov099_021bd380(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_ov099_021ba928(wk)) {
        func_02039c68(func_020399a8(), 0x11, 10);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_SEND_WAIT;
    }
    return seq;
}

// State 0x68: both parties are sent. Says how many Pokémon to choose
// pokebw2's WifiP2PMatch_StatePartySendWait
static int func_ov099_021bd3a8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_02039cb4(func_020399a8(), 0x11, 10)) {
        u32 min = func_0200ae54(wk->regulation, REGULATION_PARAM_2);
        u32 max = func_0200ae54(wk->regulation, REGULATION_PARAM_3);

        u32 msgId;

        if (min == 3 && max == 3) {
            msgId = Bank0416_Text_146;
        } else if (min == 4 && max == 4) {
            msgId = Bank0416_Text_147;
        } else if (min == 6 && max == 6) {
            msgId = Bank0416_Text_148;
        } else {
            msgId = Bank0416_Text_145;
        }
        func_ov099_021baab8(wk, msgId, FALSE);
        func_ov099_021bc090(wk);
        wk->timer = 120;
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_START_WAIT;
    }
    return seq;
}

// State 0x69
// pokebw2's WifiP2PMatch_StateBattleStartWait
static int func_ov099_021bd41c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_ov099_021ba928(wk)) {
        if (wk->timer-- < 0) {
            wk->seq = WIFIP2PMATCH_SEQ_BATTLE_START;
        }
    }
    return seq;
}

// State 0x6a: leaves for the battle. Ends the proc
// pokebw2's WifiP2PMatch_StateBattleStart
static int func_ov099_021bd444(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 gameMode;

    func_ov099_021c282c(wk->myStatus);
    wk->unk1351 = 0;
    {
        u32 gameModes[8] = { 7, 8, 9, 10, 11, 12, 13, 14 };

        gameMode = gameModes[wk->battleRule + wk->battleMode * 2];
    }
    wk->endSeq = gameMode;
    func_ov099_021bc104(wk);
    func_ov099_021ba8ec(wk);
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, gameMode);
    return 2;
}

// State 0x6b: shows what of the party breaks the rules, then the menu again
// pokebw2's WifiP2PMatch_StatePartyMenuFailParty
static int func_ov099_021bd4a8(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 flags;

    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021ba8ec(wk);
        func_ov099_021bcb70(wk->regulation, wk->gameData, &flags);
        func_ov099_021bbeb4(wk, flags, wk->parent->unk48, 2);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_END;
    }
    return seq;
}

// State 0x6c: the same for the Battle Box
// pokebw2's WifiP2PMatch_StatePartyMenuFailBattleBox
static int func_ov099_021bd500(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 flags;

    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021ba8ec(wk);
        func_ov099_021bcbbc(wk->regulation, wk->battleBoxParty, &flags);
        func_ov099_021bbeb4(wk, flags, wk->parent->unk48, 3);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU_FAIL_END;
    }
    return seq;
}

// State 0x6d
// pokebw2's WifiP2PMatch_StatePartyMenuFailEnd
static int func_ov099_021bd558(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021bc06c(wk);
        func_ov099_021baab8(wk, Bank0416_Text_113, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_PARTY_MENU_START;
    }
    return seq;
}

// State 0x6e: after a battle, the child is asked to play again and the parent waits
// pokebw2's WifiP2PMatch_StatePlayAgainAsk
static int func_ov099_021bd584(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (!func_0203d378()) {
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_119, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_PLAY_AGAIN_YES_NO_START;
    } else {
        func_02039c68(func_020399a8(), 0x1b, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    }
    return seq;
}

// State 0x6f
// pokebw2's WifiP2PMatch_StatePlayAgainYesNoStart
static int func_ov099_021bd5d8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    func_ov099_021ba828(wk);
    wk->seq = WIFIP2PMATCH_SEQ_PLAY_AGAIN_YES_NO;
    return seq;
}

// State 0x70: whether to play again
// pokebw2's WifiP2PMatch_StatePlayAgainYesNo
static int func_ov099_021bd5fc(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_GAME_MODE_MESSAGE;
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_CANCEL_RESET;
    }
    func_ov099_021ba8ec(wk);
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x48: both sides go to the state that was sent
// pokebw2's WifiP2PMatch_StateSyncSeqWait
static int func_ov099_021bd63c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba928(wk);
    if (func_02039cb4(func_020399a8(), 0x1a, 10)) {
        if (wk->recvSeq != 0) {
            wk->seq = wk->recvSeq;
        } else {
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
        }
        wk->recvSeq = 0;
    }
    return seq;
}

// State 0x49
// pokebw2's WifiP2PMatch_StateSyncSeqTimingWait
static int func_ov099_021bd678(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba928(wk);
    if (func_02039cb4(func_020399a8(), 0x1b, 10)) {
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_WAIT;
    }
    return seq;
}

// State 0x9b
// pokebw2's WifiP2PMatch_StateTalkEndTiming
static int func_ov099_021bd6a0(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    func_02039c68(func_020399a8(), 0x19, 10);
    wk->result = 0;
    wk->seq = WIFIP2PMATCH_SEQ_TALK_END_TIMING_WAIT;
    return seq;
}

// State 0x9c
// pokebw2's WifiP2PMatch_StateTalkEndTimingWait
static int func_ov099_021bd6d0(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_02039cb4(func_020399a8(), 0x19, 10)) {
        return seq;
    }
    func_0203d7b0(0);
    func_0203d7bc(FALSE);
    if (func_0203d378()) {
        func_ov099_021baab8(wk, Bank0416_Text_124, FALSE);
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_125, FALSE);
    }
    wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_START;
    return seq;
}

// State 0xa8
// pokebw2's WifiP2PMatch_StateDeclineReset
static int func_ov099_021bd71c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    func_0203d7b0(0);
    func_0203d7bc(FALSE);
    wk->seq = WIFIP2PMATCH_SEQ_DECLINE_MSG;
    return seq;
}

// State 0xa9
// pokebw2's WifiP2PMatch_StateDeclineMsg
static int func_ov099_021bd744(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->result == 2 || wk->result == 3) {
        func_ov099_021baab8(wk, Bank0416_Text_125, FALSE);
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_124, FALSE);
    }
    wk->result = 0;
    wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_START;
    return seq;
}

// State 0x4a
// pokebw2's WifiP2PMatch_StateCancelReset
static int func_ov099_021bd778(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    func_0203d7b0(0);
    func_0203d7bc(FALSE);
    wk->seq = WIFIP2PMATCH_SEQ_CANCEL_MSG;
    return seq;
}

// State 0x99
// pokebw2's WifiP2PMatch_StateCancelMsg
static int func_ov099_021bd7a0(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->result == 2 || wk->result == 3) {
        func_ov099_021baab8(wk, Bank0416_Text_124, FALSE);
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_125, FALSE);
    }
    wk->result = 0;
    wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_START;
    return seq;
}

// State 0x8d
// pokebw2's WifiP2PMatch_StateGoDisconnect
static int func_ov099_021bd7d4(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
    return seq;
}

// State 0x84
// pokebw2's WifiP2PMatch_StateRefusedMsg
static int func_ov099_021bd7e0(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    if (func_0203d378()) {
        func_ov099_021baab8(wk, Bank0416_Text_115, FALSE);
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_114, FALSE);
    }
    wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_START;
    return seq;
}

// State 0x8e
// pokebw2's WifiP2PMatch_StateDisconnectStart
static int func_ov099_021bd814(WIFIP2PMATCH_WORK *wk, int seq) {
    func_0203d7b0(0);
    func_0203d7bc(FALSE);
    wk->timer = 0;
    wk->seq = WIFIP2PMATCH_SEQ_CUT_CONNECTION;
    return seq;
}

static int func_ov099_021bd838(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 msgId;

    if (wk->parent->unk44[1] != 0) {
        msgId = Bank0416_Text_072;
    } else {
        msgId = Bank0416_Text_073;
    }
    func_ov099_021baab8(wk, msgId, FALSE);
    wk->seq = WIFIP2PMATCH_SEQ_VCHAT_DIFF_YES_NO_START;
    return seq;
}


// State 0x7e
// pokebw2's WifiP2PMatch_StateVChatDiffYesNoStart
static int func_ov099_021bd864(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    func_ov099_021ba828(wk);
    wk->seq = WIFIP2PMATCH_SEQ_VCHAT_DIFF_YES_NO;
    return seq;
}

// State 0x7f
// pokebw2's WifiP2PMatch_StateVChatDiffYesNo
static int func_ov099_021bd888(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_SEND;
    } else {
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_DIFF_NO;
    }
    func_ov099_021ba8ec(wk);
    return seq;
}

// State 0x86
// pokebw2's WifiP2PMatch_StateVChatDiffNo
static int func_ov099_021bd8c0(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->sendSeq = WIFIP2PMATCH_SEQ_CANCEL_RESET;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x80: sends the game mode and the changed voice chat setting
// pokebw2's WifiP2PMatch_StateVChatSend
static int func_ov099_021bd8d8(WIFIP2PMATCH_WORK *wk, int seq) {
    u16 data[2];

    data[0] = func_ov099_021bdd54(wk, wk->myStatus);
    data[1] = 1 - wk->parent->unk46;
    if (func_0203d3a8(func_020399a8(), 0xa03, sizeof(data), data)) {
        wk->seq = WIFIP2PMATCH_SEQ_REGULATION_SEND;
        func_ov099_021c2864(wk->myStatus, data[1]);
    }
    return seq;
}

// State 0x81: sends the regulation code
// pokebw2's WifiP2PMatch_StateRegulationSend
static int func_ov099_021bd924(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 code;

    if (wk->unk1352 == 6) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_ASK_TRADE;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    } else {
        if (wk->result == 3 || wk->result == 1) {
            code = func_ov099_021bcbd8(wk, 1);
        } else {
            code = func_ov099_021bcbd8(wk, 0);
        }
        if (func_0203d3a8(func_020399a8(), 0xa06, sizeof(code), &code)) {
            wk->seq = WIFIP2PMATCH_SEQ_REGULATION_SEND_END;
        }
    }
    return seq;
}

// State 0x82
// pokebw2's WifiP2PMatch_StateRegulationSendEnd
static int func_ov099_021bd97c(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->sendSeq = WIFIP2PMATCH_SEQ_BATTLE_OFFER_SHOW;
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0x8a: the state in sendSeq is sent to the partner, and both sides go to it
// pokebw2's WifiP2PMatch_StateSendSeqTiming
static int func_ov099_021bd994(WIFIP2PMATCH_WORK *wk, int seq) {
    func_02039c68(func_020399a8(), 0x1b, 10);
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ;
    return seq;
}

// State 0x8b
// pokebw2's WifiP2PMatch_StateSendSeq
static int func_ov099_021bd9b4(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_02039cb4(func_020399a8(), 0x1b, 10)) {
        if (func_0203d3a8(func_020399a8(), 0xa05, sizeof(wk->sendSeq), &wk->sendSeq)) {
            wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_WAIT;
        }
    }
    return seq;
}

// State 0x97
// pokebw2's WifiP2PMatch_StateUnableMsg
static int func_ov099_021bd9f4(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    wk->result = 0;
    func_0203d7b0(0);
    func_0203d7bc(FALSE);
    if (func_0203d378()) {
        func_ov099_021baab8(wk, Bank0416_Text_115, FALSE);
    } else {
        func_ov099_021baab8(wk, Bank0416_Text_124, FALSE);
    }
    wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_START;
    return seq;
}

// State 0x79: back from a game. The child is asked to play again and the parent waits
// pokebw2's WifiP2PMatch_StateReturnFromGame
static int func_ov099_021bda3c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_0203d1e8(func_020399a8(), 0);
    if (!func_0203d378()) {
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_119, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_RETURN_YES_NO_START;
    } else {
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
        func_ov099_021bc090(wk);
        func_02039c68(func_020399a8(), 0x1b, 10);
        wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
    }
    return seq;
}

// State 0xaa
// pokebw2's WifiP2PMatch_StateReturnYesNoStart
static int func_ov099_021bdaa4(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    func_ov099_021ba828(wk);
    wk->seq = WIFIP2PMATCH_SEQ_RETURN_YES_NO;
    return seq;
}

// State 0xab
// pokebw2's WifiP2PMatch_StateReturnYesNo
static int func_ov099_021bdac8(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->sendSeq = WIFIP2PMATCH_SEQ_GAME_MODE_MESSAGE;
    } else {
        wk->sendSeq = WIFIP2PMATCH_SEQ_DECLINE_RESET;
    }
    func_ov099_021ba8ec(wk);
    wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    return seq;
}

// State 0xac
// pokebw2's WifiP2PMatch_StateReturnTimingWait
static int func_ov099_021bdb08(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_02039cb4(func_020399a8(), 0x18, 10)) {
        return seq;
    }
    wk->seq = WIFIP2PMATCH_SEQ_RETURN_FROM_GAME;
    return seq;
}

// State 0x73: both sides are ready. Starts what was chosen: the voice chat here, the trade and the Xtransceiver by
// ending the proc, a battle by the rules
// pokebw2's WifiP2PMatch_StateGameStart
static int func_ov099_021bdb30(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 flags;
    u16 gameMode = func_ov099_021bdd54(wk, wk->myStatus);

    switch (gameMode) {
    case 4:
        func_0203d798();
        func_ov099_021bc104(wk);
        func_ov099_021c24a4(wk, 0);
        func_ov099_021baadc(wk, Bank0416_Text_123, FALSE);
        wk->unk1355 = 0;
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_START;
        return seq;
    case 5:
        wk->endSeq = gameMode;
        return 2;
    case 6:
        func_ov099_021bc104(wk);
        wk->endSeq = gameMode;
        return 2;
    default:
        func_0203d1e8(func_020399a8(), 0);
        if (func_0203d378()) {
            if (func_ov099_021bcb70(wk->regulation, wk->gameData, &flags) != 0 &&
                func_ov099_021bcbbc(wk->regulation, wk->battleBoxParty, &flags) != 0) {
                func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
                wk->seq = WIFIP2PMATCH_SEQ_GAME_START_FAIL_KEY_WAIT;
                return seq;
            }
            wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_DECIDE;
        } else {
            func_02039c68(func_020399a8(), 0x1b, 10);
            wk->seq = WIFIP2PMATCH_SEQ_SYNC_SEQ_TIMING_WAIT;
        }
        return seq;
    }
}

// State 0x74
// pokebw2's WifiP2PMatch_StateGameStartFailKeyWait
static int func_ov099_021bdc0c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021ba8ec(wk);
        wk->sendSeq = WIFIP2PMATCH_SEQ_CANCEL_RESET;
        wk->seq = WIFIP2PMATCH_SEQ_SEND_SEQ_TIMING;
    }
    return seq;
}

// State 0x75: "Confirm" in the battle rule menu. Starts recruiting for the battle
// pokebw2's WifiP2PMatch_StateBattleRecruitStart
static int func_ov099_021bdc44(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bcbd8(wk, 1);
    if (!func_ov099_021bddf4(wk)) {
        func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_REGULATION_FAIL_PARTY;
        return seq;
    }
    {
        u32 gameModes[8] = { 7, 8, 9, 10, 11, 12, 13, 14 };

        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_RECRUIT,
                                    (u16)gameModes[wk->battleRule + wk->battleMode * 2]);
    }
    func_ov099_021c23c8(wk, -1);
    wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
    func_ov099_021c23d8(wk, TRUE);
    return seq;
}

// pokebw2's sWifiP2PMatchVRAMConfig
static BGSysVRAMConfig data_ov099_021c3aa0 = {
    GX_VRAM_BG_128_A,  GX_VRAM_BGEXTPLTT_NONE,  GX_VRAM_SUB_BG_128_C,        GX_VRAM_SUB_BGEXTPLTT_NONE,
    GX_VRAM_OBJ_128_B, GX_VRAM_OBJEXTPLTT_NONE, GX_VRAM_SUB_OBJ_128_D,       GX_VRAM_SUB_OBJEXTPLTT_NONE,
    GX_VRAM_TEX_NONE,  GX_VRAM_TEXPLTT_NONE,    GX_OBJVRAMMODE_CHAR_1D_128K, GX_OBJVRAMMODE_CHAR_1D_128K,
};

// wifiHeapSize is set for the machine before each use
// pokebw2's sWifiP2PMatchNetInit
static GFLNetInitData data_ov099_021c3b9c = {
    NULL,
    0,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    { .wifiError = { { 0 }, func_ov099_021bdcf4 } },
    func_ov099_021bdce0,
    func_ov099_021bdcd0,
    0x5e400,
    { 1, 0, 0, 0, 0x80, 0x13, 0, 0 },
    HEAPID_USER,
    0xd,
    0xf,
    0xd,
    0xf0,
    0,
    2,
    0x50,
    0x10,
    1,
    0,
    GFL_NET_TYPE_WIFI,
    1,
    0xa,
    { 0x2c, 1, 0, 0 },
    0,
    0,
};

// pokebw2's WifiP2PMatch_GetUserData
static DWCUserData *func_ov099_021bdcd0(void *work) {
    WIFIP2PMATCH_WORK *wk = work;

    return func_02009860(func_0201283c(wk->gameData));
}

// pokebw2's WifiP2PMatch_GetFriendData
static DWCFriendData *func_ov099_021bdce0(void *work) {
    WIFIP2PMATCH_WORK *wk = work;

    return func_020099c8(func_0201283c(wk->gameData), 0);
}

// pokebw2's WifiP2PMatch_OnFriendCode
static void func_ov099_021bdcf4(int code, int param, void *work) {
    WIFIP2PMATCH_WORK *wk = work;

    func_02009c24(func_0201283c(wk->gameData), code, param);
}

// Starts the network with the Wi-Fi heap of the machine, or only gives it the init data again
// pokebw2's WifiP2PMatch_NetInit
static void func_ov099_021bdd0c(WIFIP2PMATCH_WORK *wk, BOOL init) {
    if (OS_IsRunOnTwl()) {
        data_ov099_021c3b9c.wifiHeapSize = 0x79000;
    } else {
        data_ov099_021c3b9c.wifiHeapSize = 0x5e400;
    }
    if (init) {
        func_0203cbac(&data_ov099_021c3b9c, NULL, wk);
    } else {
        func_0203d844(&data_ov099_021c3b9c);
    }
}

// pokebw2's WifiP2PMatch_GetStatus
static u32 func_ov099_021bdd48(WIFIP2PMATCH_WORK *wk, const WIFI_STATUS *status) {
    return func_ov099_021c27d0(status);
}

// pokebw2's WifiP2PMatch_GetGameMode
static u32 func_ov099_021bdd54(WIFIP2PMATCH_WORK *wk, const WIFI_STATUS *status) {
    return func_ov099_021c27d8(status);
}

// Whether a game mode is one of the battles
// pokebw2's WifiP2PMatch_IsBattleGameMode
static BOOL func_ov099_021bdd60(u32 gameMode) {
    switch (gameMode) {
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiP2PMatch_IsPlayGameMode
static BOOL func_ov099_021bdd98(int gameMode) {
    if (gameMode < 15 && gameMode > 3) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiP2PMatch_IsRecruitStatus
static BOOL func_ov099_021bdda8(u32 status) {
    if (status == WIFIP2PMATCH_STATUS_RECRUIT) {
        return TRUE;
    }
    return FALSE;
}

// Whether a status is that of a battle being played
// pokebw2's WifiP2PMatch_IsBattlePlaying
static BOOL func_ov099_021bddb4(u32 status, u32 gameMode) {
    if (func_ov099_021bdd60(gameMode) && status == WIFIP2PMATCH_STATUS_PLAYING) {
        return TRUE;
    }
    return FALSE;
}

// Whether the party has two Pokémon that are not Eggs
// pokebw2's WifiP2PMatch_HasTwoPokemon
static BOOL func_ov099_021bddd0(WIFIP2PMATCH_WORK *wk) {
    GameData_GetBoxSaveAccessor(wk->gameData);
    if (howManyPartyPokesAreNotEggs(GameData_GetParty(wk->gameData)) >= 2) {
        return TRUE;
    }
    return FALSE;
}

// Whether the party or the Battle Box passes the regulation
// pokebw2's WifiP2PMatch_PassesRegulation
static BOOL func_ov099_021bddf4(WIFIP2PMATCH_WORK *wk) {
    u32 flags;

    if (func_ov099_021bcb70(wk->regulation, wk->gameData, &flags) != 0 &&
        func_ov099_021bcbbc(wk->regulation, wk->battleBoxParty, &flags) != 0) {
        return FALSE;
    }
    return TRUE;
}

// The network's side of a status change
// pokebw2's WifiP2PMatch_ChangeNetStatus
static void func_ov099_021bde30(WIFIP2PMATCH_WORK *wk, u32 status, u32 gameMode) {
    if (status == WIFIP2PMATCH_STATUS_WAIT) {
        func_0203d644(FALSE);
        func_ov099_021bdd0c(wk, FALSE);
    } else if (func_ov099_021bddb4(status, gameMode)) {
        func_0203d644(TRUE);
    }
    switch (status) {
    case 2:
    case 5:
        func_0203d1e8(func_020399a8(), 1);
        break;
    case 0:
    case 1:
    case 3:
    case 4:
    case 6:
    default:
        func_0203d1e8(func_020399a8(), 1);
        break;
    }
}

// The screen's graphics, messages and fonts
// pokebw2's WifiP2PMatch_GraphicInit
static void func_ov099_021bde94(WIFIP2PMATCH_WORK *wk) {
    ArcTool *arc;

    if (wk->vblankTcb != NULL) {
        GFL_TCBRemove(wk->vblankTcb);
    }
    GFL_BGSysSetEnabledBGsA(0);
    GFL_BGSysSetEnabledBGsB(0);
    GX_SetVisiblePlane(0);
    GXS_SetVisiblePlane(0);
    arc = GFL_ArcSysCreateFileHandle(0x21, WIFIP2PMATCH_HEAP);
    wk->wordSet = GFL_WordSetSystemCreateDefault(WIFIP2PMATCH_HEAP);
    wk->msgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_0416, WIFIP2PMATCH_HEAP);
    wk->sysMsgData = GFL_MsgSysLoadData(FALSE, ARCID_SYSTEM_MESSAGE, TEXT_BANK_WIFI_ERROR, WIFIP2PMATCH_HEAP);
    wk->font = GFL_FontCreate(ARCID_FONT, 0, 0, FALSE, WIFIP2PMATCH_HEAP);
    wk->smallFont = GFL_FontCreate(ARCID_FONT, 3, 0, FALSE, WIFIP2PMATCH_HEAP);
    GFL_BGSysSetDisplayLayout(1);
    func_ov099_021be030();
    func_ov099_021be040(WIFIP2PMATCH_HEAP);
    BmpWin_InitAllocator(WIFIP2PMATCH_HEAP);
    func_ov099_021be53c(wk, arc);
    wk->vblankTcb = GFL_VBlankTCBAdd(func_ov099_021be028, wk, 1);
    func_ov099_021be5b4();
    func_ov099_021be5b8(wk, arc);
    GFL_BGSysSetBGEnabledA(0x10, TRUE);
    GFL_ArcToolFree(arc);
}

// pokebw2's WifiP2PMatch_GraphicExit
static void func_ov099_021bdf7c(WIFIP2PMATCH_WORK *wk) {
    if (wk->vblankTcb != NULL) {
        GFL_TCBRemove(wk->vblankTcb);
    }
    func_ov099_021bb42c(wk);
    func_ov099_021bb4d0(wk);
    func_ov099_021bb4ec(wk);
    if (func_ov099_021b96cc(&wk->room) == TRUE) {
        func_ov099_021be26c(wk);
    }
    if (func_ov099_021c1a44(wk) == TRUE) {
        func_ov099_021c1ac0(wk);
    }
    func_ov099_021be384(&wk->icon);
    func_0204b084(wk->clActRenderer);
    func_0204b1cc(wk->clActUnit);
    func_0204a5ac();
    func_ov099_021bb0b4(wk);
    func_ov099_021be538(wk);
    BmpWin_FreeAllocator();
    GFL_BGSysFree();
    GFL_MsgDataFree(wk->sysMsgData);
    GFL_MsgDataFree(wk->msgData);
    GFL_WordSetSystemFree(wk->wordSet);
    GFL_FontFree(wk->smallFont);
    GFL_FontFree(wk->font);
}

// pokebw2's WifiP2PMatch_VBlankTask
static void func_ov099_021be028(TCB *tcb, void *data) {
    func_0204a648();
}

// pokebw2's WifiP2PMatch_SetVRAMBanks
static void func_ov099_021be030(void) {
    GFL_BGSysSetVRAMBanks(&data_ov099_021c3aa0);
}

// pokebw2's WifiP2PMatch_BGInit
static void func_ov099_021be040(HeapID heapId) {
    int i;

    GFL_BGSysCreate(heapId);
    {
        BGSysLCDConfig config = { 1, 0, 0, FALSE };

        GFL_BGSysSetLCDConfig(&config);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x1000,
            0,
            BGRES_512x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x10000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            0,
        };

        GFL_BGSysCreateBG(0, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(0);
        GFL_BGSysLoadScr(0);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            0,
            0,
        };

        GFL_BGSysCreateBG(1, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(1);
        GFL_BGSysLoadScr(1);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xd800,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_23,
            2,
            0,
            0,
        };

        GFL_BGSysCreateBG(2, &setup, BGMODE_TEXT);
        GFL_BGSysFillChar(2, 0, 1, 0);
        GFL_BGSysClearBG(2);
        GFL_BGSysLoadScr(2);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_23,
            1,
            0,
            0,
        };

        GFL_BGSysCreateBG(3, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(3);
        GFL_BGSysLoadScr(3);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe000,
            GX_BG_CHARBASE_0x00000,
            0x8000,
            GX_BG_EXTPLTT_01,
            3,
            0,
            0,
        };

        GFL_BGSysCreateBG(4, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(4);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xe800,
            GX_BG_CHARBASE_0x08000,
            0x8000,
            GX_BG_EXTPLTT_01,
            0,
            0,
            0,
        };

        GFL_BGSysCreateBG(5, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(5);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf000,
            GX_BG_CHARBASE_0x10000,
            0x8000,
            GX_BG_EXTPLTT_01,
            2,
            0,
            0,
        };

        GFL_BGSysCreateBG(6, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(6);
    }
    {
        BGSetup setup = {
            0,
            0,
            0x800,
            0,
            BGRES_256x256,
            GX_BG_COLORMODE_16,
            GX_BG_SCRBASE_0xf800,
            GX_BG_CHARBASE_0x18000,
            0x8000,
            GX_BG_EXTPLTT_01,
            1,
            0,
            0,
        };

        GFL_BGSysCreateBG(7, &setup, BGMODE_TEXT);
        GFL_BGSysClearBG(7);
    }
    for (i = 0; i < 7; i++) {
        GFL_BGSysClearChar(i);
    }
    GFL_BGSysSetBGPriority(0, 3);
    GFL_BGSysSetBGPriority(1, 1);
    GFL_BGSysSetBGPriority(3, 1);
    GFL_BGSysSetBGPriority(2, 0);
    G2_BlendNone();
    G2S_BlendNone();
}

// Stores a person of the room in the first free slot
// pokebw2's WifiP2PMatch_AddMoveObj
static void func_ov099_021be21c(WIFIP2PMATCH_WORK *wk, MCR_MOVEOBJ *obj) {
    int i;

    for (i = 0; i < WIFIP2PMATCH_OBJ_MAX; i++) {
        if (wk->moveObj[i] == NULL) {
            wk->moveObj[i] = obj;
            return;
        }
    }
}

// pokebw2's WifiP2PMatch_RemoveMoveObj
static void func_ov099_021be244(WIFIP2PMATCH_WORK *wk, MCR_MOVEOBJ *obj) {
    int i;

    for (i = 0; i < WIFIP2PMATCH_OBJ_MAX; i++) {
        if (wk->moveObj[i] == obj) {
            wk->moveObj[i] = NULL;
            return;
        }
    }
}

// Frees the room and forgets who was in it
// pokebw2's WifiP2PMatch_RoomExit
static void func_ov099_021be26c(WIFIP2PMATCH_WORK *wk) {
    int i;

    func_ov099_021b9670(&wk->room);
    for (i = 0; i < WIFIP2PMATCH_OBJ_MAX; i++) {
        wk->moveObj[i] = NULL;
        func_ov099_021c203c(wk, i + 1);
    }
    MI_CpuFill8(wk->matchStatus, 0, sizeof(wk->matchStatus));
}

// The person of the room who is a friend, or NULL
// pokebw2's WifiP2PMatch_GetMoveObj
static MCR_MOVEOBJ *func_ov099_021be2a8(WIFIP2PMATCH_WORK *wk, u32 friendNo) {
    int i;

    for (i = 0; i < WIFIP2PMATCH_OBJ_MAX; i++) {
        if (wk->moveObj[i] != NULL) {
            if (friendNo == func_ov099_021b9a50(wk->moveObj[i])) {
                return wk->moveObj[i];
            }
        }
    }
    return NULL;
}

// Loads the status icons: their palettes and characters for both screens, and the screen their tiles are read from
// pokebw2's WifiP2PMatch_IconLoad
static void func_ov099_021be2dc(WIFIP2PMATCH_ICON *icon, ArcTool *arc, u32 heapId) {
    GFL_G2DIOLoadArcNCLRDefault(arc, 4, 0, 0x100, 0x80, heapId);
    GFL_G2DIOLoadArcNCLRDefault(arc, 4, 4, 0x100, 0x80, heapId);
    icon->bg1Chars = GFL_BGSysLoadArcNCGRDynamic(arc, 13, 1, 0, FALSE, heapId);
    icon->bg6Chars = GFL_BGSysLoadArcNCGRDynamic(arc, 13, 6, 0, FALSE, heapId);
    if (icon->charBuf == NULL) {
        icon->charBuf = GFL_G2DIOReadOBJNCGRArc(arc, 13, FALSE, &icon->chr, heapId);
    }
    if (icon->scrnBuf == NULL) {
        icon->scrnBuf = GFL_G2DIOReadNSCRArc(arc, 21, FALSE, &icon->scrn, heapId);
    }
}

// pokebw2's WifiP2PMatch_IconFree
static void func_ov099_021be384(WIFIP2PMATCH_ICON *icon) {
    if (icon->scrnBuf != NULL) {
        GFL_HeapFree(icon->scrnBuf);
        icon->scrnBuf = NULL;
    }
    if (icon->charBuf != NULL) {
        GFL_HeapFree(icon->charBuf);
        icon->charBuf = NULL;
    }
    GFL_BGSysFreeCharMemory(1, CHAR_POS(icon->bg1Chars), CHAR_SIZE(icon->bg1Chars));
    GFL_BGSysFreeCharMemory(6, CHAR_POS(icon->bg6Chars), CHAR_SIZE(icon->bg6Chars));
}

// Writes an icon's four tiles into a BG's screen. The icons are 16 to a row in their screen
// pokebw2's WifiP2PMatch_IconWrite
static void func_ov099_021be3cc(WIFIP2PMATCH_ICON *icon, u32 bg, u8 x, u8 y, u32 iconNo, u32 unused) {
    int i;
    int j;
    u16 *tiles = (u16 *)icon->scrn->rawData;
    u32 palette = func_ov099_021be4ec(bg);
    u32 srcY = (iconNo / 16) * 2;
    u32 srcX = (iconNo % 16) * 2;
    u32 width = icon->scrn->screenWidth / 8;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            func_02042fb0(bg, j + x, i + y, (palette << 12) + tiles[(srcY + i) * width + srcX + j]);
        }
    }
    GFL_BGSysQueueScrLoad(bg);
}

// The icon of a game mode, which differs while the friend is recruiting for it
// pokebw2's WifiP2PMatch_GetIconNo
static u32 func_ov099_021be448(u32 gameMode, u32 status) {
    u32 iconNo = 15;

    switch (gameMode) {
    case 4:
        if (status == WIFIP2PMATCH_STATUS_RECRUIT) {
            iconNo = 4;
        } else {
            iconNo = 3;
        }
        break;
    case 5:
        if (status == WIFIP2PMATCH_STATUS_RECRUIT) {
            iconNo = 6;
        } else {
            iconNo = 5;
        }
        break;
    case 6:
        if (status == WIFIP2PMATCH_STATUS_RECRUIT) {
            iconNo = 10;
        } else {
            iconNo = 9;
        }
        break;
    case 2:
        iconNo = 2;
        break;
    case 0:
        iconNo = 15;
        break;
    case 3:
        iconNo = 1;
        break;
    default:
        if (func_ov099_021bdd60(gameMode)) {
            if (status == WIFIP2PMATCH_STATUS_RECRUIT) {
                iconNo = 8;
            } else {
                iconNo = 7;
            }
        } else if (gameMode >= 15) {
            iconNo = 14;
        }
        break;
    }
    return iconNo;
}

static void func_ov099_021be4bc(WIFIP2PMATCH_ICON *icon, u32 bg, u8 x, u8 y, u32 status, u32 gameMode,
                                         u32 add) {
    u32 iconNo = func_ov099_021be448(gameMode, status);

    func_ov099_021be3cc(icon, bg, x, y, iconNo + add, 0);
}

// The palette of the icons
// pokebw2's WifiP2PMatch_GetIconPalette
static u32 func_ov099_021be4ec(u32 bg) {
    return 8;
}

// Leaves a connection: puts the voice chat settings back in the status. Returns the state to go on with
// pokebw2's WifiP2PMatch_LeaveConnection
static int func_ov099_021be4f0(WIFIP2PMATCH_WORK *wk, BOOL wipe) {
    if (wipe) {
        GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, WIFIP2PMATCH_HEAP);
    }
    func_ov099_021c27e8(wk->myStatus, wk->parent->unk46);
    func_ov099_021c2864(wk->myStatus, wk->parent->unk46);
    func_ov099_021c0c7c();
    return WIFIP2PMATCH_SEQ_LOGIN_WAIT;
}

// pokebw2's WifiP2PMatch_BGExit
static void func_ov099_021be538(WIFIP2PMATCH_WORK *wk) {
}

// pokebw2's WifiP2PMatch_BGGraphicLoad
static void func_ov099_021be53c(WIFIP2PMATCH_WORK *wk, ArcTool *arc) {
    GFL_BGSysSetBGEnabledA(0xf, TRUE);
    GFL_BGSysSetBGEnabledB(0xd, TRUE);
    func_ov099_021be2dc(&wk->icon, arc, WIFIP2PMATCH_HEAP);
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 15 * 0x20, 0x20, WIFIP2PMATCH_HEAP);
    wk->listFrameChars = LoadCursorImageEndOfHeap(2, 14, 0, WIFIP2PMATCH_HEAP);
    wk->talkFrameChars = LoadCursorImageEndOfHeap(2, 14, 0, WIFIP2PMATCH_HEAP);
    wk->subFrameChars = LoadCursorImageEndOfHeap(1, 14, 0, WIFIP2PMATCH_HEAP);
}

// pokebw2's WifiP2PMatch_CharInit
static void func_ov099_021be5b4(void) {
}

static const ClActSurfaceSetup data_ov099_021c375c[2] = {
    { 0, 0, 256, 192, CLACT_VRAM_MAIN, FALSE },
    { 0, 256, 256, 192, CLACT_VRAM_SUB, FALSE },
};

static const ClActSysSetup data_ov099_021c3740 = { 0, 0, 0, 512, 4, 124, 4, 124, 0, 100, 100, 100, 100, 16, 16 };

// pokebw2's WifiP2PMatch_ClActInit
static void func_ov099_021be5b8(WIFIP2PMATCH_WORK *wk, ArcTool *arc) {
    ClActSys_Create(&data_ov099_021c3740, &data_ov099_021c3aa0, WIFIP2PMATCH_HEAP);
    wk->clActUnit = func_0204b100(100, 0, WIFIP2PMATCH_HEAP);
    wk->clActRenderer =
        func_0204b034(data_ov099_021c375c, NELEMS(data_ov099_021c375c), WIFIP2PMATCH_HEAP);
    func_0204b258(wk->clActUnit, wk->clActRenderer);
}

// State 0: the proc's first. Starts the screen, or goes on with the connection after a game
// pokebw2's WifiP2PMatch_StateInit
static int func_ov099_021be600(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->parent->mode != 0) {
        func_ov099_021c24d4(wk, 4);
        func_02009b80(wk->wifiList, func_ov009_02156a3c());
        if ((wk->result == 3 || wk->result == 4) && !func_ov009_02156d84()) {
            func_ov009_021563d0();
            func_ov099_021beebc(wk);
            wk->friendNo = wk->parent->friendIndex;
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, 3);
            func_02039c68(func_020399a8(), 0x18, 10);
            wk->seq = WIFIP2PMATCH_SEQ_RETURN_TIMING_WAIT;
        } else {
            func_02039c68(func_020399a8(), 0x12, 10);
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_END_TIMING;
        }
    } else {
        wk->seq = WIFIP2PMATCH_SEQ_LOGIN_WAIT;
        func_ov099_021c24d4(wk, 1);
    }
    if (func_02005c4c() != SEQ_BGM_POKECEN) {
        GFL_SndBGMPlay(SEQ_BGM_POKECEN, 0xffff);
    }
    return seq;
}

// State 5
// pokebw2's WifiP2PMatch_StateBGPriority
static int func_ov099_021be6b0(WIFIP2PMATCH_WORK *wk, int seq) {
    GFL_BGSysSetBGPriority(0, 3);
    GFL_BGSysSetBGPriority(1, 1);
    GFL_BGSysSetBGPriority(3, 1);
    GFL_BGSysSetBGPriority(2, 0);
    wk->seq = WIFIP2PMATCH_SEQ_CONNECT_WAIT;
    return seq;
}

// Gives the status to the network
// pokebw2's WifiP2PMatch_SendMyStatus
static void func_ov099_021be6e4(WIFIP2PMATCH_WORK *wk) {
    func_ov009_0215683c((const char *)wk->myStatus, sizeof(WIFI_STATUS));
}

// Fills the player's status and gives it to the network
// pokebw2's WifiP2PMatch_InitMyStatus
static void func_ov099_021be6f0(WIFIP2PMATCH_WORK *wk, u32 status, u32 gameMode) {
    PlayerInfo *info = GetGameDataPlayerInfo(wk->gameData);
    u8 gender;

    wk->myStatus->version = GAME_VERSION;
    wk->myStatus->language = GAME_LANGUAGE;
    gender = getTrainerGender(info);
    wk->myStatus->gender = gender;
    wk->myStatus->trainerView = func_02008554(info);
    func_ov099_021c1908(wk, status, gameMode);
    wk->myStatus->country = UnityTowerVisitor_GetCountry(info);
    wk->myStatus->province = UnityTowerVisitor_GetProvince(info);
    func_ov099_021c27e8(wk->myStatus, wk->parent->unk46);
    func_ov099_021c2864(wk->myStatus, wk->parent->unk46);
    // WIFI_STATUS.unk0 is the trainer ID after the GameSpy profile ID
    wk->myStatus->trainerId = getIDAsUInt(info);
    wk->myStatus->profileId = func_0200853c(info);
    func_ov099_021be6e4(wk);
}

// Lists the friends who are in the room. Returns how many
// pokebw2's WifiP2PMatch_UpdateRoomFriendList
static int func_ov099_021be788(WIFIP2PMATCH_WORK *wk) {
    int i;
    int count = 0;

    for (i = 0; i < WIFIP2PMATCH_MEMBER_MAX; i++) {
        wk->index2No[i] = 0;
        if (wk->friendNum > i) {
            if (func_ov099_021c23c0(i)) {
                u32 status = func_ov099_021bdd48(wk, func_ov099_021c23b8(i));

                if (status != WIFIP2PMATCH_STATUS_NONE && status < 6) {
                    wk->index2No[count] = i + 1;
                    count++;
                } else if (func_ov099_021c2050(wk, i + 1)) {
                    func_ov099_021c203c(wk, i + 1);
                }
            }
        }
    }
    return count;
}

// Looks for friends whose status, game mode or voice chat setting changed, makes their characters show it and
// records what the friend list keeps of them. Returns how many changed
// pokebw2's WifiP2PMatch_CheckFriendStatusChange
static int func_ov099_021be7ec(WIFIP2PMATCH_WORK *wk) {
    int i;
    int count = 0;
    MCR_MOVEOBJ *obj;

    for (i = 0; i < WIFIP2PMATCH_MEMBER_MAX; i++) {
        WIFI_STATUS *status = func_ov099_021c23b8(i);
        u8 wifiStatus = func_ov099_021bdd48(wk, status);
        u8 gameMode = func_ov099_021bdd54(wk, status);

        if (func_ov099_021c23c0(i) == 0) {
            wifiStatus = WIFIP2PMATCH_STATUS_NONE;
        }
        if (wifiStatus != wk->matchStatus[i] || gameMode != wk->matchGameMode[i] ||
            wk->matchVChat[i] != func_ov099_021c27e0(status)) {
            obj = func_ov099_021be2a8(wk, i + 1);
            if (obj != NULL) {
                BOOL wasRecruiting = func_ov099_021bdda8(wk->matchStatus[i]);
                BOOL isRecruiting = func_ov099_021bdda8(wifiStatus);

                if (wasRecruiting == TRUE && isRecruiting == FALSE) {
                    func_ov099_021b9b00(&wk->room, obj, WIFI_MCR_NPC_MOVE_NORMAL);
                } else if (wasRecruiting == FALSE && isRecruiting == TRUE) {
                    func_ov099_021b9b00(&wk->room, obj, WIFI_MCR_NPC_MOVE_JUMP);
                }
                func_02009918(wk->wifiList, i, WIFI_LIST_FRIEND_TRAINER_VIEW, status->trainerView);
                func_02009918(wk->wifiList, i, WIFI_LIST_FRIEND_GENDER, status->gender);
                func_02009918(wk->wifiList, i, WIFI_LIST_FRIEND_PROFILE_ID, status->profileId);
                func_02009918(wk->wifiList, i, WIFI_LIST_FRIEND_ID, status->trainerId);
            }
            wk->matchStatus[i] = wifiStatus;
            wk->matchGameMode[i] = gameMode;
            wk->matchVChat[i] = func_ov099_021c27e0(status);
            count++;
        }
    }
    return count;
}

// pokebw2's WifiP2PMatch_OnNetError
static void func_ov099_021be900(WIFIP2PMATCH_WORK *wk) {
}

// pokebw2's WifiP2PMatch_StateReconnectKeyWait
static int func_ov099_021be904(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->timer > 0) {
        wk->timer--;
        return seq;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
        func_ov099_021baab8(wk, Bank0416_Text_055, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_RECONNECT_YES_NO_START;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateReconnectYesNoStart
static int func_ov099_021be94c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_RECONNECT_YES_NO;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateReconnectYesNo
static int func_ov099_021be96c(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret = func_ov099_021ba858(wk);

    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        func_0203ceac(NULL);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_START;
    } else {
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        func_0203ceac(NULL);
        wk->endSeq = 0;
        wk->seq = WIFIP2PMATCH_SEQ_END;
    }
    func_ov099_021ba8ec(wk);
    return seq;
}

// Takes the room and the friend page down, if they are up, and connects again
// pokebw2's WifiP2PMatch_StateConnectStart
static int func_ov099_021be9c8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_0203d210()) {
        ArcTool *arc;

        if (func_ov099_021b96cc(&wk->room) == TRUE) {
            GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_OUT, WIPE_TYPE_FADE_OUT, WIPE_COLOR_BLACK, 6, 1,
                        WIFIP2PMATCH_HEAP);
            func_ov099_021be26c(wk);
            func_0204a698();
        }
        if (func_ov099_021c1a44(wk) == TRUE) {
            func_ov099_021c1ac0(wk);
        }
        wk->titleWin = func_ov099_021ba8bc(wk->titleWin);
        GFL_BGSysClearBG(3);
        arc = GFL_ArcSysCreateFileHandle(ARCID_WIFIP2PMATCH, WIFIP2PMATCH_HEAP);
        func_ov099_021be53c(wk, arc);
        GFL_ArcToolFree(arc);
        wk->unk1328 = 1;
        if (wk->subMenuOptions != NULL) {
            BmpWin_Free(wk->subWin);
            BmpMenuList_Free(wk->subList, NULL, NULL);
            wk->subList = NULL;
            ListMenuCore_FreeOptionList(wk->subMenuOptions);
            wk->subMenuOptions = NULL;
        }
        MI_CpuFill8(wk->index2No, 0, sizeof(wk->index2No));
        MI_CpuFill8(wk->index2NoBackup, 0, sizeof(wk->index2NoBackup));
        MI_CpuFill8(wk->matchStatus, 0, sizeof(wk->matchStatus));
        MI_CpuFill8(wk->matchGameMode, 0, sizeof(wk->matchGameMode));
        MI_CpuFill8(wk->matchVChat, 0, sizeof(wk->matchVChat));
        func_ov099_021bb050(wk);
        func_ov099_021bdd0c(wk, FALSE);
        func_0203c8a8();
        func_ov099_021baab8(wk, WifiError_Text_023, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_BG_PRIORITY;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateConnectWait
static int func_ov099_021beaec(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (func_ov009_02156c84()) {
        wk->seq = WIFIP2PMATCH_SEQ_USER_DATA_SAVE_START;
        func_ov099_021baab8(wk, WifiError_Text_030, TRUE);
    }
    if (func_0203c600()) {
        if (wk->unk1E8 != 0) {
            wk->seq = WIFIP2PMATCH_SEQ_USER_INFO_NOTICE;
        } else {
            func_ov099_021be6f0(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
            func_ov099_021be788(wk);
            wk->seq = func_ov099_021be4f0(wk, FALSE);
        }
    } else if (func_0203cb00() || func_0203c3ec() == 3) {
        func_ov099_021be900(wk);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateUserDataSaveStart
static int func_ov099_021beb68(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (func_ov009_02156c84()) {
        func_02012dac(wk->gameData);
        wk->seq = WIFIP2PMATCH_SEQ_USER_DATA_SAVE;
        return seq;
    }
    if (func_0203c600()) {
        if (wk->unk1E8 != 0) {
            wk->seq = WIFIP2PMATCH_SEQ_USER_INFO_NOTICE;
        } else {
            func_ov099_021be6f0(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
            func_ov099_021be788(wk);
            wk->seq = func_ov099_021be4f0(wk, FALSE);
        }
    } else if (func_0203cb00() || func_0203c3ec() == 3) {
        func_ov099_021be900(wk);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateUserDataSave
static int func_ov099_021bebe8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_ov009_02156c84()) {
        int result = func_02012dd0(wk->gameData);

        if (result != 0 && result != 1) {
            func_ov009_02156c98();
        } else {
            return seq;
        }
    }
    if (func_0203c600()) {
        if (wk->unk1E8 != 0) {
            wk->seq = WIFIP2PMATCH_SEQ_USER_INFO_NOTICE;
        } else {
            func_ov099_021be6f0(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
            func_ov099_021be788(wk);
            wk->seq = func_ov099_021be4f0(wk, FALSE);
        }
    } else if (func_0203cb00() || func_0203c3ec() == 3) {
        func_ov099_021be900(wk);
    }
    return seq;
}

// Waits, then leaves the connection and goes back to the room
// pokebw2's WifiP2PMatch_StateDisconnectWait
static int func_ov099_021bec60(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->timer > 0) {
        wk->timer--;
        return seq;
    }
    if (wk->regulationPrintMsg != NULL) {
        func_0201a570(wk->regulationPrintMsg);
        wk->regulationPrintMsg = NULL;
    }
    if (func_0203cb00() || func_0203c3ec() == 3) {
        func_ov099_021be900(wk);
    } else if (func_0203c628()) {
        if (func_ov099_021b96cc(&wk->room) == TRUE) {
            func_ov099_021be26c(wk);
        }
        wk->seq = func_ov099_021be4f0(wk, FALSE);
    } else if (func_0203c3ec() < 4 && !func_0203c5ec() && func_0203d1c0(0xff)) {
        // Still disconnecting
    } else {
        func_0203c554(TRUE);
        if (func_ov099_021b96cc(&wk->room) == TRUE) {
            func_ov099_021be26c(wk);
        }
        wk->seq = func_ov099_021be4f0(wk, FALSE);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateUserInfoNotice
static int func_ov099_021bed08(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba8ec(wk);
    if (func_0202eb0c()) {
        func_ov099_021bab04(wk, WifiError_Text_044);
    } else {
        func_ov099_021bab04(wk, WifiError_Text_019);
    }
    wk->seq = WIFIP2PMATCH_SEQ_USER_INFO_NOTICE_KEY_WAIT;
    wk->unk1E8 = 0;
    return seq;
}

// pokebw2's WifiP2PMatch_StateUserInfoNoticeKeyWait
static int func_ov099_021bed3c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        if (func_02009a34(wk->wifiList) == 0) {
            func_ov099_021baab8(wk, WifiError_Text_026, TRUE);
            wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT;
            wk->timer = 1;
        } else {
            func_ov099_021be6f0(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
            func_ov099_021be788(wk);
            wk->seq = func_ov099_021be4f0(wk, FALSE);
            wk->sysMsgWin = func_ov099_021ba8bc(wk->sysMsgWin);
        }
    }
    return seq;
}

// Makes the windows of the room's upper screen: the player's status and the message frame
// pokebw2's WifiP2PMatch_CreateRoomWindows
static void func_ov099_021beda0(WIFIP2PMATCH_WORK *wk) {
    if (wk->myInfoWin != NULL) {
        BmpWin_Free(wk->myInfoWin);
    }
    GFL_BGSysLoadNCLRDefault(ARCID_FONT, 5, 0, 0x1e0, 0x20, WIFIP2PMATCH_HEAP);
    wk->titleWin = func_ov099_021ba8bc(wk->titleWin);
    wk->myInfoWin = BmpWin_CreateDynamic(3, 1, 1, 28, 2, 15, 1);
    wk->titleWin = BmpWin_CreateDynamic(2, 1, 1, 28, 2, 15, 1);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->myInfoWin), 0xff);
    BmpWin_FlushChar(wk->myInfoWin);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->titleWin), 0xff);
    BmpWin_FlushChar(wk->titleWin);
    BmpWin_DrawFrame(wk->titleWin, TRUE, wk->talkFrameChars, 14);
}

// Whether a friend who asks to connect is in the game mode the player is in
// pokebw2's WifiP2PMatch_ConnectRequestFunc
static BOOL func_ov099_021bee64(int friendIndex, void *work) {
    WIFIP2PMATCH_WORK *wk = work;
    WIFI_STATUS *myStatus = wk->myStatus;
    WIFI_STATUS *friendStatus = func_ov099_021c23b8(friendIndex);

    func_ov099_021c27d0(myStatus);
    func_ov099_021c27d0(friendStatus);
    if (func_ov099_021c27d8(myStatus) == func_ov099_021c27d8(friendStatus)) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiP2PMatch_StateLoginWait
static int func_ov099_021bee94(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_0203cb00() && !func_0203c628()) {
        return seq;
    }
    wk->seq = WIFIP2PMATCH_SEQ_ROOM_START;
    return seq;
}

// Starts the room, with the player in it, and the friend page
// pokebw2's WifiP2PMatch_StartRoom
static void func_ov099_021beebc(WIFIP2PMATCH_WORK *wk) {
    ArcTool *arc;

    func_ov009_02155664(func_ov099_021bee64);
    arc = GFL_ArcSysCreateFileHandle(ARCID_WIFIP2PMATCH, WIFIP2PMATCH_HEAP);
    func_ov099_021c19a8(wk);
    func_ov099_021c0290(wk);
    MI_CpuFill8(wk->index2NoBackup, 0, sizeof(wk->index2NoBackup));
    if (wk->subMenuOptions != NULL) {
        BmpWin_Free(wk->subWin);
        BmpMenuList_Free(wk->subList, NULL, NULL);
        wk->subList = NULL;
        ListMenuCore_FreeOptionList(wk->subMenuOptions);
        wk->subMenuOptions = NULL;
    }
    if (!func_ov099_021b96cc(&wk->room)) {
        u32 view;

        if (getTrainerGender(GetGameDataPlayerInfo(wk->gameData)) == 0) {
            view = 0x10;
        } else {
            view = 0x11;
        }
        wk->friendNum = func_02009a54(wk->wifiList);
        func_ov099_021b95a0(&wk->room, WIFIP2PMATCH_HEAP, arc, view, wk->friendNum, ARCID_WIFIP2PMATCH, wk->clActUnit,
                      wk->clActRenderer);
        func_ov099_021be21c(wk, func_ov099_021b9900(&wk->room, view));
    }
    if (!func_ov099_021c1a44(wk)) {
        func_ov099_021c1a58(wk, arc, WIFIP2PMATCH_HEAP);
    }
    func_ov099_021c282c(wk->myStatus);
    wk->unk1351 = 0;
    func_ov099_021c205c(wk, WIFIP2PMATCH_HEAP);
    func_ov099_021ba8ec(wk);
    func_ov099_021be788(wk);
    func_ov099_021beda0(wk);
    func_ov099_021bdd0c(wk, FALSE);
    wk->preConnect = -1;
    GFL_ArcToolFree(arc);
    GFL_WipeSet(WIPE_MODE_BOTH, WIPE_TYPE_FADE_IN, WIPE_TYPE_FADE_IN, WIPE_COLOR_BLACK, 6, 1, WIFIP2PMATCH_HEAP);
}

// pokebw2's WifiP2PMatch_StateRoomStart
static int func_ov099_021bf000(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021beebc(wk);
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    func_ov099_021bde30(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    func_ov099_021c2408(wk);
    func_0203d7b0(0);
    func_0203d7bc(FALSE);
    return seq;
}

// pokebw2's WifiP2PMatch_IsConnected
static BOOL func_ov099_021bf03c(WIFIP2PMATCH_WORK *wk) {
    if (func_0203c3ec() == 1) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiP2PMatch_HasNewConnection
static BOOL func_ov099_021bf050(WIFIP2PMATCH_WORK *wk) {
    if (func_ov009_02156b8c() != -1) {
        return TRUE;
    }
    return FALSE;
}

// Whether the connection failed
// pokebw2's WifiP2PMatch_IsConnectFailed
static BOOL func_ov099_021bf068(WIFIP2PMATCH_WORK *wk) {
    switch (func_0203c3ec()) {
    case 3:
    case 4:
    case 5:
        return TRUE;
    }
    return FALSE;
}

// Takes the friends who left out of the room and puts those who came into it. Returns how many friends are there
// pokebw2's WifiP2PMatch_UpdateFriendsInRoom
static int func_ov099_021bf084(WIFIP2PMATCH_WORK *wk) {
    int i, j;
    MCR_MOVEOBJ *obj;
    int num = func_ov099_021be788(wk);
    BOOL in = FALSE;
    BOOL out = FALSE;

    if (GFL_STD_MemCmp(wk->index2No, wk->index2NoBackup, sizeof(wk->index2No)) != 0) {
        for (i = 0; i < WIFIP2PMATCH_MEMBER_MAX; i++) {
            BOOL found = FALSE;

            for (j = 0; j < WIFIP2PMATCH_MEMBER_MAX; j++) {
                if (wk->index2No[j] == wk->index2NoBackup[i]) {
                    found = TRUE;
                    break;
                }
            }
            if (!found && wk->index2NoBackup[i] != 0) {
                obj = func_ov099_021be2a8(wk, wk->index2NoBackup[i]);
                if (obj != NULL) {
                    func_ov099_021b99f4(&wk->room, obj);
                    func_ov099_021be244(wk, obj);
                    func_ov099_021c203c(wk, wk->index2NoBackup[i]);
                    func_ov099_021c1a14(wk, wk->index2NoBackup[i], WIFIP2PMATCH_HEAP);
                    out = TRUE;
                } else {
                    func_ov099_021c203c(wk, wk->index2NoBackup[i]);
                }
            }

            found = FALSE;
            for (j = 0; j < WIFIP2PMATCH_MEMBER_MAX; j++) {
                if (wk->index2No[i] == wk->index2NoBackup[j]) {
                    found = TRUE;
                    break;
                }
            }
            if (!found && wk->index2No[i] != 0) {
                WIFI_STATUS *status = func_ov099_021c23b8(wk->index2No[i] - 1);

                obj = func_ov099_021b9968(&wk->room, status->trainerView, wk->index2No[i]);
                if (obj != NULL) {
                    func_ov099_021c2028(wk, wk->index2No[i], 2);
                    func_ov099_021be21c(wk, obj);
                    in = TRUE;
                } else {
                    func_ov099_021c2028(wk, wk->index2No[i], 1);
                }
            }
        }
        MI_CpuCopy8(wk->index2No, wk->index2NoBackup, sizeof(wk->index2No));
        if (in || out) {
            func_ov099_021c207c(wk);
        }
        if (in == TRUE) {
            GFL_SndSEPlay(SEQ_SE_FLD_05);
        } else if (out == TRUE) {
            GFL_SndSEPlay(SEQ_SE_FLD_05);
        }
    }
    return num;
}

// Puts the friends into the room who could not come in before, because the player stood on their seat
// pokebw2's WifiP2PMatch_AddWaitingFriends
static void func_ov099_021bf1cc(WIFIP2PMATCH_WORK *wk) {
    int i;
    BOOL in = FALSE;

    for (i = 0; i < WIFIP2PMATCH_MEMBER_MAX; i++) {
        if (wk->index2No[i] != 0 && func_ov099_021be2a8(wk, wk->index2No[i]) == NULL) {
            WIFI_STATUS *status = func_ov099_021c23b8(wk->index2No[i] - 1);
            MCR_MOVEOBJ *obj = func_ov099_021b9968(&wk->room, status->trainerView, wk->index2No[i]);

            if (obj != NULL) {
                func_ov099_021c2028(wk, wk->index2No[i], 2);
                func_ov099_021be21c(wk, obj);
                wk->matchStatus[i] = 0;
                wk->matchGameMode[i] = 0;
                wk->matchVChat[i] = 0;
                in = TRUE;
            }
        }
    }
    if (in == TRUE) {
        GFL_SndSEPlay(SEQ_SE_FLD_05);
    }
}

// pokebw2's WifiP2PMatch_UpdateFriendView
static void func_ov099_021bf24c(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    if (func_ov099_021be7ec(wk) > 0) {
        if (wk->view.detailMode != 0) {
            func_ov099_021bb2b0(wk, heapId);
        } else {
            wk->view.redrawAll = TRUE;
            func_ov099_021c2090(wk);
        }
    }
}

// The friend list's bookkeeping of a frame
// pokebw2's WifiP2PMatch_UpdateFriendList
static void func_ov099_021bf280(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    func_ov099_021bf084(wk);
    func_ov099_021bf24c(wk, heapId);
    func_ov099_021bf1cc(wk);
}

// pokebw2's WifiP2PMatch_ShowAllShadows
static void func_ov099_021bf29c(WIFIP2PMATCH_WORK *wk) {
    int i;

    for (i = 0; i < WIFIP2PMATCH_OBJ_MAX; i++) {
        if (wk->moveObj[i] != NULL) {
            func_ov099_021b98e4(&wk->room, wk->moveObj[i]);
        }
    }
}

static int func_ov099_021bf2c4(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret = 0;
    u32 status;
    int i;
    int friendIndex;

    wk->unk1356 = 0;
    if (!GFL_WipeIsFinished()) {
        return seq;
    }
    if (func_0203cb00()) {
        func_ov099_021be900(wk);
        return seq;
    }
    if (wk->preConnect != -1) {
        wk->unk1C8++;
        if (wk->unk1C8 > 60 * 60) {
            func_0203c9ac(0, 0, 0, 0x3f1);
            return seq;
        }
    }
    if (func_0203c3ec() == 4) {
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = 0x15;
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        func_0203c554(TRUE);
        return seq;
    }
    func_ov099_021bf280(wk, WIFIP2PMATCH_HEAP);
    func_ov099_021c1bec(wk);
    func_ov099_021b9b54(&wk->room);
    if (!func_ov099_021b98fc(&wk->room)) {
        if (func_ov009_02156b8c() == -1 && (GCTX_HIDGetPressedKeys() & 0x400)) {
            if (func_ov099_021c1968(wk)) {
                func_ov099_021baadc(wk, 0x39, FALSE);
            } else {
                func_ov099_021baadc(wk, 0x3a, FALSE);
            }
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov099_021baedc(wk);
            wk->seq = 0xc;
            func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
            return seq;
        }
        {
            u32 idx = func_ov099_021c1b04(wk, WIFIP2PMATCH_HEAP);
            MCR_MOVEOBJ *obj = func_ov099_021be2a8(wk, idx);
            if (idx != 0) {
                if (obj != NULL) {
                    func_ov099_021b989c(&wk->room, obj);
                } else {
                    func_ov099_021bf29c(wk);
                    func_ov099_021b98f0(&wk->room);
                }
            } else {
                func_ov099_021bf29c(wk);
                func_ov099_021b98f0(&wk->room);
            }
        }
    }
    for (i = 0; i < 0x20; i++) {
        int rand = GFL_RandomLCAlt(0x20);
        u32 callCounter;

        if (!func_ov099_021c27f8(wk->myStatus, func_ov099_021c23b8(rand))) {
            continue;
        }
        callCounter = func_ov099_021c2874(func_ov099_021c23b8(rand));
        if (callCounter == wk->parent->unk24[rand]) {
            continue;
        }
        func_ov099_021bdd48(wk, func_ov099_021c23b8(rand));
        func_ov099_021bdd54(wk, func_ov099_021c23b8(rand));
        wk->friendNo = rand + 1;
        GFL_SEPlayKeepVol(SEQ_SE_SYS_101, 0);
        wk->seq = 0x3f;
        return seq;
    }
    if (func_ov099_021bf03c(wk) && wk->preConnect != -1) {
        GFL_SEPlayKeepVol(SEQ_SE_SYS_101, 0);
        func_0203d7b0(1);
        func_0203d7bc(TRUE);
        wk->seq = 0x28;
        return seq;
    }
    if (wk->preConnect == -1 && func_ov009_02156b8c() != -1) {
        wk->preConnect = func_ov009_02156b8c();
        func_ov099_021ba874(wk, wk->preConnect);
        if (wk->unk1351 == 0) {
            func_ov099_021baab8(wk, Bank0416_Text_046, FALSE);
        }
        wk->unk1C8 = 0;
    } else if (wk->preConnect != -1 && func_ov009_02156b8c() == -1) {
        func_ov009_02156bc4(0);
        func_ov099_021c2864(wk->myStatus, func_ov099_021c27e0(wk->myStatus));
        func_ov099_021ba874(wk, wk->preConnect);
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        func_0203c554(TRUE);
        wk->seq = 0x13;
        return seq;
    }
    if (func_ov099_021bf068(wk) && wk->preConnect != -1) {
        func_ov099_021ba874(wk, func_ov009_02156ba8());
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = 0x13;
        return seq;
    }
    if (func_ov009_02156b70() == 1 && func_ov009_02156b8c() == -1) {
        func_ov009_02156bc4(0);
        func_ov099_021c2864(wk->myStatus, func_ov099_021c27e0(wk->myStatus));
    }
    status = func_ov099_021bdd48(wk, wk->myStatus);
    if (wk->preConnect == -1) {
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) && func_ov099_021bdda8(status)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            wk->seq = 0x1f;
            func_ov099_021c19dc(wk, WIFIP2PMATCH_HEAP);
            return seq;
        }
        ret = func_ov099_021b96d0(&wk->room);
        func_ov099_021b9b64(&wk->room);
    }
    switch (ret) {
    case 0:
        return seq;
    case 1:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (func_ov099_021bdda8(status)) {
            wk->seq = 0x1f;
        } else {
            wk->endSeq = 0;
            wk->seq = 0x31;
            func_ov099_021baab8(wk, 0x19, TRUE);
        }
        func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
        return seq;
    case 3:
        if (status == WIFIP2PMATCH_STATUS_WAIT) {
            wk->result = 1;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov099_021b9b24(&wk->room);
            wk->seq = 0x1c;
            func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
            wk->unk1351 = 0;
            return seq;
        }
        break;
    case 2:
        if (status == WIFIP2PMATCH_STATUS_WAIT) {
            friendIndex = func_ov099_021b97f4(&wk->room);
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            MI_CpuCopy8(func_ov099_021c23b8(friendIndex - 1), &wk->targetStatus, sizeof(WIFI_STATUS));
            wk->seq = 0x22;
            func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
            return seq;
        }
        break;
    }
    return seq;
}


// The room while a message is up: as func_ov099_021bf2c4, without the friend page's touches and the voice chat calls
// pokebw2's WifiP2PMatch_StateRoomMessage
static int func_ov099_021bf6e0(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 status;
    u32 ret = WIFI_MCR_RET_NONE;

    if (wk->preConnect != -1) {
        wk->unk1C8++;
        if (wk->unk1C8 > 60 * 60) {
            func_0203c9ac(0, 0, 0, 0x3f1);
            return seq;
        }
    }
    func_ov099_021ba928(wk);
    wk->unk1356 = 0;
    func_ov099_021bf280(wk, WIFIP2PMATCH_HEAP);
    func_ov099_021c1bec(wk);
    func_ov099_021b9b54(&wk->room);
    if (func_ov099_021bf03c(wk) && wk->preConnect != -1) {
        GFL_SEPlayKeepVol(SEQ_SE_SYS_101, 0);
        func_0203d7b0(1);
        func_0203d7bc(TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_FRIEND_CONNECTED;
        return seq;
    }
    if (func_ov099_021bf068(wk) && wk->preConnect != -1) {
        func_ov099_021ba874(wk, func_ov009_02156ba8());
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        return seq;
    }
    if (wk->preConnect == -1 && func_ov009_02156b8c() != -1) {
        wk->preConnect = func_ov009_02156b8c();
        func_ov099_021ba874(wk, wk->preConnect);
        if (wk->unk1351 == 0) {
            func_ov099_021baab8(wk, Bank0416_Text_046, FALSE);
        }
        wk->unk1C8 = 0;
    } else if (func_ov009_02156ba8() != -1) {
        func_ov099_021ba874(wk, func_ov009_02156ba8());
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        return seq;
    }

    status = func_ov099_021bdd48(wk, wk->myStatus);
    if (wk->preConnect == -1) {
        if ((GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) && func_ov099_021bdda8(status)) {
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_STOP_ASK;
            func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
            return seq;
        }
        ret = func_ov099_021b96d0(&wk->room);
        func_ov099_021b9b64(&wk->room);
    }
    switch (ret) {
    case WIFI_MCR_RET_NONE:
        return seq;
    case WIFI_MCR_RET_CANCEL:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (func_ov099_021bdda8(status)) {
            wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_STOP_ASK;
        } else {
            wk->endSeq = 0;
            wk->seq = WIFIP2PMATCH_SEQ_EXIT_YES_NO_INIT;
            func_ov099_021baab8(wk, WifiError_Text_025, TRUE);
        }
        func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
        return seq;
    case WIFI_MCR_RET_MYSELECT:
        if (status == WIFIP2PMATCH_STATUS_WAIT) {
            wk->result = 1;
            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            func_ov099_021b9b24(&wk->room);
            wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_MENU_START;
            func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
            return seq;
        }
        break;
    case WIFI_MCR_RET_SELECT:
        if (status == WIFIP2PMATCH_STATUS_WAIT) {
            u32 friendNo = func_ov099_021b97f4(&wk->room);

            GFL_SndSEPlay(SEQ_SE_DECIDE1);
            MI_CpuCopy8(func_ov099_021c23b8(friendNo - 1), &wk->targetStatus, sizeof(WIFI_STATUS));
            wk->seq = WIFIP2PMATCH_SEQ_FRIEND_SELECT;
            func_ov099_021c1a30(wk, WIFIP2PMATCH_HEAP);
            return seq;
        }
        break;
    }
    return seq;
}

static int func_ov099_021bf950(WIFIP2PMATCH_WORK *wk, int seq) {
    long friendIndex = wk->friendNo - 1;

    if (GFL_SndPlayerIsActive(0)) {
        return seq;
    }
    wk->parent->unk24[friendIndex] = func_ov099_021c2874(func_ov099_021c23b8(friendIndex));
    if (friendIndex >= 0 && friendIndex < WIFIP2PMATCH_MEMBER_MAX && func_ov099_021c23c8(wk, friendIndex)) {
        wk->callTimer = 60 * 60;
        func_ov099_021bde30(wk, 4, 3);
        func_ov099_021c18c8(wk, 4, 3);
        func_ov099_021c19dc(wk, WIFIP2PMATCH_HEAP);
        func_ov099_021ba874(wk, friendIndex);
        wk->callTimer = 60 * 60;
        func_ov099_021baadc(wk, Bank0416_Text_094, FALSE);
        func_ov099_021bc090(wk);
        wk->seq = WIFIP2PMATCH_SEQ_JOIN_WAIT;
        wk->unk1350 = wk->friendNo;
        return seq;
    }
    wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    return seq;
}


// pokebw2's WifiP2PMatch_StateVChatToggleKeyWait
static int func_ov099_021bfa00(WIFIP2PMATCH_WORK *wk, int seq) {
    if (GCTX_HIDGetPressedKeys()) {
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        func_ov099_021c2408(wk);
        func_ov099_021ba8ec(wk);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatCallWait
static int func_ov099_021bfa28(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *status = func_ov099_021c23b8(wk->friendNo - 1);
    u32 wifiStatus;

    func_ov099_021c27e0(status);
    wifiStatus = func_ov099_021bdd48(wk, status);
    wk->callTimer--;
    if (wk->callTimer < 0) {
        wk->seq = WIFIP2PMATCH_SEQ_JOIN_TIMEOUT;
    } else if (wifiStatus != WIFIP2PMATCH_STATUS_WAIT && wifiStatus != WIFIP2PMATCH_STATUS_RECRUIT) {
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
    } else if (func_0203c3ec() == 1) {
        func_ov099_021ba8ec(wk);
        func_ov099_021c282c(wk->myStatus);
        wk->unk1351 = 0;
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, 4);
        wk->unk1355 = 0;
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_START;
        func_ov099_021c24a4(wk, 0);
        func_ov099_021baadc(wk, Bank0416_Text_123, FALSE);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatInvited
static int func_ov099_021bfad0(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba874(wk, func_ov009_02156a3c());
    func_ov099_021baab8(wk, Bank0416_Text_011, FALSE);
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, 4);
    wk->seq = WIFIP2PMATCH_SEQ_VCHAT_INVITED_WAIT;
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatInvitedWait
static int func_ov099_021bfb04(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_0203cb00()) {
        func_ov099_021be900(wk);
        return seq;
    }
    if (func_ov009_02156a3c() < 0) {
        func_ov099_021baab8(wk, Bank0416_Text_016, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        return seq;
    }
    if (func_ov099_021ba928(wk)) {
        if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
            func_ov099_021ba8ec(wk);
            func_ov099_021baadc(wk, Bank0416_Text_123, FALSE);
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, 4);
            func_ov099_021c24a4(wk, 0);
            wk->unk1355 = 0;
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_START;
        } else {
            func_ov099_021bfb90(wk, seq);
        }
    }
    return seq;
}

// The voice chat starts
// pokebw2's WifiP2PMatch_StateVChatStart
static int func_ov099_021bfb90(WIFIP2PMATCH_WORK *wk, int seq) {
    func_02039c68(func_020399a8(), 0x13, 10);
    func_02009b80(wk->wifiList, func_ov009_02156a3c());
    wk->seq = WIFIP2PMATCH_SEQ_VCHAT;
    return seq;
}

// Shows whether the friend is talking
// pokebw2's WifiP2PMatch_DrawVChatTalkIcon
static void func_ov099_021bfbbc(WIFIP2PMATCH_WORK *wk) {
    if (func_ov009_02156b68()) {
        func_ov099_021be3cc(&wk->icon, 1, 2, 1, 0x3c, 0);
    } else {
        func_ov099_021be3cc(&wk->icon, 1, 2, 1, 0x33, 0);
    }
}

// The voice chat
// pokebw2's WifiP2PMatch_StateVChat
static int func_ov099_021bfc04(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *status = func_ov099_021c23b8(func_ov009_02156a3c());

    func_ov099_021bdd48(wk, status);
    func_ov099_021bdd54(wk, status);
    func_ov099_021bf280(wk, WIFIP2PMATCH_HEAP);
    func_ov099_021c1bec(wk);
    if (GCTX_HIDGetPressedKeys() & PAD_BUTTON_B) {
        func_ov099_021baab8(wk, Bank0416_Text_017, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_END_YES_NO_START;
    } else if (wk->unk1355 != 0) {
        func_ov099_021ba8ec(wk);
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_016, FALSE);
        wk->timer = 60;
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_ENDED_MESSAGE;
    } else {
        func_ov099_021bfbbc(wk);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatEndYesNoStart
static int func_ov099_021bfc94(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bfbbc(wk);
    if (func_ov099_021ba928(wk)) {
        if (wk->unk1355 != 0) {
            func_ov099_021ba8ec(wk);
            func_ov099_021ba874(wk, func_ov009_02156a3c());
            func_ov099_021baab8(wk, Bank0416_Text_016, FALSE);
            wk->timer = 60;
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_ENDED_MESSAGE;
        } else {
            func_ov099_021ba828(wk);
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_END_YES_NO;
        }
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatEndYesNo
static int func_ov099_021bfcec(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_ov099_021bfbbc(wk);
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        if (wk->unk1355 != 0) {
            func_ov099_021ba8ec(wk);
            func_0201fef4(wk->yesNoMenu);
            wk->yesNoMenu = NULL;
            func_ov099_021ba874(wk, func_ov009_02156a3c());
            func_ov099_021baab8(wk, Bank0416_Text_016, FALSE);
            wk->timer = 60;
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_ENDED_MESSAGE;
        }
    } else if (ret == 0) {
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_END_SEND;
    } else {
        func_ov099_021baadc(wk, Bank0416_Text_123, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_START;
    }
    return seq;
}

// Tells the friend that the voice chat ends
// pokebw2's WifiP2PMatch_StateVChatEndSend
static int func_ov099_021bfd68(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (func_0203d3a8(func_020399a8(), 0xa08, 0, NULL)) {
        func_02039c68(func_020399a8(), 0x12, 10);
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_END_TIMING;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatStopWait
static int func_ov099_021bfda8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov009_021563ec()) {
        func_02039c68(func_020399a8(), 0x18, 10);
        wk->seq = WIFIP2PMATCH_SEQ_RETURN_TIMING_WAIT;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatEndTiming
static int func_ov099_021bfdd0(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->unk1355 = 0;
    if (func_02039cb4(func_020399a8(), 0x12, 10) || func_020399ec() != 2) {
        if (wk->result == 3 || wk->result == 4) {
            func_ov009_021563d0();
            func_ov099_021c24a4(wk, 0x7f);
            func_ov099_021ba8ec(wk);
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_CALL, 3);
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_STOP_WAIT;
        } else {
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
            func_0203d7b0(0);
            func_0203d7bc(FALSE);
            func_02039c68(func_020399a8(), 0x15, 10);
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_DISCONNECT_TIMING;
        }
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatDisconnectTiming
static int func_ov099_021bfe54(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_02039cb4(func_020399a8(), 0x15, 10) || func_020399ec() != 2) {
        func_0203c554(TRUE);
        wk->preConnect = -1;
        wk->timer = 20;
        func_ov099_021ba8ec(wk);
        wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_WAIT;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateVChatEndedMessage
static int func_ov099_021bfe9c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    wk->timer--;
    if (wk->timer < 0) {
        func_02039c68(func_020399a8(), 0x12, 10);
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_END_TIMING;
    }
    return seq;
}

// The connection failed or was called off: shows the message, then goes back to the room
// pokebw2's WifiP2PMatch_StateConnectFailed
static int func_ov099_021bfed0(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_0203cb00()) {
        func_ov099_021be900(wk);
        return seq;
    }
    if (func_ov099_021bdd48(wk, wk->myStatus) != WIFIP2PMATCH_STATUS_WAIT) {
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    }
    if (!func_ov099_021ba928(wk)) {
        wk->timer = 30;
        return seq;
    }
    wk->timer--;
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || wk->timer == 0) {
        func_ov099_021ba8ec(wk);
        func_0203c554(TRUE);
        func_ov099_021bdd0c(wk, FALSE);
        if (!func_ov099_021b96cc(&wk->room)) {
            wk->timer = 20;
            wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_WAIT;
        } else {
            func_ov099_021c19a8(wk);
            func_ov099_021baedc(wk);
            wk->preConnect = -1;
            wk->seq = WIFIP2PMATCH_SEQ_ROOM;
            func_ov099_021b9ae0(&wk->room, func_ov099_021be2a8(wk, func_ov099_021b97f4(&wk->room)));
            func_ov099_021c2408(wk);
        }
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateConnectFailedKeyWait
static int func_ov099_021bff9c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        func_ov099_021ba8ec(wk);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        func_0203c554(TRUE);
        wk->timer = 20;
        wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_WAIT;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateCutConnection
static int func_ov099_021bffe0(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->timer++;
    if (wk->timer <= 240) {
        return seq;
    }
    func_ov099_021b9ae0(&wk->room, func_ov099_021be2a8(wk, func_ov099_021b97f4(&wk->room)));
    func_ov099_021ba8ec(wk);
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    func_0203c554(TRUE);
    func_ov099_021bdd0c(wk, FALSE);
    if (!func_ov099_021b96cc(&wk->room)) {
        wk->seq = func_ov099_021be4f0(wk, FALSE);
    } else {
        func_ov099_021c19a8(wk);
        func_ov099_021baedc(wk);
        wk->preConnect = -1;
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        func_ov099_021c2408(wk);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateConnectError
static int func_ov099_021c0078(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_0203cb00()) {
        func_ov099_021be900(wk);
        return seq;
    }
    if (!func_ov099_021ba928(wk)) {
        wk->timer = 30;
        return seq;
    }
    wk->timer--;
    if ((GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) || wk->timer == 0) {
        func_ov099_021b9ae0(&wk->room, func_ov099_021be2a8(wk, func_ov099_021b97f4(&wk->room)));
        func_ov099_021ba8ec(wk);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        func_0203c554(TRUE);
        func_ov099_021bdd0c(wk, FALSE);
        if (!func_ov099_021b96cc(&wk->room)) {
            wk->seq = func_ov099_021be4f0(wk, FALSE);
        } else {
            func_ov099_021c19a8(wk);
            func_ov099_021baedc(wk);
            wk->preConnect = -1;
            wk->seq = WIFIP2PMATCH_SEQ_ROOM;
            func_ov099_021c2408(wk);
        }
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateNone16
static int func_ov099_021c0134(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// pokebw2's WifiP2PMatch_StateNone17
static int func_ov099_021c0138(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// pokebw2's WifiP2PMatch_StateNone18
static int func_ov099_021c013c(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// pokebw2's WifiP2PMatch_StateNoFriendKeyWait
static int func_ov099_021c0140(WIFIP2PMATCH_WORK *wk, int seq) {
    if (wk->timer > 0) {
        wk->timer--;
        return seq;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        func_ov099_021baab8(wk, WifiError_Text_026, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT;
        wk->timer = 1;
    }
    return seq;
}

// B in the room while seeking: asks whether to stop
// pokebw2's WifiP2PMatch_StateRecruitStopAsk
static int func_ov099_021c017c(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021b9b64(&wk->room);
    func_ov099_021baab8(wk, Bank0416_Text_007, FALSE);
    wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_STOP_YES_NO_START;
    return seq;
}

// pokebw2's WifiP2PMatch_StateRecruitStopYesNoStart
static int func_ov099_021c01a0(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021b9b64(&wk->room);
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_STOP_YES_NO;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateRecruitStopYesNo
static int func_ov099_021c01cc(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret = func_ov099_021ba858(wk);

    func_ov099_021b9b64(&wk->room);
    if (wk->preConnect == -1 && func_ov009_02156b8c() != -1) {
        // A friend connected meanwhile: the answer is no
        if (ret == BMPMENU_NULL) {
            func_0201fef4(wk->yesNoMenu);
            wk->yesNoMenu = NULL;
        }
        ret = 1;
    }
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        func_0203c554(TRUE);
        func_ov099_021bdd0c(wk, FALSE);
        func_ov099_021c2408(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    } else {
        func_ov099_021baadc(wk, Bank0416_Text_006, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
    }
    return seq;
}

// A in front of a PC: the menu of what to seek
// pokebw2's WifiP2PMatch_StateRecruitMenuStart
static int func_ov099_021c0254(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021bad50(wk);
    wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_MENU_MESSAGE;
    func_ov099_021b9b64(&wk->room);
    return seq;
}

// pokebw2's WifiP2PMatch_StateRecruitMenuMessage
static int func_ov099_021c0274(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021baab8(wk, Bank0416_Text_005, FALSE);
    wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_MENU;
    return seq;
}

// Frees the menu of what to seek
// pokebw2's WifiP2PMatch_FreeRecruitMenu
static void func_ov099_021c0290(WIFIP2PMATCH_WORK *wk) {
    if (wk->listWin != NULL) {
        wk->listWin = func_ov099_021ba8bc(wk->listWin);
        BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[4]);
        ListMenuCore_FreeOptionList(wk->menuOptions);
        func_ov099_021ba8ec(wk);
    }
}

static int func_ov099_021c02c8(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    countActivePkms(wk->party);
    func_ov099_021b9b64(&wk->room);
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    ret = (u32)BmpMenuList_Update(wk->list);
    switch (ret) {
    case BMPMENULIST_NULL:
        return seq;
    case BMPMENULIST_CANCEL:
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        func_ov099_021c0290(wk);
        func_ov099_021c2408(wk);
        return seq;
    case 7:
        func_ov099_021c0290(wk);
        wk->battleMode = wk->parent->unk21;
        wk->battleRule = wk->parent->unk22;
        wk->parent->unk48 = wk->parent->unk20;
        func_ov099_021bb954(wk, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_BATTLE_SETUP_MENU;
        return seq;
    case 4:
        if (!wk->parent->unk46) {
            return seq;
        }
        func_ov099_021c0290(wk);
        func_ov099_021c23c8(wk, -1);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
        break;
    case 6:
        if (!func_ov099_021bddd0(wk)) {
            wk->listWin = func_ov099_021ba8bc(wk->listWin);
            BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[4]);
            ListMenuCore_FreeOptionList(wk->menuOptions);
            func_ov099_021ba8ec(wk);
            func_ov099_021baab8(wk, Bank0416_Text_121, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_MESSAGE_KEY_WAIT_TO_ROOM;
            return seq;
        }
        func_ov099_021c0290(wk);
        func_ov099_021c23c8(wk, -1);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
        break;
    case 5:
        if (!wk->parent->unk46) {
            return seq;
        }
        if (!canPlayerExchangePhotos() && func_0202eb0c()) {
            func_ov099_021c0290(wk);
            func_ov099_021baab8(wk, Bank0416_Text_149, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_CAMERA_NOTICE_YES_NO_START;
            return seq;
        }
        func_ov099_021c0290(wk);
        func_ov099_021c23c8(wk, -1);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
        break;
    }
    func_ov099_021c23d8(wk, TRUE);
    wk->timer = 0;
    wk->unk246 = ret;
    wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_START;
    return seq;
}


// pokebw2's WifiP2PMatch_StateRecruitStart
static int func_ov099_021c047c(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->timer--;
    if (wk->timer < 0) {
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_RECRUIT, wk->unk246);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateCameraNoticeYesNoStart
static int func_ov099_021c04a4(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021b9b64(&wk->room);
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_CAMERA_NOTICE_YES_NO;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateCameraNoticeYesNo
static int func_ov099_021c04d0(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret = func_ov099_021ba858(wk);

    func_ov099_021b9b64(&wk->room);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        func_ov099_021c23c8(wk, -1);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_RECRUIT, 5);
        func_ov099_021c23d8(wk, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM_MESSAGE;
    } else {
        func_ov099_021c2408(wk);
        func_ov099_021ba8ec(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateConnectFriendYesNoStart
static int func_ov099_021c0528(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021b9b64(&wk->room);
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FRIEND_YES_NO;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateConnectFriendYesNo
static int func_ov099_021c0554(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *targetStatus = &wk->targetStatus;
    u32 friendNo = func_ov099_021b97f4(&wk->room);
    u32 ret = func_ov099_021ba858(wk);
    MCR_MOVEOBJ *obj = func_ov099_021be2a8(wk, friendNo);

    func_ov099_021b9b64(&wk->room);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->result = 2;
        if (func_ov099_021c27e0(targetStatus) == func_ov099_021c27e0(wk->myStatus)) {
            wk->seq = WIFIP2PMATCH_SEQ_JOIN_FRIEND;
        } else {
            if (func_ov099_021c27e0(wk->myStatus)) {
                func_ov099_021baab8(wk, Bank0416_Text_073, FALSE);
            } else {
                func_ov099_021baab8(wk, Bank0416_Text_072, FALSE);
            }
            wk->seq = WIFIP2PMATCH_SEQ_VCHAT_CHANGE_YES_NO_INIT;
        }
    } else {
        func_ov099_021ba8ec(wk);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        wk->preConnect = -1;
        func_ov099_021c2408(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        if (obj != NULL) {
            func_ov099_021b9ae0(&wk->room, obj);
        }
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateMessageKeyWaitToRoom
static int func_ov099_021c0618(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (GCTX_HIDGetPressedKeys()) {
        func_ov099_021ba8ec(wk);
        func_0203c554(TRUE);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        wk->preConnect = -1;
        func_ov099_021c2408(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    }
    return seq;
}

// pokebw2's WifiP2PMatch_StateBackToRoom
static int func_ov099_021c0664(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba8ec(wk);
    func_0203c554(TRUE);
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    wk->preConnect = -1;
    func_ov099_021c2408(wk);
    wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    return seq;
}

// pokebw2's WifiP2PMatch_StateRecruitMenuOld
static int func_ov099_021c0698(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    func_ov099_021b9b64(&wk->room);
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    ret = (u32)BmpMenuList_Update(wk->list);
    switch (ret) {
    case BMPMENULIST_NULL:
        return seq;
    case BMPMENULIST_CANCEL:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        wk->seq = WIFIP2PMATCH_SEQ_RECRUIT_MENU_START;
        break;
    default:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_RECRUIT, ret);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        func_ov099_021ba8ec(wk);
        break;
    }
    if (wk->seq == WIFIP2PMATCH_SEQ_ROOM) {
        func_ov099_021c23c8(wk, -1);
        func_ov099_021c23d8(wk, TRUE);
    }
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, &wk->menuCursor[wk->menuCursorType]);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    return seq;
}

// A in front of a friend: makes the friend face the player and says what the friend is doing, or calls the friend
// pokebw2's WifiP2PMatch_StateFriendSelect
static int func_ov099_021c0750(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 friendNo;
    MCR_MOVEOBJ *obj;
    u16 status, gameMode;
    u32 msgId;
    int game = 0;
    WIFI_STATUS *targetStatus = &wk->targetStatus;

    func_0203d1e8(func_020399a8(), 0);
    friendNo = func_ov099_021b97f4(&wk->room);
    obj = func_ov099_021be2a8(wk, friendNo);
    func_ov099_021b9a70(&wk->room, obj, func_ov099_021b9a60(func_ov099_021be2a8(wk, 0)));
    status = func_ov099_021bdd48(wk, targetStatus);
    gameMode = func_ov099_021bdd54(wk, targetStatus);
    func_ov099_021ba874(wk, friendNo - 1);
    if (gameMode == 6) {
        game = 1;
    } else if (gameMode == 5) {
        game = 2;
    } else if (gameMode == 4) {
        game = 3;
    } else if (func_ov099_021bdd60(gameMode)) {
        game = 4;
    }
    switch (status) {
    case WIFIP2PMATCH_STATUS_NONE:
        msgId = Bank0416_Text_089;
        break;
    case WIFIP2PMATCH_STATUS_WAIT:
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        func_ov099_021c08f8(wk);
        return seq;
    case WIFIP2PMATCH_STATUS_RECRUIT: {
        u32 msgIds[] = {
            Bank0416_Text_089,
            Bank0416_Text_003,
            Bank0416_Text_092,
            Bank0416_Text_090,
            Bank0416_Text_002,
        };

        msgId = msgIds[game];
        break;
    }
    case WIFIP2PMATCH_STATUS_PLAYING: {
        u32 msgIds[] = {
            Bank0416_Text_089,   Bank0416_Text_052,  Bank0416_Text_091,
            Bank0416_Text_053, Bank0416_Text_051,
        };

        msgId = msgIds[game];
        break;
    }
    case 5:
        msgId = Bank0416_Text_089;
        break;
    default:
        msgId = Bank0416_Text_089;
        break;
    }
    func_ov099_021baab8(wk, msgId, FALSE);
    wk->seq = WIFIP2PMATCH_SEQ_FRIEND_MENU_INIT;
    return seq;
}

// State 0x23: the message about the friend is done: opens the friend menu if the friend can still be joined
// pokebw2's WifiP2PMatch_StateFriendMenuInit
static int func_ov099_021c0858(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *targetStatus = &wk->targetStatus;
    MCR_MOVEOBJ *obj;
    u32 status;
    u32 gameMode;

    if (func_0203cb00()) {
        func_ov099_021be900(wk);
        return seq;
    }
    obj = func_ov099_021be2a8(wk, func_ov099_021b97f4(&wk->room));
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    status = func_ov099_021bdd48(wk, targetStatus);
    gameMode = func_ov099_021bdd54(wk, targetStatus);
    switch (status) {
    case WIFIP2PMATCH_STATUS_WAIT:
        func_ov099_021bad74(wk);
        wk->seq = WIFIP2PMATCH_SEQ_WAITING_FRIEND_MENU;
        break;
    case WIFIP2PMATCH_STATUS_RECRUIT:
    case 5:
        if (func_ov099_021bdd98(gameMode)) {
            func_ov099_021bad74(wk);
            wk->seq = WIFIP2PMATCH_SEQ_RECRUITING_FRIEND_MENU;
            break;
        }
        // fallthrough
    case WIFIP2PMATCH_STATUS_NONE:
    case WIFIP2PMATCH_STATUS_CALL:
    case WIFIP2PMATCH_STATUS_PLAYING:
    default:
        func_ov099_021b9ae0(&wk->room, obj);
        wk->seq = WIFIP2PMATCH_SEQ_MESSAGE_KEY_WAIT_TO_ROOM;
        break;
    }
    return seq;
}

// Calls the friend the player stands in front of. Returns FALSE when the friend cannot be called
// pokebw2's WifiP2PMatch_CallFriend
static BOOL func_ov099_021c08f8(WIFIP2PMATCH_WORK *wk) {
    int friendNo = func_ov099_021b97f4(&wk->room);
    int friendIndex;
    WIFI_STATUS *status;
    u32 wifiStatus;

    wk->friendNo = friendNo;
    if (friendNo != 0) {
        countActivePkms(wk->party);
        friendIndex = friendNo - 1;
        status = func_ov099_021c23b8(friendIndex);
        wifiStatus = func_ov099_021bdd48(wk, status);
        func_ov099_021c27e0(status);
        if (wifiStatus == 6) {
            return FALSE;
        }
        if (func_ov099_021c23c8(wk, -1)) {
            func_ov099_021c2810(wk->myStatus, func_ov099_021c23b8(friendIndex));
            wk->callTimer = 3600;
            func_ov099_021bde30(wk, WIFIP2PMATCH_STATUS_CALL, 3);
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_CALL, 3);
            func_ov099_021ba874(wk, friendNo - 1);
            wk->unk1351 = friendNo;
            func_ov099_021baadc(wk, Bank0416_Text_014, FALSE);
            func_ov099_021bc090(wk);
            wk->seq = WIFIP2PMATCH_SEQ_CALL_WAIT;
        } else {
            func_ov099_021ba874(wk, friendNo - 1);
            func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
        }
    }
    return TRUE;
}

// State 0x88: waits for the friend called from the menu to connect, until B is pressed or a minute has passed
// pokebw2's WifiP2PMatch_StateCallWait
static int func_ov099_021c09c0(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba928(wk);
    if (func_ov009_02156b8c() != -1) {
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    }
    if (func_ov099_021c23c0(wk->friendNo - 1) == 0) {
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        return seq;
    }
    wk->timer++;
    if (wk->timer % 900 == 0) {
        func_ov099_021c2810(wk->myStatus, func_ov099_021c23b8(wk->friendNo - 1));
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_CALL, 3);
    }
    if (wk->timer > 3600) {
        func_ov099_021ba874(wk, wk->friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        return seq;
    }
    if (GCTX_HIDGetPressedKeys() == PAD_BUTTON_B) {
        func_ov099_021baab8(wk, Bank0416_Text_071, FALSE);
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_CALL_CANCEL_YES_NO;
    }
    return seq;
}

// State 0x89: whether to stop calling the friend
// pokebw2's WifiP2PMatch_StateCallCancelYesNo
static int func_ov099_021c0a94(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        func_ov099_021baab8(wk, Bank0416_Text_074, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
    } else {
        func_ov099_021ba8ec(wk);
        func_ov099_021baab8(wk, Bank0416_Text_014, FALSE);
        func_ov099_021bc090(wk);
        wk->seq = WIFIP2PMATCH_SEQ_CALL_WAIT;
    }
    return seq;
}

// State 0x40: the menu of a friend who is waiting
// pokebw2's WifiP2PMatch_StateWaitingFriendMenu
static int func_ov099_021c0aec(WIFIP2PMATCH_WORK *wk, int seq) {
    BOOL keepMessage = FALSE;
    intptr_t ret = BmpMenuList_Update(wk->list);
    int friendNo = func_ov099_021b97f4(&wk->room);
    MCR_MOVEOBJ *obj = func_ov099_021be2a8(wk, friendNo);
    WIFI_STATUS *status;
    BOOL vchat;
    u32 gameMode;

    if (func_0203cb00()) {
        wk->listWin = func_ov099_021ba8bc(wk->listWin);
        BmpMenuList_Free(wk->list, NULL, NULL);
        ListMenuCore_FreeOptionList(wk->menuOptions);
        if (obj != NULL) {
            func_ov099_021b9ae0(&wk->room, obj);
        }
        func_ov099_021be900(wk);
        return seq;
    }
    switch (ret) {
    case BMPMENULIST_NULL:
        // The friend has left the room or is doing something else by now
        if (obj == NULL) {
            BmpMenuList_Free(wk->list, NULL, NULL);
            func_ov099_021ba874(wk, friendNo - 1);
            func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
            keepMessage = TRUE;
        } else {
            status = func_ov099_021c23b8(friendNo - 1);
            func_ov099_021bdd48(wk, status);
            vchat = func_ov099_021c27e0(status);
            gameMode = func_ov099_021bdd54(wk, status);
            if (gameMode != func_ov099_021bdd54(wk, &wk->targetStatus) ||
                vchat != func_ov099_021c27e0(&wk->targetStatus)) {
                func_ov099_021ba874(wk, friendNo - 1);
                func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
                wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
                keepMessage = TRUE;
            } else {
                return seq;
            }
        }
        break;
    case BMPMENULIST_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        break;
    default:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        if (ret == 1) {
            wk->seq = WIFIP2PMATCH_SEQ_ROOM;
            func_ov099_021c08f8(wk);
        } else if (ret == 2) {
            wk->seq = WIFIP2PMATCH_SEQ_FRIEND_DETAIL_INIT;
        }
        break;
    }
    if (!keepMessage) {
        func_ov099_021ba8ec(wk);
    }
    wk->listWin = func_ov099_021ba8bc(wk->listWin);
    BmpMenuList_Free(wk->list, NULL, NULL);
    ListMenuCore_FreeOptionList(wk->menuOptions);
    if (obj != NULL) {
        func_ov099_021b9ae0(&wk->room, obj);
    }
    return seq;
}

// pokebw2's WifiP2PMatch_ResetVChatVolume
static void func_ov099_021c0c7c(void) {
    func_ov009_02156bc4(0);
}

// State 0x24: the menu of a friend who is looking for someone to play with
// pokebw2's WifiP2PMatch_StateRecruitingFriendMenu
static int func_ov099_021c0c88(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *targetStatus = &wk->targetStatus;
    intptr_t ret = BmpMenuList_Update(wk->list);
    u32 friendNo;
    MCR_MOVEOBJ *obj;
    int gameMode;

    if (func_ov099_021bf050(wk)) {
        ret = BMPMENULIST_CANCEL;
    }
    friendNo = func_ov099_021b97f4(&wk->room);
    wk->friendNo = friendNo;
    obj = func_ov099_021be2a8(wk, friendNo);
    if (func_0203cb00()) {
        wk->listWin = func_ov099_021ba8bc(wk->listWin);
        BmpMenuList_Free(wk->list, NULL, NULL);
        ListMenuCore_FreeOptionList(wk->menuOptions);
        if (obj != NULL) {
            func_ov099_021b9ae0(&wk->room, obj);
        }
        func_ov099_021be900(wk);
        return seq;
    }
    if (ret != BMPMENULIST_NULL) {
        func_ov099_021ba8ec(wk);
        wk->listWin = func_ov099_021ba8bc(wk->listWin);
        BmpMenuList_Free(wk->list, NULL, NULL);
        ListMenuCore_FreeOptionList(wk->menuOptions);
    }
    switch (ret) {
    case BMPMENULIST_NULL:
        return seq;
    case BMPMENULIST_CANCEL:
        GFL_SndSEPlay(SEQ_SE_CANCEL1);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        break;
    default:
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
        gameMode = func_ov099_021bdd54(wk, targetStatus);
        if (gameMode == 5 && !canPlayerExchangePhotos() && func_0202eb0c()) {
            func_ov099_021baab8(wk, Bank0416_Text_149, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FRIEND_YES_NO_START;
            return seq;
        }
        if (gameMode == 6 && !func_ov099_021bddd0(wk)) {
            func_ov099_021baab8(wk, Bank0416_Text_121, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_MESSAGE_KEY_WAIT_TO_ROOM;
            break;
        }
        if (func_ov099_021bdd60(gameMode)) {
            wk->battleMode = (gameMode - 7) / 2;
            wk->battleRule = (gameMode - 7) % 2;
            func_ov099_021bcc4c(wk);
            if (!func_ov099_021bddf4(wk)) {
                func_ov099_021baab8(wk, Bank0416_Text_104, FALSE);
                wk->seq = WIFIP2PMATCH_SEQ_MESSAGE_KEY_WAIT_TO_ROOM;
            }
        }
        wk->result = 2;
        if (func_ov099_021c27e0(targetStatus) == func_ov099_021c27e0(wk->myStatus)) {
            wk->seq = WIFIP2PMATCH_SEQ_JOIN_FRIEND;
            break;
        }
        if (func_ov099_021c27e0(wk->myStatus)) {
            func_ov099_021baab8(wk, Bank0416_Text_073, FALSE);
        } else {
            func_ov099_021baab8(wk, Bank0416_Text_072, FALSE);
        }
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_CHANGE_YES_NO_INIT;
        break;
    }
    if (obj != NULL) {
        func_ov099_021b9ae0(&wk->room, obj);
    }
    return seq;
}

// State 0x76: connects to the friend who is looking for someone, if nothing has changed since
// pokebw2's WifiP2PMatch_StateJoinFriend
static int func_ov099_021c0e60(WIFIP2PMATCH_WORK *wk, int seq) {
    int friendNo = wk->friendNo;
    WIFI_STATUS *status;
    u32 wifiStatus;
    u32 gameMode;
    BOOL vchat;

    countActivePkms(wk->party);
    if (wk->friendNo == 0) {
        func_ov099_021baab8(wk, Bank0416_Text_074, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
    }
    status = func_ov099_021c23b8(friendNo - 1);
    wifiStatus = func_ov099_021bdd48(wk, status);
    gameMode = func_ov099_021bdd54(wk, status);
    vchat = func_ov099_021c27e0(status);
    if (wifiStatus != func_ov099_021bdd48(wk, &wk->targetStatus) ||
        gameMode != func_ov099_021bdd54(wk, &wk->targetStatus) ||
        vchat != func_ov099_021c27e0(&wk->targetStatus) || wifiStatus == 6) {
        func_ov099_021ba874(wk, friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
        return seq;
    }
    func_ov099_021c23c0(friendNo - 1);
    if (func_ov099_021c23c8(wk, friendNo - 1)) {
        wk->callTimer = 3600;
        func_ov099_021bde30(wk, WIFIP2PMATCH_STATUS_CALL, gameMode);
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_CALL, gameMode);
        func_ov099_021ba874(wk, friendNo - 1);
        func_ov099_021baadc(wk, Bank0416_Text_014, FALSE);
        func_ov099_021bc090(wk);
        wk->friendNo = friendNo;
        wk->seq = WIFIP2PMATCH_SEQ_JOIN_WAIT;
    } else {
        func_ov099_021ba874(wk, friendNo - 1);
        func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
    }
    return seq;
}

// State 0x25: waits for the connection to the friend
// pokebw2's WifiP2PMatch_StateJoinWait
static int func_ov099_021c0f70(WIFIP2PMATCH_WORK *wk, int seq) {
    MCR_MOVEOBJ *obj;

    func_ov099_021ba928(wk);
    wk->callTimer--;
    if (wk->callTimer < 0) {
        obj = func_ov099_021be2a8(wk, func_ov099_021b97f4(&wk->room));
        if (obj != NULL) {
            func_ov099_021b9ae0(&wk->room, obj);
        }
        wk->seq = WIFIP2PMATCH_SEQ_JOIN_TIMEOUT;
    } else if (func_0203c3ec() == 5) {
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
    } else if (func_ov009_02156d5c() == 5) {
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_012, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
    } else if (func_ov009_02156d5c() == 4 || func_0203c5ec()) {
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
    } else if (func_0203c3ec() == 1) {
        func_ov099_021c282c(wk->myStatus);
        func_ov099_021be6e4(wk);
        func_ov099_021bdd48(wk, wk->myStatus);
        wk->seq = WIFIP2PMATCH_SEQ_NEGOTIATION_WAIT;
        func_ov099_021ba8ec(wk);
        func_ov099_021ba7d8(wk);
        wk->timer = 30;
        func_0203d7b0(1);
        func_0203d7bc(TRUE);
    }
    return seq;
}

// State 0x3a
// pokebw2's WifiP2PMatch_StateDummy3A
static int func_ov099_021c105c(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// State 0x3b
// pokebw2's WifiP2PMatch_StateDummy3B
static int func_ov099_021c1060(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// State 0x3d: the friend did not answer: back to the room
// pokebw2's WifiP2PMatch_StateJoinTimeout
static int func_ov099_021c1064(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021c19a8(wk);
    func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    func_0203c554(TRUE);
    wk->preConnect = -1;
    wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    func_ov099_021c2408(wk);
    func_ov099_021ba8ec(wk);
    return seq;
}

// State 0x26
// pokebw2's WifiP2PMatch_StateDummy26
static int func_ov099_021c10a0(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// State 0x27
// pokebw2's WifiP2PMatch_StateDummy27
static int func_ov099_021c10a4(WIFIP2PMATCH_WORK *wk, int seq) {
    return seq;
}

// State 0x28: a friend has connected to the player
// pokebw2's WifiP2PMatch_StateFriendConnected
static int func_ov099_021c10a8(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *status;

    if (GFL_SndPlayerIsActive(0)) {
        return seq;
    }
    if (func_ov099_021c23c0(func_ov009_02156a3c()) == 0) {
        func_ov099_021ba874(wk, wk->preConnect);
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        return seq;
    }
    status = func_ov099_021c23b8(func_ov009_02156a3c());
    func_ov099_021bdd48(wk, wk->myStatus);
    func_ov099_021bdd48(wk, status);
    func_ov099_021c27e0(wk->myStatus);
    wk->friendNo = func_ov009_02156a3c() + 1;
    if (!wk->unk1351) {
        func_ov099_021ba874(wk, func_ov009_02156a3c());
        func_ov099_021baab8(wk, Bank0416_Text_008, FALSE);
    }
    func_ov099_021ba7d8(wk);
    wk->seq = WIFIP2PMATCH_SEQ_NEGOTIATION_WAIT;
    wk->timer = 30;
    return seq;
}

// Whether the connection to the friend was lost, which ends the state
// pokebw2's WifiP2PMatch_CheckDisconnect
static BOOL func_ov099_021c1154(WIFIP2PMATCH_WORK *wk) {
    if (func_0203c3ec() >= 3) {
        func_ov099_021ba874(wk, wk->preConnect);
        func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
    } else if (func_0203cb00()) {
        func_ov099_021be900(wk);
    } else {
        return FALSE;
    }
    wk->unk244 = 0;
    return TRUE;
}

// State 0x29: waits for the negotiation with the friend, then starts the exchange of the players' data
// pokebw2's WifiP2PMatch_StateNegotiationWait
static int func_ov099_021c11a0(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba928(wk);
    if (func_0203c3ec() >= 4 || func_0203c5ec()) {
        if (wk->unk244) {
            func_ov099_021baab8(wk, Bank0416_Text_068, FALSE);
        } else {
            func_ov099_021ba874(wk, wk->preConnect);
            func_ov099_021baab8(wk, Bank0416_Text_015, FALSE);
        }
        wk->unk244 = 0;
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
    } else if (!func_ov099_021c1154(wk)) {
        if (wk->timer > 1) {
            wk->timer--;
        }
        if (wk->timer == 0) {
            if (func_020399b8(func_020399a8())) {
                MI_CpuFill8(wk->connectGameMode, 0, sizeof(wk->connectGameMode));
                func_02039c68(func_020399a8(), 0xe, 10);
                wk->unk244 = 0;
                func_ov099_021baab8(wk, Bank0416_Text_069, FALSE);
                func_ov099_021bc090(wk);
                wk->seq = WIFIP2PMATCH_SEQ_SEND_PLAYER_INFO;
            }
        } else if (wk->timer == 1) {
            if (func_0203d1a4(func_020399a8())) {
                if (func_02039acc() == TRUE) {
                    wk->timer = 0;
                }
            } else if (func_020399ec()) {
                if (func_02039acc() == TRUE) {
                    wk->timer = 0;
                }
            }
        }
    }
    return seq;
}

// The partner's player data: adds the partner to the trainers the player has met
// pokebw2's WifiP2PMatch_RecvPlayerInfo
void func_ov099_021c1288(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WIFIP2PMATCH_WORK *wk = work;
    PlayerInfo *info;

    if (handle == func_020399a8()) {
        info = func_02012950(wk->gameData, netId);
        func_02008494(data, info);
        func_0200ed54(getHollow_RivalData(wk->save), getIDAsUInt(info));
    }
}

// The partner's party, which is received in the parent work's buffer
// pokebw2's func_ov099_021c12cc
void func_ov099_021c12cc(int netId, int size, const void *data, void *work, NetHandle *handle) {
}

// pokebw2's WifiP2PMatch_RecvCancel
void func_ov099_021c12d0(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WIFIP2PMATCH_WORK *wk = work;

    if (handle == func_020399a8()) {
        wk->unk1355 = 1;
    }
}

// pokebw2's func_ov099_021c12ec
void *func_ov099_021c12ec(int netId, void *work, int size) {
    WIFIP2PMATCH_WORK *wk = work;

    return wk->parent->parties[netId];
}

// The state both sides go to next, which state 0x48 is waiting for
// pokebw2's WifiP2PMatch_RecvNextSeq
void func_ov099_021c12fc(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WIFIP2PMATCH_WORK *wk = work;
    const u8 *recvSeq = data;

    if (handle == func_020399a8()) {
        if (wk->seq == WIFIP2PMATCH_SEQ_SYNC_SEQ_WAIT) {
            wk->recvSeq = *recvSeq;
            func_02039c68(func_020399a8(), 0x1a, 10);
        } else {
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_ERROR;
        }
    }
}

// The regulation the partner chose
// pokebw2's WifiP2PMatch_RecvRegulation
void func_ov099_021c1334(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WIFIP2PMATCH_WORK *wk = work;
    const u32 *regulationCode = data;

    if (handle == func_020399a8()) {
        func_ov099_021bcc58(wk, *regulationCode);
    }
}

void func_ov099_021c1350(int netId, int size, const void *data, void *work, NetHandle *handle) {
    WIFIP2PMATCH_WORK *wk = work;
    const WIFIP2PMATCH_CONNECT_DATA *connect = data;

    wk->connectGameMode[netId] = connect->gameMode;
    wk->parent->unk44[netId] = connect->vchat;
    func_ov099_021bcc58(wk, connect->regulationCode);
}


// State 0x2a: sends the player's data
// pokebw2's WifiP2PMatch_StateSendPlayerInfo
static int func_ov099_021c1374(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021c1154(wk) && func_02039cb4(func_020399a8(), 0xe, 10)) {
        if (func_0203d3a8(func_020399a8(), WIFIP2PMATCH_NET_CMD_BASE + 4, func_02008468(),
                          GetGameDataPlayerInfo(wk->gameData))) {
            func_ov099_021ba8ec(wk);
            wk->seq = WIFIP2PMATCH_SEQ_SEND_CONNECT_DATA;
        }
    }
    return seq;
}

// State 0x87: sends what the player is connecting for
// pokebw2's WifiP2PMatch_StateSendConnectData
static int func_ov099_021c13d0(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFIP2PMATCH_CONNECT_DATA connect;

    func_ov099_021bdd48(wk, wk->myStatus);
    connect.gameMode = func_ov099_021bdd54(wk, wk->myStatus);
    if (wk->unk1356) {
        connect.vchat = wk->unk1356 - 1;
        wk->unk1356 = 0;
    } else {
        connect.vchat = wk->parent->unk46;
    }
    connect.regulationCode = func_ov099_021bcbd8(wk, 1);
    if (func_0203d3a8(func_020399a8(), WIFIP2PMATCH_NET_CMD_BASE + 3, sizeof(connect), &connect)) {
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_DATA_WAIT;
    }
    return seq;
}

// State 0x2b: waits for what the partner is connecting for, which has to be what the player is
// pokebw2's WifiP2PMatch_StateConnectDataWait
static int func_ov099_021c143c(WIFIP2PMATCH_WORK *wk, int seq) {
    int netId = 0;
    u16 gameMode;

    if (func_0203d378()) {
        netId = 1;
    }
    if (!func_ov099_021c1154(wk) && wk->connectGameMode[netId] != 0) {
        gameMode = func_ov099_021bdd54(wk, wk->myStatus);
        if (gameMode == wk->connectGameMode[netId]) {
            func_02039c68(func_020399a8(), 0xf, 10);
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_TIMING;
        } else {
            func_ov099_021ba874(wk, func_ov009_02156a3c());
            func_ov099_021baab8(wk, Bank0416_Text_013, FALSE);
            wk->seq = WIFIP2PMATCH_SEQ_CONNECT_FAILED;
        }
    }
    return seq;
}

// State 0x2c
// pokebw2's WifiP2PMatch_StateConnectTiming
static int func_ov099_021c14a8(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021c1154(wk) && func_02039cb4(func_020399a8(), 0xf, 10)) {
        func_0203d7b0(1);
        func_0203d7bc(TRUE);
        func_02039c68(func_020399a8(), 0x12, 10);
        wk->seq = WIFIP2PMATCH_SEQ_CONNECT_END;
    }
    return seq;
}

// Whether the message is done or there is none
// pokebw2's WifiP2PMatch_IsMessageEndOrNone
static BOOL func_ov099_021c14ec(WIFIP2PMATCH_WORK *wk) {
    if (func_ov099_021ba928(wk)) {
        return TRUE;
    }
    if (wk->msgWin == NULL) {
        return TRUE;
    }
    return FALSE;
}

// State 0x2d: both sides are ready: goes on to what the partner is doing
// pokebw2's WifiP2PMatch_StateConnectEnd
static int func_ov099_021c1510(WIFIP2PMATCH_WORK *wk, int seq) {
    WIFI_STATUS *status;
    u32 gameMode;

    if (!func_ov099_021c1154(wk) && func_02039cb4(func_020399a8(), 0x12, 10) &&
        func_ov099_021c14ec(wk) == TRUE) {
        func_0200f4e4(func_020099c8(func_0201283c(wk->gameData), 0));
        func_ov099_021ba8ec(wk);
        status = func_ov099_021c23b8(func_ov009_02156a3c());
        func_ov099_021bdd48(wk, status);
        gameMode = func_ov099_021bdd54(wk, status);
        if (gameMode == 3) {
            wk->seq = WIFIP2PMATCH_SEQ_CONNECTED;
        } else {
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_PLAYING, gameMode);
            wk->seq = WIFIP2PMATCH_SEQ_GAME_START;
        }
    }
    return seq;
}

// State 0x19
// pokebw2's WifiP2PMatch_StateReturnToRoom
static int func_ov099_021c158c(WIFIP2PMATCH_WORK *wk, int seq) {
    GFL_BGSysClearBG(3);
    wk->unk1358 = 0;
    wk->seq = func_ov099_021be4f0(wk, FALSE);
    return seq;
}

// State 0x2e
// pokebw2's WifiP2PMatch_StateFriendDetailInit
static int func_ov099_021c15b4(WIFIP2PMATCH_WORK *wk, int seq) {
    wk->seq = WIFIP2PMATCH_SEQ_FRIEND_DETAIL_WAIT;
    return seq;
}

// State 0x2f: the friend's details, until a key is pressed
// pokebw2's WifiP2PMatch_StateFriendDetailWait
static int func_ov099_021c15c0(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (func_ov099_021bf03c(wk)) {
        wk->seq = WIFIP2PMATCH_SEQ_FRIEND_DETAIL_END;
    }
    if (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B)) {
        wk->seq = WIFIP2PMATCH_SEQ_FRIEND_DETAIL_END;
    }
    return seq;
}

// State 0x30
// pokebw2's WifiP2PMatch_StateFriendDetailEnd
static int func_ov099_021c15fc(WIFIP2PMATCH_WORK *wk, int seq) {
    func_ov099_021ba8ec(wk);
    wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    return seq;
}

// State 0x31: asks whether to leave the Wi-Fi Club
// pokebw2's WifiP2PMatch_StateExitYesNoInit
static int func_ov099_021c1614(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_EXIT_YES_NO;
    }
    return seq;
}

// State 0x32
// pokebw2's WifiP2PMatch_StateExitYesNo
static int func_ov099_021c1634(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        wk->yesNoMenu = NULL;
        func_ov099_021baab8(wk, WifiError_Text_026, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT;
        wk->timer = 1;
    } else {
        wk->yesNoMenu = NULL;
        func_ov099_021ba8ec(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
        func_ov099_021c2408(wk);
    }
    return seq;
}

// State 0x33: disconnects from Nintendo WFC
// pokebw2's WifiP2PMatch_StateDisconnect
static int func_ov099_021c169c(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    if (wk->timer == 1) {
        wk->timer = 0;
        func_0203ceac(NULL);
    }
    if (!func_0203cd98()) {
        func_02009b3c(wk->wifiList);
        func_ov099_021baab8(wk, WifiError_Text_027, TRUE);
        wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_END;
        wk->timer = 30;
    }
    return seq;
}

// State 0x34
// pokebw2's WifiP2PMatch_StateDisconnectEnd
static int func_ov099_021c16ec(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    wk->timer--;
    if (wk->timer == 0) {
        wk->endSeq = 0;
        wk->seq = WIFIP2PMATCH_SEQ_END;
        func_ov099_021ba8ec(wk);
    }
    return seq;
}

// State 0x35
// pokebw2's WifiP2PMatch_StateRetryYesNoInit
static int func_ov099_021c1724(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_02039cb4(func_020399a8(), 0x10, 10)) {
        return seq;
    }
    func_0203d7b0(0);
    func_0203d7bc(TRUE);
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_RETRY_YES_NO;
    }
    return seq;
}

// State 0x36
// pokebw2's WifiP2PMatch_StateRetryYesNo
static int func_ov099_021c1768(WIFIP2PMATCH_WORK *wk, int seq) {
    u32 ret;

    if (!func_ov099_021ba928(wk)) {
        return seq;
    }
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        func_ov099_021baab8(wk, Bank0416_Text_076, FALSE);
        func_ov099_021bc090(wk);
        wk->unk244 = 1;
        wk->seq = WIFIP2PMATCH_SEQ_NEGOTIATION_WAIT;
        wk->timer = 30;
    } else {
        func_ov099_021ba8ec(wk);
        func_0203c554(TRUE);
        wk->timer = 20;
        wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_WAIT;
    }
    return seq;
}

// State 0x37: asks whether to switch voice chat to the friend's setting
// pokebw2's WifiP2PMatch_StateVChatChangeYesNoInit
static int func_ov099_021c17d4(WIFIP2PMATCH_WORK *wk, int seq) {
    if (func_ov099_021ba928(wk)) {
        func_ov099_021ba828(wk);
        wk->seq = WIFIP2PMATCH_SEQ_VCHAT_CHANGE_YES_NO;
    }
    return seq;
}

// State 0x38
// pokebw2's WifiP2PMatch_StateVChatChangeYesNo
static int func_ov099_021c17f4(WIFIP2PMATCH_WORK *wk, int seq) {
    MCR_MOVEOBJ *obj = func_ov099_021be2a8(wk, func_ov099_021b97f4(&wk->room));
    u32 ret;

    func_0201dd68();
    ret = func_ov099_021ba858(wk);
    if (ret == BMPMENU_NULL) {
        return seq;
    }
    if (ret == 0) {
        func_ov099_021c27e8(wk->myStatus, 1 - func_ov099_021c27e0(wk->myStatus));
        func_ov099_021c2864(wk->myStatus, func_ov099_021c27e0(wk->myStatus));
        wk->unk1356 = 2 - wk->parent->unk46;
        wk->seq = WIFIP2PMATCH_SEQ_JOIN_FRIEND;
    } else {
        wk->result = 0;
        func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
        wk->preConnect = -1;
        if (obj != NULL) {
            func_ov099_021b9ae0(&wk->room, obj);
        }
        func_ov099_021c19a8(wk);
        func_ov099_021baedc(wk);
        wk->seq = WIFIP2PMATCH_SEQ_ROOM;
    }
    func_ov099_021ba8ec(wk);
    return seq;
}

// State 0x1a: ends the proc once the connection is closed
// pokebw2's WifiP2PMatch_StateEnd
static int func_ov099_021c18b4(WIFIP2PMATCH_WORK *wk, int seq) {
    if (!func_0203cd98()) {
        seq = 2;
    }
    return seq;
}

// Changes the player's status and tells the friends
// pokebw2's WifiP2PMatch_ChangeMyStatus
static void func_ov099_021c18c8(WIFIP2PMATCH_WORK *wk, u32 status, int gameMode) {
    if (wk->myStatus != NULL) {
        if (status == WIFIP2PMATCH_STATUS_WAIT && gameMode == 2) {
            func_ov099_021c282c(wk->myStatus);
        }
        if (status == WIFIP2PMATCH_STATUS_PLAYING && gameMode < 3) {
            func_02009b80(wk->wifiList, func_ov009_02156a3c());
        }
        func_ov099_021c1908(wk, status, gameMode);
        func_ov099_021be6e4(wk);
    }
}

// pokebw2's WifiP2PMatch_SetMyStatus
static void func_ov099_021c1908(WIFIP2PMATCH_WORK *wk, u32 status, u32 gameMode) {
    u32 oldStatus;
    u32 oldGameMode;

    if (wk->myStatus != NULL) {
        oldStatus = func_ov099_021bdd48(wk, wk->myStatus);
        oldGameMode = func_ov099_021bdd54(wk, wk->myStatus);
        if (oldStatus != status || oldGameMode != gameMode) {
            func_ov099_021bde30(wk, status, gameMode);
            func_ov099_021c27d4(wk->myStatus, status);
            func_ov099_021c27dc(wk->myStatus, gameMode);
            // The volume stays low while playing
            if ((gameMode != 5 || status != WIFIP2PMATCH_STATUS_PLAYING) && status != WIFIP2PMATCH_STATUS_PLAYING) {
                func_ov099_021c24a4(wk, 127);
            }
        }
        func_ov099_021baedc(wk);
    }
}

// Switches voice chat on or off. Returns the new setting
// pokebw2's WifiP2PMatch_ToggleVChat
BOOL func_ov099_021c1968(WIFIP2PMATCH_WORK *wk) {
    wk->parent->unk46 = 1 - wk->parent->unk46;
    func_ov099_021c27e8(wk->myStatus, wk->parent->unk46);
    func_ov099_021c2864(wk->myStatus, wk->parent->unk46);
    func_ov099_021be6e4(wk);
    return func_ov099_021c27e0(wk->myStatus);
}

// Sets the player's status back to the voice chat setting chosen
// pokebw2's WifiP2PMatch_ResetVChat
static void func_ov099_021c19a8(WIFIP2PMATCH_WORK *wk) {
    func_ov099_021c24a4(wk, 127);
    func_ov099_021c27e8(wk->myStatus, wk->parent->unk46);
    func_ov099_021c2864(wk->myStatus, wk->parent->unk46);
    func_ov099_021be6e4(wk);
    func_ov099_021c27e0(wk->myStatus);
}

// Closes the friend's details on the lower screen
// pokebw2's WifiP2PMatch_ViewCloseDetail
static void func_ov099_021c19dc(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    func_ov099_021bb42c(wk);
    func_ov099_021bb4d0(wk);
    func_ov099_021bb4ec(wk);
    func_ov099_021c1c70(wk);
    func_ov099_021bf29c(wk);
    func_ov099_021b98f0(&wk->room);
    func_ov099_021c205c(wk, heapId);
}

// pokebw2's WifiP2PMatch_ViewCloseDetailOf
static void func_ov099_021c1a14(WIFIP2PMATCH_WORK *wk, u32 friendNo, HeapID heapId) {
    if (friendNo == func_ov099_021c1c64(wk)) {
        func_ov099_021c19dc(wk, heapId);
    }
}

// pokebw2's WifiP2PMatch_ViewCloseDetailIfOpen
static void func_ov099_021c1a30(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    if (wk->view.detailMode != 0) {
        func_ov099_021c19dc(wk, heapId);
    }
}

// pokebw2's WifiP2PMatch_ViewExists
static BOOL func_ov099_021c1a44(WIFIP2PMATCH_WORK *wk) {
    if (wk->view.buttonMan != NULL) {
        return TRUE;
    }
    return FALSE;
}

// The frames of a pressed button's animation
static const u8 data_ov099_021c3720[16] = { 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2 };

// The buttons of the lower screen: two columns of four friends
static const TouchRect data_ov099_021c377c[WIFIP2PMATCH_VIEW_FRIEND_NUM + 1] = {
    { 0, 47, 0, 119 },     { 48, 95, 0, 119 },     { 96, 143, 0, 119 },
    { 144, 191, 0, 119 },  { 0, 47, 128, 255 },    { 48, 95, 128, 255 },
    { 96, 143, 128, 255 }, { 144, 191, 128, 255 }, { TOUCH_RECT_END, 0, 0, 0 },
};

// Creates the friends' page of the lower screen
// pokebw2's WifiP2PMatch_ViewInit
static void func_ov099_021c1a58(WIFIP2PMATCH_WORK *wk, ArcTool *arc, u32 heapId) {
    MI_CpuFill8(&wk->view, 0, sizeof(WIFIP2PMATCH_VIEW));
    GFL_BGSysSetBGEnabledB(7, TRUE);
    GFL_BGSysSetBGEnabledB(8, FALSE);
    wk->view.buttonMan = func_02051b84(data_ov099_021c377c, func_ov099_021c1c9c, wk, heapId);
    wk->view.wordSet = GFL_WordSetSystemCreateDefault(heapId);
    func_ov099_021c1d1c(wk, arc, heapId);
    wk->view.redrawAll = TRUE;
    func_ov099_021c208c(wk);
    func_ov099_021c2090(wk);
}

// pokebw2's WifiP2PMatch_ViewExit
static void func_ov099_021c1ac0(WIFIP2PMATCH_WORK *wk) {
    if (func_ov099_021c1a44(wk)) {
        func_ov099_021c1f74(wk);
        func_02051c6c(wk->view.buttonMan);
        wk->view.buttonMan = NULL;
        GFL_WordSetSystemFree(wk->view.wordSet);
        wk->view.wordSet = NULL;
        GFL_BGSysSetBGEnabledB(1, TRUE);
        GFL_BGSysSetBGEnabledB(14, FALSE);
    }
}

// Runs the friends' page. Returns the number of the friend whose button was touched, or 0
// pokebw2's WifiP2PMatch_ViewMain
u32 func_ov099_021c1b04(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    u32 param = func_ov099_021b9800(&wk->room);
    u32 underParam = func_ov099_021b984c(&wk->room);

    if (underParam >= WIFI_MCR_MAPPM_BLOCK0 && underParam <= WIFI_MCR_MAPPM_BLOCK3) {
        param = underParam;
    }
    // The page follows the block of the room the player is in
    if (param >= WIFI_MCR_MAPPM_BLOCK0 && param <= WIFI_MCR_MAPPM_BLOCK3) {
        if (wk->view.page != param - WIFI_MCR_MAPPM_BLOCK0) {
            wk->view.page = param - WIFI_MCR_MAPPM_BLOCK0;
            func_ov099_021c208c(wk);
            wk->view.redrawButtons = TRUE;
            wk->view.redrawAll = TRUE;
        }
    }
    if (wk->view.detailMode == 1) {
        wk->view.detailMode = 2;
        func_ov099_021bb2f0(wk, heapId);
    }
    if (wk->view.detailMode == 0) {
        func_ov099_021bb42c(wk);
        func_ov099_021bb4d0(wk);
        func_ov099_021bb4ec(wk);
        func_02051c88(wk->view.buttonMan);
        func_ov099_021c1cd4(wk);
        if (wk->view.redrawButtons == TRUE) {
            func_ov099_021c2090(wk);
            wk->view.redrawButtons = FALSE;
        }
    }
    if (wk->view.detailMode == 2 || wk->view.detailMode == 3) {
        if (func_ov099_021c1c10(wk) == TRUE) {
            wk->view.touchFriendNo = 0;
            wk->view.buttonFrame = 0;
            wk->view.redrawButtons = TRUE;
            wk->view.redrawAll = TRUE;
            func_ov099_021c19dc(wk, heapId);
            wk->view.detailMode = 0;
            wk->view.redrawAll = TRUE;
        }
    }
    return wk->view.touchFriendNo;
}

// pokebw2's WifiP2PMatch_ViewRedrawButtons
static void func_ov099_021c1bec(WIFIP2PMATCH_WORK *wk) {
    if (wk->view.detailMode == 0 && wk->view.redrawButtons == TRUE) {
        func_ov099_021c2090(wk);
        wk->view.redrawButtons = FALSE;
    }
}

// Whether the player closes the friend's details
// pokebw2's WifiP2PMatch_ViewDetailCloseInput
BOOL func_ov099_021c1c10(WIFIP2PMATCH_WORK *wk) {
    if (wk->view.detailMode == 3) {
        return FALSE;
    }
    if (wk->touchBar != NULL) {
        func_ov067_021b63d0(wk->touchBar);
        if (func_ov067_021b6448(wk->touchBar) == 1) {
            return TRUE;
        }
    }
    if ((GCTX_HIDGetHeldKeys() & (PAD_KEY_RIGHT | PAD_KEY_LEFT | PAD_KEY_UP | PAD_KEY_DOWN)) ||
        (GCTX_HIDGetPressedKeys() & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_X))) {
        return TRUE;
    }
    return FALSE;
}

// pokebw2's WifiP2PMatch_ViewGetTouchFriendNo
static u32 func_ov099_021c1c64(WIFIP2PMATCH_WORK *wk) {
    return wk->view.touchFriendNo;
}

// pokebw2's WifiP2PMatch_ViewResetTouch
static void func_ov099_021c1c70(WIFIP2PMATCH_WORK *wk) {
    if (func_ov099_021c1a44(wk) == TRUE) {
        wk->view.touchFriendNo = 0;
        wk->view.buttonFrame = 0;
        wk->view.detailMode = 0;
        wk->view.redrawButtons = TRUE;
        wk->view.redrawAll = TRUE;
    }
}

// A button of the friends' page was touched
// pokebw2's WifiP2PMatch_ViewButtonCallback
static void func_ov099_021c1c9c(u32 button, u32 event, void *work) {
    WIFIP2PMATCH_WORK *wk = work;
    u32 friendIndex = button + wk->view.page * WIFIP2PMATCH_VIEW_FRIEND_NUM;

    if (wk->view.friendState[friendIndex] == 2 && wk->view.touchFriendNo == 0 && event == 0) {
        wk->view.touchFriendNo = friendIndex + 1;
        GFL_SndSEPlay(SEQ_SE_DECIDE1);
    }
}

// Animates the touched button, after which the friend's details open
// pokebw2's WifiP2PMatch_ViewButtonAnime
void func_ov099_021c1cd4(WIFIP2PMATCH_WORK *wk) {
    if (wk->view.touchFriendNo != 0) {
        wk->view.buttonFrame = data_ov099_021c3720[wk->view.buttonAnimCount];
        wk->view.buttonAnimCount++;
        wk->view.redrawButtons = TRUE;
        if (wk->view.buttonAnimCount >= 16) {
            wk->view.buttonAnimCount = 0;
            wk->view.buttonFrame = 0;
            wk->view.detailMode = 1;
        }
    }
}

// Loads the graphics of the friends' page and creates its windows
// pokebw2's WifiP2PMatch_ViewGraphicInit
static void func_ov099_021c1d1c(WIFIP2PMATCH_WORK *wk, ArcTool *arc, u32 heapId) {
    int i;
    int j;
    int x;
    int y;

    GFL_G2DIOLoadArcNCLRDefault(arc, 6, 4, 0, 0x100, heapId);
    wk->viewBgChars = GFL_BGSysLoadArcNCGRDynamic(arc, 14, 4, 0, FALSE, heapId);
    GFL_BGSysLoadArcNCGRStatic(arc, 15, 6, 0x80, 0, FALSE, heapId);
    GFL_G2DIOLoadNSCRSync(arc, 22, 4, 0, (u16)wk->viewBgChars, 0, FALSE, heapId);
    wk->view.buttonScrnBuf = GFL_G2DIOReadNSCRArc(arc, 23, FALSE, &wk->view.buttonScrn, heapId);
    func_ov099_021c1fe8(wk->view.buttonScrn, 0x80);
    wk->view.backScrnBuf = GFL_G2DIOReadNSCRArc(arc, 25, FALSE, &wk->view.backScrn, heapId);
    func_ov099_021c1fe8(wk->view.backScrn, 0x80);
    GFL_BGSysLoadNCLRDefault(23, 5, 4, 0x1e0, 0x20, heapId);
    for (i = 0; i < WIFIP2PMATCH_VIEW_FRIEND_NUM; i++) {
        y = i % 4;
        x = i / 4;
        wk->view.nameWin[i] = BmpWin_CreateDynamic(7, x * 16 + 4, y * 6 + 1, 9, 3, 15, TRUE);
        GFL_BitmapFill(BmpWin_GetBitmap(wk->view.nameWin[i]), 0);
        BmpWin_FlushChar(wk->view.nameWin[i]);
        for (j = 0; j < 2; j++) {
            wk->view.statusWin[i][j] = BmpWin_CreateDynamic(5, x * 16 + 1 + j * 12, y * 6 + 1, 2, 3, 15, TRUE);
            GFL_BitmapFill(BmpWin_GetBitmap(wk->view.statusWin[i][j]), 0);
            BmpWin_FlushChar(wk->view.statusWin[i][j]);
        }
    }
    wk->view.detailWin[0] = BmpWin_CreateDynamic(7, 1, 1, 30, 2, 15, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->view.detailWin[0]), 0);
    BmpWin_FlushMap(wk->view.detailWin[0]);
    BmpWin_FlushChar(wk->view.detailWin[0]);
    wk->view.detailWin[1] = BmpWin_CreateDynamic(7, 1, 5, 30, 17, 15, TRUE);
    GFL_BitmapFill(BmpWin_GetBitmap(wk->view.detailWin[1]), 0);
    BmpWin_FlushMap(wk->view.detailWin[1]);
    BmpWin_FlushChar(wk->view.detailWin[1]);
    GFL_BGSysLoadScr(4);
    GFL_BGSysLoadScr(7);
}

// pokebw2's WifiP2PMatch_ViewGraphicExit
static void func_ov099_021c1f74(WIFIP2PMATCH_WORK *wk) {
    int i;
    int j;

    for (i = 0; i < WIFIP2PMATCH_VIEW_FRIEND_NUM; i++) {
        BmpWin_Free(wk->view.nameWin[i]);
        for (j = 0; j < 2; j++) {
            BmpWin_Free(wk->view.statusWin[i][j]);
        }
    }
    BmpWin_Free(wk->view.detailWin[0]);
    BmpWin_Free(wk->view.detailWin[1]);
    GFL_HeapFree(wk->view.buttonScrnBuf);
    GFL_HeapFree(wk->view.backScrnBuf);
}

// Adds to every tile of a screen, to use the characters loaded at an offset
// pokebw2's WifiP2PMatch_ViewScreenAddCharOffset
static void func_ov099_021c1fe8(NNSG2dScreenData *scrn, u16 add) {
    int y;
    int x;
    u16 *tiles = (u16 *)scrn->rawData;
    int width = scrn->screenWidth / 8;
    int height = scrn->screenHeight / 8;

    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            tiles[y * width + x] += add;
        }
    }
}

// state: 0 for a friend who is not online, 1 for one who is and 2 for one who is in the room
// pokebw2's WifiP2PMatch_ViewSetFriendState
static void func_ov099_021c2028(WIFIP2PMATCH_WORK *wk, unsigned long friendNo, u32 state) {
    wk->view.friendState[friendNo - 1] = state;
    wk->view.redrawButtons = TRUE;
}

// pokebw2's WifiP2PMatch_ViewClearFriendState
static void func_ov099_021c203c(WIFIP2PMATCH_WORK *wk, unsigned long friendNo) {
    wk->view.friendState[friendNo - 1] = 0;
    wk->view.redrawButtons = TRUE;
}

// pokebw2's WifiP2PMatch_ViewGetFriendState
static u8 func_ov099_021c2050(WIFIP2PMATCH_WORK *wk, unsigned long friendNo) {
    return wk->view.friendState[friendNo - 1];
}

// pokebw2's WifiP2PMatch_ViewRedraw
static void func_ov099_021c205c(WIFIP2PMATCH_WORK *wk, HeapID heapId) {
    if (wk->view.detailMode != 0) {
        func_ov099_021bb2f0(wk, heapId);
    } else {
        wk->view.redrawAll = TRUE;
        func_ov099_021c2090(wk);
    }
}

// pokebw2's WifiP2PMatch_ViewRequestRedrawAll
static void func_ov099_021c207c(WIFIP2PMATCH_WORK *wk) {
    wk->view.redrawAll = TRUE;
    wk->view.redrawButtons = TRUE;
}

// pokebw2's WifiP2PMatch_ViewPageChanged
static void func_ov099_021c208c(WIFIP2PMATCH_WORK *wk) {
}

// Draws the buttons of the page: all of them, or only the one that is animating
// pokebw2's WifiP2PMatch_ViewDrawButtons
static void func_ov099_021c2090(WIFIP2PMATCH_WORK *wk) {
    int i;
    int j;
    BOOL all;
    int friendNo;
    u32 type;
    int x;
    int y;
    u32 frame;
    BOOL redraw;

    if (wk->view.redrawAll == TRUE) {
        wk->view.redrawAll = FALSE;
        all = TRUE;
        GFL_BGSysClearBG(7);
    } else {
        all = FALSE;
    }
    for (i = 0; i < WIFIP2PMATCH_VIEW_FRIEND_NUM; i++) {
        friendNo = wk->view.page * WIFIP2PMATCH_VIEW_FRIEND_NUM + i + 1;
        if (wk->view.friendState[friendNo - 1] != 0) {
            if (func_02009864(wk->wifiList, friendNo - 1, 9) == 0) {
                type = WIFIP2PMATCH_VIEW_BUTTON_MALE;
            } else {
                type = WIFIP2PMATCH_VIEW_BUTTON_FEMALE;
            }
        } else {
            type = WIFIP2PMATCH_VIEW_BUTTON_NONE;
        }
        x = i / 4;
        y = i % 4;
        if (friendNo == wk->view.touchFriendNo) {
            frame = wk->view.buttonFrame;
        } else {
            frame = 0;
        }
        if (friendNo == wk->view.touchFriendNo || friendNo == wk->view.lastTouchFriendNo) {
            redraw = TRUE;
        } else {
            redraw = FALSE;
        }
        if (all == TRUE || redraw == TRUE) {
            func_ov099_021c2224(wk, x * 16, y * 6, type, frame);
            if (type != WIFIP2PMATCH_VIEW_BUTTON_NONE) {
                func_ov099_021c2274(wk, wk->view.nameWin[i], friendNo, frame, i);
                BmpWin_FlushMap(wk->view.nameWin[i]);
                func_ov099_021c22e8(wk, wk->view.statusWin[i], friendNo, frame, x * 16, y * 6);
            } else {
                GFL_BitmapFill(BmpWin_GetBitmap(wk->view.nameWin[i]), 0);
                BmpWin_FlushChar(wk->view.nameWin[i]);
                for (j = 0; j < 2; j++) {
                    GFL_BitmapFill(BmpWin_GetBitmap(wk->view.statusWin[i][j]), 0);
                    BmpWin_FlushChar(wk->view.statusWin[i][j]);
                }
            }
        }
    }
    GFL_BGSysQueueScrLoad(7);
    GFL_BGSysQueueScrLoad(6);
    GFL_BGSysSetBGEnabledB(2, TRUE);
    GFL_BGSysSetBGEnabledB(8, TRUE);
}

// Copies a button from the buttons' screen data: a column per type and a row per frame, and the empty one below
// pokebw2's WifiP2PMatch_ViewDrawButton
static void func_ov099_021c2224(WIFIP2PMATCH_WORK *wk, u8 x, u8 y, u8 type, u8 frame) {
    u16 srcX;
    u16 srcY;

    if (type == WIFIP2PMATCH_VIEW_BUTTON_NONE) {
        srcX = 0;
        srcY = 18;
    } else {
        srcX = type * 16;
        srcY = frame * 6;
    }
    GFL_BGSysLoadScrArea(6, x, y, 16, 6, wk->view.buttonScrn->rawData, srcX, srcY, 32, 24);
}

// Draws a friend's name
// pokebw2's WifiP2PMatch_ViewDrawFriendName
static void func_ov099_021c2274(WIFIP2PMATCH_WORK *wk, BmpWin *window, u32 friendNo, u32 frame, u32 index) {
    GFL_BitmapFill(BmpWin_GetBitmap(window), 0);
    GFL_TextRndUpdateColorIndexLUT(15, 2, 0);
    func_ov099_021c2370(wk, friendNo - 1);
    GFL_MsgDataLoadStrbuf(wk->msgData, Bank0416_Text_033, wk->expStrBuf);
    GFL_WordSetFormatStrbuf(wk->view.wordSet, wk->titleString, wk->expStrBuf);
    GFL_TextRendererDrawToBitmap(BmpWin_GetBitmap(window), 2, 8, wk->titleString, wk->font);
    BmpWin_FlushChar(window);
}

// Draws the icons of what a friend is doing and of the friend's voice chat setting
static void func_ov099_021c22e8(WIFIP2PMATCH_WORK *wk, BmpWin **windows, u32 friendNo, u32 frame, u32 x,
                                             u32 y) {
    int add = 0;
    WIFI_STATUS *status;
    u32 wifiStatus;
    u32 gameMode;
    int vchatIcon;

    if (frame != 0) {
        add = 16;
    }
    status = func_ov099_021c23b8(friendNo - 1);
    wifiStatus = func_ov099_021bdd48(wk, status);
    gameMode = func_ov099_021bdd54(wk, status);
    func_ov099_021be4bc(&wk->icon, 6, x + 2, y + 2, wifiStatus, gameMode, add);
    if (func_ov099_021c285c(status)) {
        vchatIcon = 15;
    } else {
        vchatIcon = 11;
    }
    func_ov099_021be3cc(&wk->icon, 6, x + 12, y + 2, vchatIcon + add, 0);
    GFL_BGSysQueueScrLoad(7);
}

// Puts a friend's name in the page's word set
// pokebw2's WifiP2PMatch_ViewSetFriendNameWord
static void func_ov099_021c2370(WIFIP2PMATCH_WORK *wk, int friendIndex) {
    u32 sex;

    if (friendIndex != -1) {
        sex = func_02009864(wk->wifiList, friendIndex, 9);
        func_020099e4(wk->wifiList, friendIndex, wk->expStrBuf);
        func_0201ee0c(wk->view.wordSet, 0, wk->expStrBuf, sex, 1, 2);
    }
}

// pokebw2's WifiP2PMatch_GetFriendStatusData
static WIFI_STATUS *func_ov099_021c23b8(int friendIndex) {
    return (WIFI_STATUS *)func_ov009_02156860(friendIndex);
}

// pokebw2's WifiP2PMatch_GetFriendStatus
static u8 func_ov099_021c23c0(int friendIndex) {
    return func_ov009_02156874(friendIndex);
}

// pokebw2's WifiP2PMatch_StartConnect
static BOOL func_ov099_021c23c8(WIFIP2PMATCH_WORK *wk, int a1) {
    func_ov099_021ba7d8(wk);
    return func_0203c2b0(a1);
}

// Stops the player in the room
// pokebw2's WifiP2PMatch_RoomPause
static void func_ov099_021c23d8(WIFIP2PMATCH_WORK *wk, BOOL message) {
    if (!wk->roomPaused) {
        wk->roomPaused = TRUE;
        func_ov099_021b98f8(&wk->room, TRUE);
        if (message == TRUE) {
            func_ov099_021baadc(wk, Bank0416_Text_006, FALSE);
        }
    }
}

// pokebw2's WifiP2PMatch_RoomResume
static void func_ov099_021c2408(WIFIP2PMATCH_WORK *wk) {
    if (wk->roomPaused == TRUE) {
        wk->roomPaused = FALSE;
        func_ov099_021ba8ec(wk);
        func_ov099_021b98f8(&wk->room, FALSE);
        wk->result = 0;
    }
}

// Fades the music's volume. Returns TRUE when the fade is over
// pokebw2's WifiP2PMatch_BgmVolumeFadeMain
static BOOL func_ov099_021c2438(WIFIP2PMATCH_WORK *wk) {
    int diff;

    if (wk->bgmFadeFrames != 0) {
        wk->bgmFadeFrames--;
        if (wk->bgmFadeFrames != 0) {
            diff = wk->bgmVolumeTarget - wk->bgmVolumeStart;
            if (diff < 0) {
                wk->bgmVolume = wk->bgmVolumeTarget + (-diff / 6) * wk->bgmFadeFrames;
            } else {
                wk->bgmVolume = wk->bgmVolumeTarget - (diff / 6) * wk->bgmFadeFrames;
            }
            GFL_SndBGMSetVolume(0xffff, wk->bgmVolume);
            return FALSE;
        }
        wk->bgmVolume = wk->bgmVolumeTarget;
        GFL_SndBGMSetVolume(0xffff, wk->bgmVolumeTarget);
    }
    return TRUE;
}

// pokebw2's WifiP2PMatch_BgmVolumeFadeStart
static void func_ov099_021c24a4(WIFIP2PMATCH_WORK *wk, u8 volume) {
    if (wk->bgmVolumeTarget != volume) {
        wk->bgmVolumeStart = wk->bgmVolume;
        wk->bgmFadeFrames = 6;
        wk->bgmVolumeTarget = volume;
        GFL_SndPlayerSetVolumeEx(volume, 0x3f);
        PokeVoice_SetMasterVolume(volume);
    }
}

// pokebw2's WifiP2PMatch_BgmVolumeInit
static void func_ov099_021c24d4(WIFIP2PMATCH_WORK *wk, u32 mode) {
    if (mode == 4) {
        wk->bgmVolumeTarget = 42;
    } else {
        wk->bgmVolumeTarget = 127;
    }
    wk->bgmVolumeStart = wk->bgmVolumeTarget;
}

// pokebw2's WifiP2PMatch_ProcInit
static BOOL func_ov099_021c24f0(GameProc *proc, u32 *state, void *param, void *work) {
    WifiClubData *parent = param;
    WIFIP2PMATCH_WORK *wk;

    GFL_HeapCreateChild(1, WIFIP2PMATCH_HEAP | 0x8000, 0xa4000);
    wk = GFL_ProcInitSubsystem(proc, sizeof(WIFIP2PMATCH_WORK), WIFIP2PMATCH_HEAP);
    MI_CpuFill8(wk, 0, sizeof(WIFIP2PMATCH_WORK));
    func_0203d668(wk);
    wk->myStatus = parent->buffer;
    wk->save = parent->save;
    wk->gameData = parent->gameData;
    wk->battleBoxParty = func_ov099_021bcb8c(wk->gameData);
    wk->party = GameData_GetParty(wk->gameData);
    wk->wifiList = func_0201283c(wk->gameData);
    wk->trainerData = getTrainerDataBlkAddress(parent->save);
    wk->initMode = parent->mode;
    wk->result = parent->unk49;
    if (parent->mode == 1) {
        parent->mode = 0;
        func_ov099_021c24d4(wk, 4);
    } else if (parent->mode != 0) {
        func_ov099_021c24d4(wk, 4);
    } else {
        func_ov099_021c24d4(wk, 1);
    }
    wk->endSeq = 0;
    wk->preConnect = -1;
    wk->parent = parent;
    GFL_BGSysSetBGEnabledA(15, FALSE);
    GFL_BGSysSetBGEnabledB(15, FALSE);
    func_ov099_021bb1a8(wk);
    wk->seq = WIFIP2PMATCH_SEQ_INIT;
    func_ov099_021bde94(wk);
    if (parent->mode == 0) {
        func_ov099_021be6f0(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
    }
    if (func_0203cd98()) {
        func_ov099_021ba7d8(wk);
        func_0203d34c(TRUE, WIFIP2PMATCH_HEAP);
    }
    return TRUE;
}

// The states by number. The last seven have no function
// pokebw2's sWifiP2PMatchSeqFuncs
static WifiP2PMatchSeqFunc data_ov099_021c3c0c[WIFIP2PMATCH_SEQ_MAX] = {
    func_ov099_021be600,                     // 0x00
    func_ov099_021be904,         // 0x01
    func_ov099_021be94c,      // 0x02
    func_ov099_021be96c,           // 0x03
    func_ov099_021be9c8,             // 0x04
    func_ov099_021be6b0,               // 0x05
    func_ov099_021beaec,              // 0x06
    func_ov099_021bed08,           // 0x07
    func_ov099_021bed3c,    // 0x08
    func_ov099_021bee94,                // 0x09
    func_ov099_021bf000,                // 0x0a
    func_ov099_021bf2c4,                     // 0x0b
    func_ov099_021bfa00,       // 0x0c
    func_ov099_021bfa28,            // 0x0d
    func_ov099_021bfad0,             // 0x0e
    func_ov099_021bfb04,         // 0x0f
    func_ov099_021bfb90,               // 0x10
    func_ov099_021bfc94,       // 0x11
    func_ov099_021bfcec,            // 0x12
    func_ov099_021bfed0,            // 0x13
    func_ov099_021bff9c,     // 0x14
    func_ov099_021c0078,             // 0x15
    func_ov099_021c0134,                   // 0x16
    func_ov099_021c0138,                   // 0x17
    func_ov099_021c013c,                   // 0x18
    func_ov099_021c158c,             // 0x19
    func_ov099_021c18b4,                      // 0x1a
    func_ov099_021c0140,          // 0x1b
    func_ov099_021c0254,         // 0x1c
    func_ov099_021c02c8,              // 0x1d
    func_ov099_021c0698,           // 0x1e
    func_ov099_021c017c,           // 0x1f
    func_ov099_021c01a0,    // 0x20
    func_ov099_021c01cc,         // 0x21
    func_ov099_021c0750,             // 0x22
    func_ov099_021c0858,           // 0x23
    func_ov099_021c0c88,     // 0x24
    func_ov099_021c0f70,                 // 0x25
    func_ov099_021c10a0,                  // 0x26
    func_ov099_021c10a4,                  // 0x27
    func_ov099_021c10a8,          // 0x28
    func_ov099_021c11a0,          // 0x29
    func_ov099_021c1374,           // 0x2a
    func_ov099_021c143c,          // 0x2b
    func_ov099_021c14a8,            // 0x2c
    func_ov099_021c1510,               // 0x2d
    func_ov099_021c15b4,         // 0x2e
    func_ov099_021c15c0,         // 0x2f
    func_ov099_021c15fc,          // 0x30
    func_ov099_021c1614,            // 0x31
    func_ov099_021c1634,                // 0x32
    func_ov099_021c169c,               // 0x33
    func_ov099_021c16ec,            // 0x34
    func_ov099_021c1724,           // 0x35
    func_ov099_021c1768,               // 0x36
    func_ov099_021c17d4,     // 0x37
    func_ov099_021c17f4,         // 0x38
    func_ov099_021bec60,           // 0x39
    func_ov099_021c105c,                  // 0x3a
    func_ov099_021c1060,                  // 0x3b
    func_ov099_021beb68,        // 0x3c
    func_ov099_021c1064,              // 0x3d
    func_ov099_021bebe8,             // 0x3e
    func_ov099_021bf950,           // 0x3f
    func_ov099_021c0aec,        // 0x40
    func_ov099_021bc14c,                // 0x41
    func_ov099_021bc1c4,           // 0x42
    func_ov099_021bc1e8,                // 0x43
    func_ov099_021bc228,          // 0x44
    func_ov099_021bc29c,         // 0x45
    func_ov099_021bc2c0,             // 0x46
    func_ov099_021bc51c,          // 0x47
    func_ov099_021bd63c,              // 0x48
    func_ov099_021bd678,        // 0x49
    func_ov099_021bd778,              // 0x4a
    func_ov099_021bc5c0,                 // 0x4b
    func_ov099_021bc5d4,                   // 0x4c
    func_ov099_021bc5e8,                 // 0x4d
    func_ov099_021bc850,     // 0x4e
    func_ov099_021bc5fc,         // 0x4f
    func_ov099_021bc620,              // 0x50
    func_ov099_021bc7b4,                // 0x51
    func_ov099_021bc76c,            // 0x52
    func_ov099_021bc784,          // 0x53
    func_ov099_021bc868,          // 0x54
    func_ov099_021bc940,      // 0x55
    func_ov099_021bc958,           // 0x56
    func_ov099_021bc9fc,      // 0x57
    func_ov099_021bca14,           // 0x58
    func_ov099_021bcab8,         // 0x59
    func_ov099_021bcad0,              // 0x5a
    func_ov099_021bccb8,        // 0x5b
    func_ov099_021bce80,                // 0x5c
    func_ov099_021bceac,            // 0x5d
    func_ov099_021bcecc,          // 0x5e
    func_ov099_021bcf98,                 // 0x5f
    func_ov099_021bcf9c,    // 0x60
    func_ov099_021bcfb4,         // 0x61
    func_ov099_021bd038,    // 0x62
    func_ov099_021bcd58,      // 0x63
    func_ov099_021bd0f0,         // 0x64
    func_ov099_021bd15c,           // 0x65
    func_ov099_021bd1a4,                // 0x66
    func_ov099_021bd318,                // 0x67
    func_ov099_021bd3a8,            // 0x68
    func_ov099_021bd41c,          // 0x69
    func_ov099_021bd444,              // 0x6a
    func_ov099_021bd4a8,       // 0x6b
    func_ov099_021bd500,   // 0x6c
    func_ov099_021bd558,         // 0x6d
    func_ov099_021bd584,             // 0x6e
    func_ov099_021bd5d8,      // 0x6f
    func_ov099_021bd5fc,           // 0x70
    func_ov099_021c0618,     // 0x71
    func_ov099_021c0664,               // 0x72
    func_ov099_021bdb30,                // 0x73
    func_ov099_021bdc0c,     // 0x74
    func_ov099_021bdc44,       // 0x75
    func_ov099_021c0e60,               // 0x76
    func_ov099_021bcdb0,  // 0x77
    func_ov099_021bce30,        // 0x78
    func_ov099_021bda3c,           // 0x79
    func_ov099_021bcf4c,       // 0x7a
    func_ov099_021bcf6c,       // 0x7b
    func_ov099_021bcf8c,       // 0x7c
    func_ov099_021bd838,             // 0x7d
    func_ov099_021bd864,      // 0x7e
    func_ov099_021bd888,           // 0x7f
    func_ov099_021bd8d8,                // 0x80
    func_ov099_021bd924,           // 0x81
    func_ov099_021bd97c,        // 0x82
    func_ov099_021c0274,       // 0x83
    func_ov099_021bd7e0,               // 0x84
    func_ov099_021bc79c,           // 0x85
    func_ov099_021bd8c0,              // 0x86
    func_ov099_021c13d0,          // 0x87
    func_ov099_021c09c0,                 // 0x88
    func_ov099_021c0a94,          // 0x89
    func_ov099_021bd994,            // 0x8a
    func_ov099_021bd9b4,                  // 0x8b
    func_ov099_021bd014,         // 0x8c
    func_ov099_021bd7d4,             // 0x8d
    func_ov099_021bd814,          // 0x8e
    func_ov099_021bc73c,             // 0x8f
    func_ov099_021bc6f4,                 // 0x90
    func_ov099_021bffe0,            // 0x91
    func_ov099_021bfd68,             // 0x92
    func_ov099_021bfdd0,           // 0x93
    func_ov099_021bfe9c,        // 0x94
    func_ov099_021bfc04,                    // 0x95
    func_ov099_021bc6b0,      // 0x96
    func_ov099_021bd9f4,                // 0x97
    func_ov099_021bd380,          // 0x98
    func_ov099_021bd7a0,                // 0x99
    func_ov099_021bfda8,            // 0x9a
    func_ov099_021bd6a0,            // 0x9b
    func_ov099_021bd6d0,        // 0x9c
    func_ov099_021bd0bc,    // 0x9d
    func_ov099_021bf6e0,              // 0x9e
    func_ov099_021bfe54,    // 0x9f
    func_ov099_021c04a4,   // 0xa0
    func_ov099_021c04d0,        // 0xa1
    func_ov099_021c0528,  // 0xa2
    func_ov099_021c0554,       // 0xa3
    func_ov099_021bc43c,       // 0xa4
    func_ov099_021bc468,            // 0xa5
    func_ov099_021bc4a8, // 0xa6
    func_ov099_021bc4d4,      // 0xa7
    func_ov099_021bd71c,             // 0xa8
    func_ov099_021bd744,               // 0xa9
    func_ov099_021bdaa4,         // 0xaa
    func_ov099_021bdac8,              // 0xab
    func_ov099_021bdb08,         // 0xac
    func_ov099_021c047c,             // 0xad
};

// pokebw2's WifiP2PMatch_ProcMain
static BOOL func_ov099_021c2604(GameProc *proc, u32 *state, void *param, void *work) {
    WIFIP2PMATCH_WORK *wk = work;
    WifiP2PMatchSeqFunc func;
    u8 letter;
    u8 shadow;
    u8 background;

    if (func_0203cd98()) {
        switch (func_ov009_02157170(1, 1)) {
        case 4:
            // The partner was lost: back to the room
            func_ov099_021bc0c4(wk);
            func_ov099_021bb0b4(wk);
            func_0203d7b0(0);
            func_0203d7bc(FALSE);
            func_ov099_021c18c8(wk, WIFIP2PMATCH_STATUS_WAIT, 2);
            wk->timer = 20;
            wk->seq = WIFIP2PMATCH_SEQ_DISCONNECT_WAIT;
            return FALSE;
        case 2:
            func_0202223c(0, 0);
            func_0202223c(1, 0);
            wk->endSeq = 1;
            return TRUE;
        }
    }
    if (!func_0200fb24()) {
        func = data_ov099_021c3c0c[wk->seq];
        if (func != NULL) {
            *state = func(wk, *state);
            if (*state == 2) {
                GFL_WipeSet(0, 0, 0, 0, 6, 1, WIFIP2PMATCH_HEAP);
                return TRUE;
            }
        }
    }
    if (wk->clActUnit != NULL) {
        func_0204a600();
    }
    if (wk->printQueue != NULL) {
        GFL_TextRndGetGlobalColors(&letter, &shadow, &background);
        GFL_TextRndUpdateColorIndexLUT(1, 2, 15);
        GFL_TCBExMgrUpdate(wk->tcbManager);
        GFL_TextRndUpdateColorIndexLUT(letter, shadow, background);
        func_0201c4e4(wk->printQueue);
    }
    if (wk->listPrintQueue != NULL) {
        func_0201c4e4(wk->listPrintQueue);
    }
    func_ov099_021c2438(wk);
    return FALSE;
}

// pokebw2's WifiP2PMatch_ProcExit
static BOOL func_ov099_021c2724(GameProc *proc, u32 *state, void *param, void *work) {
    WIFIP2PMATCH_WORK *wk = work;
    WifiClubData *parent = param;

    if (!func_ov099_021c2438(wk)) {
        return FALSE;
    }
    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (func_0203cd98()) {
        func_ov099_021ba7f8(wk);
    }
    parent->unk49 = wk->result;
    if (wk->regulation != NULL) {
        func_0200ae08(wk->regulation, parent->regulation);
        GFL_HeapFree(wk->regulation);
        wk->regulation = NULL;
    }
    func_ov099_021bc0c4(wk);
    func_ov099_021bdf7c(wk);
    parent->friendIndex = wk->friendNo;
    parent->mode = wk->endSeq;
    if (func_0203cd98()) {
        parent->unk10 = func_ov009_02156a3c();
    }
    func_ov099_021bcbb0(wk->battleBoxParty);
    func_ov099_021bb224(wk);
    GFL_ProcReleaseSubsystem(proc);
    GFL_HeapDelete(WIFIP2PMATCH_HEAP);
    return TRUE;
}

// pokebw2's WIFICLUB_PROC_FUNCTIONS (event_wificlub.c runs it)
const GameProcFunctions data_ov099_021c37e8 = {
    func_ov099_021c24f0,
    func_ov099_021c2604,
    func_ov099_021c2724,
};
