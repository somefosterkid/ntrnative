// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/pokelist.h, the lines in use so far,
// with Black's layout.
#ifndef BWPORT_APP_POKELIST_H
#define BWPORT_APP_POKELIST_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/g3d.h"
#include "gfl/particle.h"
#include "nnsys/g2d.h"
#include "system/printsys.h"
#include "gfl/touchpanel.h"
#include "field/player_action.h"

// A partner's team, shown beside the player's in a multi battle's selection
typedef struct {
    PokeParty *party;
    u16 *name;
    u8 gender;
} PokeListPartnerParam;

// A Pokémon of a partner's team: its icon, item, sex and level
// Black 2's; twinned code confirms the offsets of sexWindow, levelWindow, iconChars, icon and itemIcon in Black
typedef struct {
    BOOL dirty;
    BmpWin *sexWindow;
    BmpWin *levelWindow;
    u32 iconChars;
    ClActor *icon;
    ClActor *itemIcon;
} PokeListPartnerPkm;

// A partner's team as the multi battle's selection shows it: a dirty flag, its name window and the six Pokémon
struct PokeListPartner {
    BOOL dirty;
    BmpWin *window;
    PokeListPartnerPkm *pkms[6];
};

// Black's party list parameters are 0x98 bytes, 0x10 fewer than Black 2's, all of them between regulation and mode
typedef struct {
    PokeParty *party;
    BagSave *bag;
    MailData *unk08;
    TrainerDataSave *trainerData;
    void *unk10;
    Regulation *regulation;
    PokeDexSave *pokedex;
    PlayerActionPossibilities action;
    u16 zoneId;
    // What the screen is for
    int mode;
    u32 unk38;
    // The Pokémon whose summary to show, when result is 1
    u32 index;
    // 0 once the screen is done, and 1 to show a summary
    u32 result;
    u16 item;
    u16 move;
    // The move slot to teach the move to
    u8 moveSlot;
    // The party slots, from 1, of the Pokémon picked
    u8 picked[6];
    // Where to go on in the moves a Pokémon learns at its new level
    u32 learnIndex;
    u16 unk54;
    u32 unk58;
    u16 mailSlot;
    // The party slots, as bits, of the Pokémon that can learn the move
    u8 learnMask;
    u8 unk5F;
    // Seconds left to pick, counted down while timerEnabled is set
    u16 timeLeft;
    u8 unk62[2];
    u32 partnerCount;
    PokeListPartnerParam partners[3];
    BOOL showPartners;
    BOOL timerEnabled;
    u8 battleMsg;
    u8 unk95[3];
} PokeListParam;

// pokebw2's data_ov091_021bed00
extern GameProcFunctions data_ov091_021bed00;

GameEvent *EventPokeList_Create(GameSystem *gsys, Field *field, PokeListParam *param, void *summaryParam);

// status_rcv.c
// Whether the item would do anything to the Pokémon, pos being the move slot for items that act on one move
BOOL func_ov091_021bdd78(PartyPkm *pkm, u16 item, u16 pos, u32 heapId);
// Uses the item on the Pokémon, and whether it changed anything
BOOL func_ov091_021be2b4(PartyPkm *pkm, u16 item, u16 pos, u16 zoneId, u32 heapId);

// Black 2's, not checked against Black
#define POKELIST_PLATE_COUNT 6

// A plate's entry in a battle's selection, when it is not one of the order's places from 0
#define POKELIST_ENTRY_ABLE 6

// Black 2's, not checked against Black
#define POKELIST_CL_RES_COUNT 23

// Black 2's; twinned code confirms the offsets of heapId, touch and glowPaused in Black
// The list's work, which every file of the overlay shares
struct PokeListWork {
    u16 heapId;
    TCB *vblankTask;
    BOOL touch;
    u8 state;
    // The state to go to once the wipe is done
    u8 nextState;
    u8 subState;
    u8 input;
    // Holds the glow of the selected plate
    BOOL glowPaused;
    BOOL showShortcutButtons;
    // The modes that the list rewrites at its start, and gives back at its end
    BOOL wasMode18;
    BOOL wasMode19;
    BOOL wasMode1A;
    BOOL playHealSe;
    BOOL unk28;
    // 0 to 5 for the plates, then the buttons
    s32 cursorPos;
    // 9 when nothing is selected
    s32 selectPos;
    s32 selectPos2;
    PartyPkm *pkm;
    u32 menuItem;
    // The palettes that glow, as loaded, and as blended
    u16 baseColors[3][16];
    u16 colors[3][16];
    u8 flashTimer;
    u16 glowAngle;
    u8 unk104;
    u8 enteredCount;
    void *buttons[2];
    u16 unk110;
    u16 unk112;
    u8 waitTimer;
    BOOL decided;
    u16 prevHp;
    s8 hpStep;
    // Called once a plate's HP is done changing
    void (*hpDoneFunc)(PokeListWork *wk);
    // The stats before a level up
    u16 prevStats[6];
    BmpWin *statsWindow;
    MsgData *msgData;
    Font *font;
    Font *smallFont;
    PrintQueue *printQueue;
    BOOL msgWaitInput;
    // Called once a message is done
    void (*msgDoneFunc)(PokeListWork *wk);
    // Called with the item picked in the menu
    void (*menuFunc)(PokeListWork *wk, u32 item);
    PokeListMessage *message;
    PokeListMenu *menu;
    void *plateScreenFile;
    NNSG2dScreenData *plateScreen;
    PokeListPlate *plates[POKELIST_PLATE_COUNT];
    // Palettes, then characters, then cell animations
    u32 clResources[POKELIST_CL_RES_COUNT];
    ClActUnit *actorUnit;
    ClActor *cursor;
    ClActor *subCursor;
    ClActor *exitButton;
    ClActor *shortcutButton;
    ClActor *pressedButton;
    ClActUnit *partnerUnit;
    PokeListPartner *partners[3];
    BmpWin *timerWindows[2];
    BOOL timerWindowDirty[2];
    u8 shownBattleMsg;
    u8 shownTime;
    u16 timerBaseColors[16];
    u16 timerColors[16];
    u16 timerBlink;
    BOOL timeUp;
    void *taskMenuRes;
    G3DCamera *camera;
    // plist_demo.c's particle heap and particle system (Black 2 has two of each)
    void *particleHeap;
    ParticleSystem *particles;
    // The form change shown
    u32 demo;
    u8 demoTimer;
    u8 demoLength;
    u8 demoStep;
    u8 demoForm;
    u32 demoSe;
    u32 voice;
    u32 unk274;
    BOOL unk278;
    u32 unk27c;
    PokeListParam *param;
};

// pokebw2's PokeListMenu_Create
// plist_menu.c
PokeListMenu *func_ov091_021bba8c(PokeListWork *wk);
// pokebw2's PokeListMenu_Free
void func_ov091_021bbae0(PokeListWork *wk, PokeListMenu *menu);
// pokebw2's PokeListMenu_Close
void func_ov091_021bbba0(PokeListWork *wk, PokeListMenu *menu);
// pokebw2's PokeListMenu_Update
void func_ov091_021bbbd8(PokeListWork *wk, PokeListMenu *menu);

// The message window's shapes
#define POKELIST_MESSAGE_WINDOW_SHORT 0

#define POKELIST_MESSAGE_WINDOW_SHORTER 1
#define POKELIST_MESSAGE_WINDOW_MENU 2
#define POKELIST_MESSAGE_WINDOW_WIDE 3
#define POKELIST_MESSAGE_WINDOW_NONE 4

// pokebw2's PokeListMessage_Create
// plist_message.c
PokeListMessage *func_ov091_021bb594(PokeListWork *wk);
// pokebw2's PokeListMessage_Free
void func_ov091_021bb5fc(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_Update
void func_ov091_021bb644(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_Open
void func_ov091_021bb714(PokeListWork *wk, PokeListMessage *msg, u32 windowType);
// pokebw2's PokeListMessage_Close
void func_ov091_021bb7a4(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_IsOpen
BOOL func_ov091_021bb7d8(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_IsDone
BOOL func_ov091_021bb8fc(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_CreateWordSet
// The word set that the messages printed are expanded with, and its words
void func_ov091_021bb97c(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_FreeWordSet
void func_ov091_021bb98c(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_SetPkmName
void func_ov091_021bb99c(PokeListWork *wk, PokeListMessage *msg, u32 index, PartyPkm *pkm);
// pokebw2's PokeListMessage_SetStatName
void func_ov091_021bb9dc(PokeListWork *wk, PokeListMessage *msg, u32 index, u32 stat);
// pokebw2's PokeListMessage_SetNumber
void func_ov091_021bb9ec(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 number, u8 digits);
// pokebw2's PokeListMessage_SetString
void func_ov091_021bba0c(PokeListWork *wk, PokeListMessage *msg, u32 index, const StrBuf *str, u32 a4);
// pokebw2's PokeListMessage_LoadFrame
void func_ov091_021bba54(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_ShowWaitIcon
void func_ov091_021bba68(PokeListWork *wk, PokeListMessage *msg);
// pokebw2's PokeListMessage_DrawKeyCursor
void func_ov091_021bba70(PokeListWork *wk, PokeListMessage *msg);

// Black 2's, not checked against Black
// The moves to teach that stand for a set of moves: the ultimate moves, and the pledges
#define POKELIST_MOVE_ULTIMATE 0xfffe

// Black 2's, not checked against Black
#define POKELIST_MOVE_PLEDGE 0xfffd
#define CL_RES_NONE 0xffffffff
// The palettes, characters and cell animations in PokeListWork's clResources
#define CL_RES_PLTT(i) (i)

// Black 2's, not checked against Black
#define CL_RES_CHAR(i) (8 + (i))
#define CL_RES_CELL(i) (15 + (i))
#define CL_RES_PLTT_COUNT 8
#define CL_RES_CHAR_COUNT 7
#define CL_RES_CELL_COUNT 8

// pokebw2's PokeList_IsItemForParty
BOOL func_ov091_021bc1b4(PokeListWork *wk, u16 item);
// pokebw2's PokeListPlate_Redraw
void func_ov091_021bb2c0(PokeListWork *wk, PokeListPlate *plate);
// pokebw2's PokeList_UpdateArceusForm
void func_ov091_021bc24c(PokeListWork *wk, PartyPkm *pkm, u16 item);
// pokebw2's PokeList_UpdateGenesectForm
void func_ov091_021bc288(PokeListWork *wk, PartyPkm *pkm, u16 item);
// pokebw2's PokeList_IsItemForMove
// plist_item.c
BOOL func_ov091_021bc17c(PokeListWork *wk, u16 item);
// pokebw2's PokeListPlate_SetPkm
void func_ov091_021bb200(PokeListWork *wk, PokeListPlate *plate, PartyPkm *pkm, int step);
// pokebw2's PokeListPlate_GetHp
u16 func_ov091_021bb464(PokeListWork *wk, PokeListPlate *plate);
// pokebw2's PokeListPlate_GetTouchRect
void func_ov091_021bb3fc(PokeListWork *wk, PokeListPlate *plate, TouchRect *rect);
// pokebw2's PokeListPlate_GetCursorPos
void func_ov091_021bb3d4(PokeListWork *wk, PokeListPlate *plate, ClActorPos *pos);
// pokebw2's PokeListPlate_SetEntry
void func_ov091_021bb440(PokeListWork *wk, PokeListPlate *plate, int entry);
// pokebw2's PokeListPlate_ClearSlid
void func_ov091_021bb1a0(PokeListWork *wk, PokeListPlate *plate, int step);
// pokebw2's PokeList_LearnMessageDone
void func_ov091_021bc6c0(PokeListWork *wk);
// pokebw2's PokeList_CreateBG0
// BG 0 as a text BG, or as the 3D BG with the camera and particles of plist_demo.c's form changes
void func_ov091_021b6f0c(PokeListWork *wk);
// pokebw2's PokeList_ReleaseBG0
void func_ov091_021b7034(PokeListWork *wk);
// pokebw2's PokeList_GetHidenResult
u32 func_ov091_021b99e4(PartyPkm *pkm, u8 slot);
// pokebw2's PokeList_MessageDoneExit
void func_ov091_021b9e3c(PokeListWork *wk);

// Black 2's, not checked against Black
#define POKELIST_ENTRY_UNABLE 7
// Not a battle's selection, or an empty slot
#define POKELIST_ENTRY_EMPTY 8

// pokebw2's PokeListPlate_SetPalette
void func_ov091_021bb050(PokeListWork *wk, PokeListPlate *plate, u32 palette);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's PokeListMenu_Open
void func_ov091_021bbaf8(PokeListWork *wk, PokeListMenu *menu, const u32 *items);
// pokebw2's PokeListMenu_OpenYesNo
void func_ov091_021bbb40(PokeListWork *wk, PokeListMenu *menu);
// pokebw2's PokeListMenu_GetPicked
u32 func_ov091_021bbbe4(PokeListWork *wk, PokeListMenu *menu);
// pokebw2's PokeListMenu_CreateButton
void *func_ov091_021bbdec(PokeListWork *wk, PokeListMenu *menu, u32 msgId, u8 x, u8 y, BOOL isBack);
// pokebw2's PokeListMenu_FreeButton
void func_ov091_021bbe44(void *button);
// pokebw2's PokeListMenu_UpdateButton
void func_ov091_021bbe50(void *button);
// pokebw2's PokeListMenu_SetButtonActive
void func_ov091_021bbe5c(void *button, BOOL active);
// pokebw2's PokeListMenu_SetButtonPressed
void func_ov091_021bbe68(void *button, BOOL pressed);
// pokebw2's PokeListMessage_Print
void func_ov091_021bb7e8(PokeListWork *wk, PokeListMessage *msg, u32 msgId);
// pokebw2's PokeListMessage_PrintStream
void func_ov091_021bb860(PokeListWork *wk, PokeListMessage *msg, u32 msgId, BOOL waitInput);
// pokebw2's PokeListMessage_SetItemName
void func_ov091_021bb9ac(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 item);
// pokebw2's PokeListMessage_SetItemTextName
void func_ov091_021bb9bc(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 item);
// pokebw2's PokeListMessage_SetMoveName
void func_ov091_021bb9cc(PokeListWork *wk, PokeListMessage *msg, u32 index, u16 move);
// pokebw2's PokeListDemo_CanBecomeOrigin
BOOL func_ov091_021bdbf8(PokeListWork *wk, PartyPkm *pkm);
// pokebw2's PokeListDemo_SetOrigin
void func_ov091_021bdc3c(PokeListWork *wk, PartyPkm *pkm);
// pokebw2's PokeListDemo_CanBecomeAltered
BOOL func_ov091_021bdc5c(PokeListWork *wk, PartyPkm *pkm);
// pokebw2's PokeListDemo_SetAltered
void func_ov091_021bdca0(PokeListWork *wk, PartyPkm *pkm);
// pokebw2's PokeList_Init
// plist_sys.c
BOOL func_ov091_021b660c(PokeListWork *wk);
// pokebw2's PokeList_Exit
BOOL func_ov091_021b6818(PokeListWork *wk);
// pokebw2's PokeList_Main
BOOL func_ov091_021b68f0(PokeListWork *wk);
// Shows the cursor, or the second cursor, on a plate
void func_ov091_021b8f68(PokeListWork *wk, s32 pos);
void func_ov091_021b8fc4(PokeListWork *wk, s32 pos);
void func_ov091_021b90f8(PokeListWork *wk);
// Prints a message in the wide window, and calls doneFunc once it is read
void func_ov091_021b9788(PokeListWork *wk, u32 msgId, BOOL waitInput, void (*doneFunc)(PokeListWork *wk));
void func_ov091_021b999c(PokeListWork *wk);
// pokebw2's PokeList_Init3D
void func_ov091_021b6f28(PokeListWork *wk);
// pokebw2's PokeList_Exit3D
void func_ov091_021b7040(PokeListWork *wk);
// pokebw2's PokeList_IsBattle
BOOL func_ov091_021b9a84(PokeListWork *wk);
// pokebw2's PokeList_CheckLearnMove
u32 func_ov091_021b9ac8(PokeListWork *wk, PartyPkm *pkm, u8 pos);
// pokebw2's PokeList_CanEvolveWithItem
BOOL func_ov091_021b9bf4(PokeListWork *wk, PartyPkm *pkm, u16 item);
// pokebw2's PokeList_PrintString
void func_ov091_021b9c1c(PokeListWork *wk, BmpWin *window, u16 msgId, u16 x, s16 y, u16 color);
// pokebw2's PokeList_PrintStringSmall
void func_ov091_021b9c64(PokeListWork *wk, BmpWin *window, u32 msgId, int x, s16 y, u16 color);
// pokebw2's PokeList_DrawStringSmall
void func_ov091_021b9cb0(PokeListWork *wk, BmpWin *window, u32 msgId, int x, s16 y, u16 color);
// pokebw2's PokeList_PrintWordSetString
void func_ov091_021b9cf4(PokeListWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, s16 x, s16 y,
                                 u16 color);
// pokebw2's PokeList_PrintWordSetStringSmall
void func_ov091_021b9d58(PokeListWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, s16 x, s16 y,
                                      u16 color);
// pokebw2's PokeList_DrawWordSetStringSmall
void func_ov091_021b9dc0(PokeListWork *wk, BmpWin *window, WordSet *wordSet, u32 msgId, s16 x, s16 y,
                                     u16 color);
// pokebw2's PokeList_MessageDoneSelect
void func_ov091_021b9e20(PokeListWork *wk);
// pokebw2's PokeList_MessageDoneItem
void func_ov091_021b9e44(PokeListWork *wk);
void func_ov091_021b9ec0(PokeListWork *wk, u32 item);
void func_ov091_021b709c(PokeListWork *wk);
void func_ov091_021ba004(PokeListWork *wk);
void func_ov091_021ba318(PokeListWork *wk);
void func_ov091_021ba0a8(PokeListWork *wk, u32 item);
void func_ov091_021ba3a4(PokeListWork *wk, u32 item);
void func_ov091_021ba454(PokeListWork *wk, u32 item);
// pokebw2's PokeList_AskStopLearning
void func_ov091_021b9eb0(PokeListWork *wk);
// pokebw2's PokeList_TimeUp
void func_ov091_021ba4f8(PokeListWork *wk);

// plist_plate.c
PokeListPlate *func_ov091_021ba6a0(PokeListWork *wk, u8 index, PartyPkm *pkm);
PokeListPlate *func_ov091_021ba81c(PokeListWork *wk, u8 index);
int func_ov091_021bb43c(PokeListPlate *plate);
BOOL func_ov091_021bb3c4(PokeListWork *wk, PokeListPlate *plate);
BOOL func_ov091_021bb468(PokeListWork *wk, PokeListPlate *plate);
u32 func_ov091_021bc228(PokeListWork *wk, u16 item);
BOOL func_ov112_021f6744(PartyPkm *pkm);
void func_ov091_021ba890(PokeListWork *wk, PokeListPlate *plate);
void func_ov091_021bb0c4(PokeListWork *wk, PokeListPlate *plate, int step);
void func_ov091_021bafa0(PokeListWork *wk, PokeListPlate *plate, int step);
void func_ov091_021b9968(PokeListWork *wk);
void func_ov091_021b9750(PokeListWork *wk);
void func_ov091_021b97d4(PokeListWork *wk);
// plist_plate.c
void func_ov091_021ba8cc(PokeListWork *wk, PokeListPlate *plate);
u32 func_ov091_021bb4a0(PokeListWork *wk, PokeListPlate *plate);
void func_ov091_021bb30c(PokeListWork *wk, PokeListPlate *plate);
BOOL func_ov091_021bb358(PokeListWork *wk, PokeListPlate *plate);
// plist_demo.c
void func_ov091_021bc6cc(PokeListWork *wk);
void func_ov091_021bc30c(PokeListWork *wk);
int func_ov091_021bc1d0(PokeListWork *wk);
void func_ov091_021bc2c8(PokeListWork *wk);
int func_ov091_021bc350(PokeListWork *wk, u32 move);
BOOL func_ov091_021bdcc0(PokeListWork *wk, PartyPkm *pkm);
void func_ov091_021bdd58(PokeListWork *wk, PartyPkm *pkm);
void func_ov091_021bd8b0(PokeListWork *wk);
void func_ov091_021bd8e4(PokeListWork *wk);
void func_ov091_021bd9c8(PokeListWork *wk);
void func_ov091_021bcde8(PokeListWork *wk);
void func_ov091_021bcccc(PokeListWork *wk);
void func_ov091_021bcd80(PokeListWork *wk);
void func_ov091_021bd4ec(PokeListWork *wk);
void func_ov091_021bd480(PokeListWork *wk, PokeListPartnerPkm *partnerPkm);
// plist_battle.c
void func_ov091_021bd18c(PokeListWork *wk, PokeListPartner *partner);
void func_ov091_021bd1bc(PokeListWork *wk, PokeListPartner *partner);
PokeListPartnerPkm *func_ov091_021bd218(PokeListWork *wk, PartyPkm *pkm, u8 x, u8 y);

#endif // BWPORT_APP_POKELIST_H
