#ifndef POKEHEARTGOLD_OVERLAY_43_PALPAD_H
#define POKEHEARTGOLD_OVERLAY_43_PALPAD_H

// PalPadWork (hk290): the Pal Pad app's OverlayManager data (PalPad_Init: 0x5A4 bytes), and its seven sub-objects, each
// built by its own ov43 constructor: gfx (0x4, ov43_0222A48C), shared (0x214, ov43_0222A290), menu (0x280, ov43_0222AE5C),
// roster (0x2C0, ov43_0222B5A8), naming (0x3C0, ov43_0222CBAC), friendCode (0x3CC, ov43_0222CD90), details (0x408, ov43_0222D134).
// Platinum's twin is overlay064 (UnkStruct_ov64_0222DFD0).

#include "bg_window.h"
#include "g2d_renderer.h"
#include "sprite_resources_header.h"

struct BgConfig;
struct CharSprite;
struct CharSpriteManager;
struct CodeEntryArgs;
struct GF_2DGfxResMan;
struct ListMenuItem;
struct MessageFormat;
struct MsgData;
struct NARC;
struct NamingScreenArgs;
struct OverlayManager;
struct PalPadArgs;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct String;
struct TouchscreenListMenu;
struct TouchscreenListMenuSpawner;
struct YesNoPrompt;


enum PalPadScreen {
    PALPAD_SCREEN_MENU,
    PALPAD_SCREEN_ROSTER,
    PALPAD_SCREEN_NAMING,
    PALPAD_SCREEN_FRIEND_CODE,
    PALPAD_SCREEN_DETAILS,
    PALPAD_SCREEN_EXIT,
};

typedef struct PalPadScreens {
    void *files[2];
    NNSG2dScreenData *data[2];
} PalPadScreens;

typedef struct PalPadGfx {
    struct BgConfig *bgConfig;
    struct SpriteList *spriteList;
    NNSG2dRenderSurface renderSurface;
    struct MessageFormat *msgFormat;
    struct MsgData *msgData;
    struct NARC *narc;
    struct TouchscreenListMenuSpawner *listMenuSpawner;
    struct YesNoPrompt *yesNoPrompt;
    Window titleWindow;
    struct String *titleText;
    struct String *titleBuffer;
    struct String *pageText;
    struct String *pageBuffer;
    u32 titlePrinterId;
    SpriteResourcesHeader spriteHeader;
    struct SpriteResource *spriteResources[4];
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resourceManagers[4];
    struct Sprite *arrows[2];
    struct Sprite *cursor;
    PalPadScreens screens;
} PalPadGfx;

typedef struct PalPadFriendList {
    u32 count;
    int numberedCount;
    u8 slots[32];
    u8 numbers[32];
} PalPadFriendList;

typedef struct PalPadShared {
    u32 inputState;
    struct SaveData *saveData;
    u8 subState;
    u8 nextScreen;
    u8 nextSubState;
    s8 selectedFriend;
    u8 textSpeed;
    u8 frame;
    u8 lrPaging;
    PalPadFriendList friends;
    struct String *name;
    struct String *code;
    const u16 *initialName;
    BOOL useInitialName;
} PalPadShared;

typedef struct PalPadMenu {
    u32 selection;
    u32 timer;
    Window menuWindow;
    void *screenFile;
    NNSG2dScreenData *screen;
    Window msgWindow;
    u32 unused30;
    u32 printerId;
    u32 textSpeed;
    struct String *msgText;
} PalPadMenu;

typedef struct PalPadPanelLayout {
    s16 x;
    s16 y;
    u8 width;
    u8 height;
} PalPadPanelLayout;

typedef struct PalPadWindowSpec {
    u8 x;
    u8 y;
    u8 width;
    u8 height;
} PalPadWindowSpec;

typedef struct PalPadRosterPage {
    PalPadPanelLayout layout;
    Window window;
    struct CharSprite *icons[8];
} PalPadRosterPage;

typedef struct PalPadRoster {
    s16 page;
    s16 prevPage;
    s16 cursor;
    s16 prevCursor;
    PalPadRosterPage pages[3];
    s16 scrollStep;
    u16 scrollDir;
    struct CharSpriteManager *trainerIcons;
    Window returnWindow;
    Window msgWindow;
    struct String *msgText;
    u32 printerId;
    u32 textSpeed;
    struct ListMenuItem *menuItems[2];
    struct TouchscreenListMenu *menu;
    u8 unk_F0[4];
    int timer;
    struct Sprite *deleteEffect;
    u32 lastFrame;
} PalPadRoster;

typedef struct PalPadNaming {
    struct OverlayManager *manager;
    struct NamingScreenArgs *namingArgs;
    struct CodeEntryArgs *codeEntryArgs;
} PalPadNaming;

typedef struct PalPadFriendCode {
    Window windows[3];
    void *screenFile;
    NNSG2dScreenData *screen;
    int timer;
} PalPadFriendCode;

typedef struct PalPadDetailsSlot {
    PalPadPanelLayout layout;
    u32 baseTile;
    Window *windows[8];
    u32 windowCounts[8];
} PalPadDetailsSlot;

typedef struct PalPadDetailsScreen {
    void *file;
    NNSG2dScreenData *data;
} PalPadDetailsScreen;

typedef struct PalPadDetails {
    s16 page;
    s16 prevPage;
    int scrollStep;
    int direction;
    u16 friendChangeDelay;
    u16 friendChangeHeld;
    BOOL hasFrontierPages;
    PalPadDetailsSlot slots[3];
    PalPadDetailsScreen screens[4];
    Window profileWindows[8];
} PalPadDetails;

typedef struct PalPadWork {
    u32 screen;
    PalPadGfx gfx;
    PalPadShared shared;
    struct PalPadArgs *args;
    PalPadMenu menu;
    PalPadRoster roster;
    PalPadNaming naming;
    PalPadFriendCode friendCode;
    PalPadDetails details;
    s32 screenChangeDelay;
} PalPadWork;

// sizeof(PalPadGfx) == 0x210, sizeof(PalPadShared) == 0x68, sizeof(PalPadMenu) == 0x40, sizeof(PalPadRoster) == 0x100,
// sizeof(PalPadNaming) == 0xC, sizeof(PalPadFriendCode) == 0x3C, sizeof(PalPadDetails) == 0x198, sizeof(PalPadWork) == 0x5A4

#endif // POKEHEARTGOLD_OVERLAY_43_PALPAD_H
