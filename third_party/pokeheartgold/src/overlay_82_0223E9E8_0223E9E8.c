#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "font.h"
#include "gf_gfx_planes.h"
#include "message_format.h"
#include "message_printer.h"
#include "msgdata.h"
#include "options.h"
#include "palette.h"
#include "party.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "sprite.h"
#include "system.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "unk_02035900.h"
#include "unk_0203A3B0.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;


typedef struct ApplicationManager ApplicationManager;

typedef struct ApplicationManagerTemplate ApplicationManagerTemplate;

typedef struct ColoredArrow ColoredArrow;

typedef struct StringList StringList;

typedef struct MenuTemplate MenuTemplate;

typedef struct Menu Menu;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct StringTemplate StringTemplate;

typedef struct Resource Resource;

typedef struct ResourceCollection ResourceCollection;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct ChatotCry ChatotCry;

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

typedef struct PokemonSummary PokemonSummary;

typedef struct BattleHallAppSpriteManager BattleHallAppSpriteManager;

typedef struct BattleHallAppSprite BattleHallAppSprite;

typedef struct BattleHallApp BattleHallApp;

typedef u16 charcode_t;

struct MessageBankEntry {
    u32 offset;
    u32 length;
};

struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
};


typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
};

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

typedef u32 TextColor;

struct ColoredArrow {
    TextColor color;
    String *string;
};

struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
};

struct MenuTemplate {
    StringList *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
};

struct Menu {
    MenuTemplate template;
    ColoredArrow *cursor;
    u32 cancelKeys;
    u8 dummy14;
    u8 cursorPos;
    u8 width;
    u8 xOffset;
    u8 yOffset;
    u8 letterWidth;
    u8 lineHeight;
    u8 lastAction;
    u8 heapID;
};

struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
};

struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
};

struct StringTemplate {
    u32 maxArgs;
    enum HeapID heapID;
    StringTemplateArg *args;
    String *templateBuf;
};

struct Resource {
    int id;
    void *data;
};

struct ResourceCollection {
    Resource *resources;
    int capacity;
    int count;
};

enum SpriteResourceType {
    SPRITE_RESOURCE_CHAR = 0,
    SPRITE_RESOURCE_PLTT,
    SPRITE_RESOURCE_CELL,
    SPRITE_RESOURCE_ANIM,
    SPRITE_RESOURCE_MULTI_CELL,
    SPRITE_RESOURCE_MULTI_ANIM,
    SPRITE_RESOURCE_MAX,
};

struct SpriteResourceCollection {
    ResourceCollection *collection;
    SpriteResource *resources;
    void *capacity;
    void *count;
    enum SpriteResourceType type;
};

struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
};

struct FontSpecialCharsContext {
    void *ncgrBuffer;
    NNSG2dCharacterData *charData;
    charcode_t charcodes[16];
    u32 bgColor;
};

struct PokemonSummary;

struct BattleHallAppSpriteManager {
    SpriteList *spriteList;
    G2dRenderer renderer;
    SpriteResourceCollection *resourceCollection[4];
    SpriteResource *resources[2][4];
};

struct BattleHallAppSprite {
    u32 unused[3];
    const u8 *isAlwaysNull;
    Sprite *sprite;
};

struct BattleHallApp {
    ApplicationManager *unused;
    ApplicationManager *monSummaryAppMan;
    u8 subState;
    u8 challengeType;
    u8 printerID;
    u8 summaryScreenOpened;
    u8 prevCursorPos;
    u8 cursorPos;
    u8 unused2;
    u8 yesNoMenuOpen;
    u16 summaryScreenOpen;
    u16 isSynced;
    u16 openMenuDelay;
    u8 numSelectionMsgsReceived;
    u8 startMultiSelectionConfirm;
    u8 selectionID;
    u8 selectionCanceled;
    u8 selectedCursorPos;
    u8 fadeOutDelay;
    u16 currentStreak;
    u8 matronBattleAvailable;
    u8 savedCursorPos;
    MsgData *msgLoader;
    StringTemplate *strTemplate;
    String *displayStr;
    String *fmtStr;
    String *yesNoStrs[2];
    u16 unused3[8];
    BgConfig *bgConfig;
    Window windows[4];
    MenuTemplate yesNoMenuTemplate;
    Menu *yesNoMenu;
    StringList yesNoStrList[2];
    PaletteData *plttData;
    FontSpecialCharsContext *specialChars;
    Options *options;
    SaveData *saveData;
    PokemonSummary *monSummary;
    BattleHallAppSpriteManager spriteMan;
    BattleHallAppSprite *cursorSprite;
    BattleHallAppSprite *monSprite;
    int monIndex;
    u16 *selectedCellPtr;
    Party *party;
    u8 *typeRanks;
    u16 *partnersLevel;
    NARC *narc;
    u16 commPayload[44];
    u8 partnersSelectedCell;
    u8 partnersDecision;
    u8 unused4;
    u32 unused5;
};

void ov82_0223EB3C(void *arg0);

void ov82_0223EB9C(void *arg0);

void ov82_0223F580(BattleHallApp *app, BgConfig *bgConfig);

BattleHallAppSprite *ov82_0223FC48(BattleHallAppSpriteManager *spriteMan, u32 resourceID, u32 animID, u16 x, u16 y, const u8 *null);

void ov82_0223FD2C(BgConfig *bgConfig, Window *windows);

void ov82_0223EC0C(void *);

u16 ov82_0223F558(BattleHallApp *arg0);

u16 ov82_0223F570(BattleHallApp *);

s32 ov82_0223FD18(SpriteResource *, Pokemon *);

StringList *ov82_0223FDB8(s32);

void ov82_0223E9E8(BattleHallApp *arg0);

void ov82_0223E9E8(BattleHallApp *arg0) {
    BattleHallApp *var_r4;
    s32 var_r6;
    u16 temp_r4;

    arg0->monSprite = (struct BattleHallAppSprite *)(NARC_New(NARC_a_1_8_3, HEAP_ID_105));
    ov82_0223EB3C(arg0);
    ov82_0223EB9C(arg0);
    arg0->msgLoader = (struct MsgData *)(NewMsgDataFromNarc(MSGDATA_LOAD_LAZY, NARC_msgdata_msg, 0x1B9, HEAP_ID_105));
    arg0->strTemplate = (struct StringTemplate *)(MessageFormat_New(HEAP_ID_105));
    arg0->displayStr = String_New(0x258U, HEAP_ID_105);
    arg0->fmtStr = String_New(0x258U, HEAP_ID_105);
    var_r6 = 0;
    var_r4 = arg0;
    do {
        var_r4->yesNoStrs[0] = String_New(0x20U, HEAP_ID_105);
        var_r4 = (void *)((u8 *)var_r4 + (4));
        var_r6 += 1;
    } while (var_r6 < 2);
    LoadFontPal0(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_13_OFFSET, HEAP_ID_105);
    LoadFontPal1(GF_PAL_LOCATION_MAIN_BG, GF_PAL_SLOT_12_OFFSET, HEAP_ID_105);
    LoadFontPal0(GF_PAL_LOCATION_SUB_BG, GF_PAL_SLOT_2_OFFSET, HEAP_ID_105);
    arg0->yesNoMenu = (struct Menu *)(MessagePrinter_New(0xFU, 0xEU, 0U, HEAP_ID_105));
    ov82_0223FD2C(arg0->bgConfig, arg0->windows);
    ov82_0223F580(arg0, arg0->bgConfig);
    reg_GX_POWCNT &= 0xFFFF7FFF;
    GfGfx_BothDispOn();
    temp_r4 = ov82_0223F558(arg0);
    arg0->spriteMan.resources[0][2] = (SpriteResource *)ov82_0223FC48((BattleHallAppSpriteManager *)&arg0->yesNoStrList[1].index, 0U, 1U, temp_r4, (u16)ov82_0223F570(arg0), NULL);
    arg0->spriteMan.resources[0][3] = (SpriteResource *)ov82_0223FC48((BattleHallAppSpriteManager *)&arg0->yesNoStrList[1].index, 1U, 1U, 0xA0U, 0xA0U, NULL);
    ov82_0223FD18(arg0->spriteMan.resources[0][3], Party_GetMonByIndex((Party *)arg0->spriteMan.resources[1][2], 0));
    if (sub_02037474() != 0) {
        G2dRenderer_SetObjCharTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN, GX_OBJVRAMMODE_CHAR_1D_32K);
        G2dRenderer_SetPlttTransferReservedRegion(NNS_G2D_VRAM_TYPE_2DMAIN);
        sub_0203A880();
    }
    arg0->yesNoMenuTemplate.choices = ov82_0223FDB8(0x69);
    Main_SetVBlankIntrCB(ov82_0223EC0C, arg0);
}
