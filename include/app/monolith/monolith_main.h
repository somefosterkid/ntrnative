// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/monolith/monolith_main.h and
// include/app/monolith.h, the lines in use so far.
#ifndef BWPORT_APP_MONOLITH_MONOLITH_MAIN_H
#define BWPORT_APP_MONOLITH_MONOLITH_MAIN_H

#include "types.h"
#include "field/intrude_comm.h"
#include "gfl/clact.h"
#include "struct_decls.h"
#include "system/app_printsys_common.h"
#include "system/printsys.h"

// The Entralink monolith (overlay 71): the work its screens share, and the param of each screen's procs. The layouts
// are Black's, cut short where nothing in C reads further

// The number of missions the monolith offers
#define MONOLITH_MISSION_COUNT 6

// The monolith's param, cut short
typedef struct {
    GameSystem *gsys;
    // The mission's player, copied from the communication's work (its player is the one a mission's text names)
    IntrudeCommPlayer commPlayer;
    // The Entralink shown: the mission player's (the communication's copy) or the player's own (func_ov011_02175648's).
    // Its unk_04 and unk_08 are the seconds played, a u64's low and high words
    IntrudeHighLinkData highLink;
    // The net ID of the player whose Entralink it is (func_ov010_0215841c's), 0 for the player's own
    u8 netId;
    u8 unk_3D;
} MonolithParam;

// A pass power's entry in the table the monolith's work points to (12 bytes each, overlay 10's data): its level
// requirement, the number of the power's points, its name's and description's message ids. Fields by offset
typedef struct {
    // The level that unlocks it, against the player's level1
    u8 unk_00;
    u8 unk_01;
    u16 unk_02;
    // The messages in msgPowerNames and msgPowerInfo
    u16 unk_04;
    u16 unk_06;
    u8 unk_08[4];
} MonolithPassPowerData;

// The monolith's work, cut short
// The actors' resources on each screen
struct MonolithActorRes {
    u32 palette;
    u32 chars;
    u32 cellAnims;
    u32 fontPalette;
};

struct MonolithWork {
    PaletteFade *fade;
    u32 unk_04;
    PrintQueue *printQueue;
    Font *font;
    WordSet *wordSet;
    ClActUnit *clactUnit;
    TCBExManager *tcbMgr;
    // The missions' text
    MsgData *msgMission;
    // The pass powers' names and descriptions (pokebw2's names)
    MsgData *msgPowerNames;
    MsgData *msgPowerInfo;
    // The monolith's own text
    MsgData *msgMonolith;
    BmpOamSys *bmpOam;
    ArcTool *arc;
    const MonolithPassPowerData *unk_34;
    AppPrintsysCommon printsys;
    // The actors' resources, the second's for the return button
    MonolithActorRes actorRes[2];
    IntrudeMission missions[MONOLITH_MISSION_COUNT];
};

// What the screens hand to each other, cut short
typedef struct {
    // The mission shown, an index of data_ov071_021baa4c or 6 for the one in progress
    u8 mission;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} MonolithState;

// The param of every screen's procs, cut short
typedef struct {
    MonolithParam *param;
    MonolithWork *work;
    MonolithState *state;
    // The palette effects of the screens' panels
    u8 unk_0C[0x20];
    // The screen to show next
    u8 next;
    // Asks the top screen's proc to end
    u8 endTop;
    u8 exiting;
} MonolithScreenParam;

// The order of the monolith's missions for each index of MonolithState's mission
extern const u32 data_ov071_021baa4c[MONOLITH_MISSION_COUNT];

// The procs of the monolith's screens (pokebw2's MONOLITH_*_PROC_FUNCTIONS), each a top-screen or bottom-screen proc
extern const GameProcFunctions data_ov071_021ba8d0;
extern const GameProcFunctions data_ov071_021ba91c;
extern const GameProcFunctions data_ov071_021baa34;
extern const GameProcFunctions data_ov071_021baa40;
extern const GameProcFunctions data_ov071_021baaac;
extern const GameProcFunctions data_ov071_021bab04;
extern const GameProcFunctions data_ov071_021bab88;

// Black 2's, not checked against Black
// The screens, each a proc on the top screen and one on the bottom screen
enum {
    MONOLITH_SCREEN_MENU,
    MONOLITH_SCREEN_PASS_POWER,
    MONOLITH_SCREEN_RECORDS,
    MONOLITH_SCREEN_MISSION,
    MONOLITH_SCREEN_COUNT,
    // Asked for by a screen to close the monolith
    MONOLITH_SCREEN_EXIT = MONOLITH_SCREEN_COUNT,
};

// Black 2's, not checked against Black
// The menu's items, where MonolithState's menuCursor returns
enum {
    MONOLITH_MENU_MISSION,
    MONOLITH_MENU_PASS_POWER,
    MONOLITH_MENU_RECORDS,
};

// The palette effects of the screens' panels, one per PaletteFade buffer (monolith_tool.c)
#define PANEL_CONTROL_MAX 4

// Black 2's; twinned code confirms the offsets of mode in Black
struct MonolithPanelControl {
    // The weight of the blend, in 1/256
    u16 fraction;
    // PANEL_MODE_*
    u8 mode;
    // Whether the pulse is fading back
    u8 falling;
    // The frames until the flash's next step
    u8 wait;
    u8 flashes;
    u8 pad[2];
};

#endif // BWPORT_APP_MONOLITH_MONOLITH_MAIN_H
