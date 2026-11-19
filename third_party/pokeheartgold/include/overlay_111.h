#ifndef POKEHEARTGOLD_OVY_111_H
#define POKEHEARTGOLD_OVY_111_H

#include "bg_window.h"
#include "options.h"
#include "overlay_manager.h"
#include "pokemon_types_def.h"

typedef struct BugContestSwapMonArgs {
    Pokemon *newlyCaughtMon;
    Pokemon *currentMon;
    BOOL noPokemonCaught;
    Options *options;
    Pokemon *unk10;
} BugContestSwapMonArgs;

struct BgConfig;
struct ManagedSprite;
struct MessageFormat;
struct MessagePrinter;
struct MsgData;
struct NARC;
struct SpriteManager;
struct SpriteSystem;
struct SwapMonPanel;
struct SwapMonPanels;
struct SwapMonText;
struct String;
struct YesNoPrompt;

typedef struct SwapMonPanel {
    Pokemon *mon;
    struct ManagedSprite *sprites[2]; // 0: panel background, 1: mon icon
    Window windows[6]; // name, gender, level, HP, max HP, HP bar
    s32 sliding;
    s16 targetY;
    s16 x;
    s16 y;
} SwapMonPanel;

// sizeof(SwapMonPanel) == 0x78 on the DS

typedef struct SwapMonPanelWindowTemplate {
    s16 x;
    s16 y;
    u8 width;
    u8 height;
    u8 paletteNum;
    u16 baseTile;
} SwapMonPanelWindowTemplate;

typedef struct SwapMonText {
    struct BgConfig *bgConfig;
    struct MessagePrinter *msgPrinter;
    struct MessageFormat *msgFormat;
    struct MsgData *msgData;
    struct String *string;
    Options *options;
    enum HeapID heapID;
    Window window;
    int printerId;
    u32 timer : 16;
    u32 bgLayer : 3;
    u32 skipDelay : 1;
    u32 unused : 12;
} SwapMonText;

// sizeof(SwapMonText) == 0x34 on the DS

typedef struct SwapMonPanels {
    enum HeapID heapID;
    struct BgConfig *bgConfig;
    SwapMonText *text;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    s8 iconPalette;
    SwapMonPanel *panels[2];
} SwapMonPanels;

typedef struct BugContestSwapMon {
    enum HeapID heapID;
    BugContestSwapMonArgs *args;
    struct BgConfig *bgConfig;
    struct SpriteSystem *spriteSystem;
    struct SpriteManager *spriteManager;
    struct ManagedSprite *cursorSprite;
    struct YesNoPrompt *yesNoPrompt;
    struct NARC *narc;
    SwapMonPanels *panels;
    SwapMonText *text;
    s8 iconPalette;
    int state;
    int substate;
} BugContestSwapMon;

BOOL BugContestSwapMon_Init(OverlayManager *man, int *state);
BOOL BugContestSwapMon_Main(OverlayManager *man, int *state);
BOOL BugContestSwapMon_Exit(OverlayManager *man, int *state);

SwapMonPanels *ov111_021E66DC(enum HeapID heapID, struct BgConfig *bgConfig, SwapMonText *text, struct SpriteSystem *spriteSystem, struct SpriteManager *spriteManager, s8 iconPalette);
void ov111_021E6710(SwapMonPanels *panels);
void ov111_021E6738(SwapMonPanels *panels, s32 idx, u8 x, u8 y, Pokemon *mon);
void ov111_021E6770(SwapMonPanels *panels, s32 idx, s32 sliding, s32 targetY);
void ov111_021E6784(SwapMonPanels *panels);
s32 ov111_021E67A4(SwapMonPanels *panels);

void ov111_021E6268(struct ManagedSprite *sprite, Pokemon *mon, s32 paletteOffset, enum HeapID heapID);
SwapMonPanel *ov111_021E63D0(SwapMonPanels *panels, u8 x, u8 y, s32 idx, Pokemon *mon);
void ov111_021E64C8(SwapMonPanel *panel, SwapMonText *text);
void ov111_021E65CC(SwapMonPanel *panel, s32 sliding, s32 targetY);
void ov111_021E65D4(SwapMonPanel *panel);
s32 ov111_021E6684(SwapMonPanel *panel);
void ov111_021E6694(SwapMonPanel *panel);

SwapMonText *ov111_021E67C4(enum HeapID heapID);
void ov111_021E67EC(SwapMonText *text, struct BgConfig *bgConfig, s32 bgLayer, Options *options);
void ov111_021E685C(SwapMonText *text);
BOOL ov111_021E6888(SwapMonText *text);
void ov111_021E68FC(SwapMonText *text, s32 msgId);
void ov111_021E6934(SwapMonText *text, Window *window, u32 msgId, u8 x, u8 y);
void ov111_021E696C(SwapMonText *text, Window *window, u32 msgId, u8 x, u8 y, u32 color);
void ov111_021E69A0(SwapMonText *text, Window *window, u32 num, u32 digits, s32 useStatFont, s32 unk);
void ov111_021E69F4(SwapMonText *text);
struct MessageFormat *ov111_021E6A2C(SwapMonText *text);
void ov111_021E6A44(struct BgConfig *bgConfig, u8 layer, enum HeapID heapID);
u8 ov111_021E6A74(struct BgConfig *bgConfig, Window *window, struct String **string, struct MessageFormat *msgFormat, struct MsgData *msgData, u32 msgId, u8 bgLayer, enum HeapID heapID, Options *options);
void ov111_021E6B30(struct BgConfig *bgConfig, Window *window, struct MessageFormat *msgFormat, struct MsgData *msgData, u32 msgId, u8 bgLayer, u8 x, u8 y, u32 color, enum HeapID heapID);

#endif // POKEHEARTGOLD_OVY_111_H
