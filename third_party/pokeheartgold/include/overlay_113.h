#ifndef POKEHEARTGOLD_OVY_113_H
#define POKEHEARTGOLD_OVY_113_H

#include "bg_window.h"
#include "menu_input_state.h"
#include "overlay_manager.h"
#include "save.h"

struct ManagedSprite;
struct MessageFormat;
struct MsgData;
struct NARC;
struct NNSG2dScreenData;
struct Pokedex;
struct Sprite;
struct SpriteManager;
struct SpriteSystem;
struct String;
struct YesNoPrompt;

typedef struct UnownReportArgs {
    MenuInputStateMgr *menuInputStatePtr;
    SaveData *saveData;
} UnownReportArgs;

typedef struct UnownReportScreen {
    void *buf;
    struct NNSG2dScreenData *scrnData;
    void *tilemap;
    u8 width;
    u8 height;
} UnownReportScreen;

#define UNOWN_REPORT_MAX_FORMS 28
#define UNOWN_REPORT_N_WINDOWS 4
#define UNOWN_REPORT_N_MON_ICONS 14
#define UNOWN_REPORT_N_SCREENS 6

typedef struct UnownReportData {
    enum HeapID heapID;
    UnownReportArgs *args;
    u8 unk_08[0x4];
    struct Pokedex *pokedex;
    u16 state;
    u8 unk_12[0x2];
    enum MenuInputState menuInputState;
    u8 textFrameDelay;
    u8 frame;
    u8 page;
    u8 numPages;
    u8 numFormPages;
    u8 numExtraPages;
    u8 unk_1E;
    u8 numFormsSeen;
    u8 forms[UNOWN_REPORT_MAX_FORMS];
    u8 reportLevel;
    u8 unk_3D[0x3];
    BgConfig *bgConfig;
    struct MsgData *msgData;
    struct MessageFormat *msgFormat;
    struct String *strBuf1;
    struct String *strBuf2;
    struct String *titleString;
    struct String *pageFormatString;
    struct String *playerName;
    u8 unk_60[0x8];
    Window windows[UNOWN_REPORT_N_WINDOWS];
    struct YesNoPrompt *yesNoPrompt;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager1;
    struct SpriteManager *spriteManager2;
    struct Sprite *sprites[3];
    struct ManagedSprite *monIcons[UNOWN_REPORT_N_MON_ICONS];
    struct NARC *narc;
    UnownReportScreen screens[UNOWN_REPORT_N_SCREENS];
} UnownReportData;

BOOL UnownReport_Init(OverlayManager *man, int *state);
BOOL UnownReport_Main(OverlayManager *man, int *state);
BOOL UnownReport_Exit(OverlayManager *man, int *state);

#endif // POKEHEARTGOLD_OVY_113_H
