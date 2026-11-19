#include "global.h"

#include "constants/heap.h"

#include "bg_window.h"
#include "error_handling.h"
#include "filesystem.h"
#include "options.h"
#include "overlay_manager.h"
#include "palette.h"
#include "player_data.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "save.h"
#include "screen_fade.h"
#include "sprite.h"
#include "text.h"
#include "unk_02005D10.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_41_sprite_resource_collection.h"
#include "pokemon_summary_app.h"
#include "msgdata.h"
#include "unk_02088288.h"
#include "struct_defs/resdat_narc_structs.h"

typedef struct MessageBankEntry MessageBankEntry;

typedef struct MessageBank MessageBank;

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenuItem ListMenuItem;

typedef struct ListMenu2DTemplate ListMenu2DTemplate;

typedef struct ListMenu2D ListMenu2D;

typedef struct StringTemplateArgHeader StringTemplateArgHeader;

typedef struct StringTemplateArg StringTemplateArg;

typedef struct ChatotCry ChatotCry;

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

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

typedef u32 TextColor;

struct ListMenuCursor {
    TextColor color;
    String *string;
};

struct ListMenuItem {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
};

struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
};

struct ListMenu2D {
    ListMenu2DTemplate template;
    ListMenuCursor *cursor;
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
    OverlayManager *unused;
    OverlayManager *monSummaryAppMan;
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
    MessageFormat *strTemplate;
    String *displayStr;
    String *fmtStr;
    String *yesNoStrs[2];
    u16 unused3[8];
    BgConfig *bgConfig;
    Window windows[4];
    ListMenu2DTemplate yesNoMenuTemplate;
    ListMenu2D *yesNoMenu;
    ListMenuItem yesNoStrList[2];
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

u8 ov80_02237920(u8 param0);

void ov82_0223E8C4(void *arg0);

void ov82_0223F224(void *arg0);

s32 ov82_0223F53C(void *arg0);

void ov82_0223F580(BattleHallApp *app, BgConfig *bgConfig);

void ov82_0223F834(void *arg0);

void ov82_0223F84C(void *arg0);

void ov82_0223F8E4(void *arg0);

void ov82_0223F90C(void *arg0);

void ov82_0223F948(s32 arg0);

s32 ov80_0223792C(u8);

u8 ov82_0223EF1C(BattleHallApp *, Window *, u32, u32, u32, u32, u8, u8, u8, u8);

u8 ov82_0223EF7C(BattleHallApp *, u32, u8);

void ov82_0223F300(BattleHallApp *, u32, s32);

s32 ov82_0223F488(BattleHallApp *);

u16 ov82_0223F558(BattleHallApp *);

u16 ov82_0223F570(BattleHallApp *);

void ov82_0223F5E0(BgConfig *, u8, u8);

u8 ov82_0223F6C4(u8 param0);

s32 ov82_0223F6CC(BattleHallApp *);

s32 ov82_0223F6E4(BattleHallApp *);

void ov82_0223FCBC(SpriteResource *, u32, u32);

void ov82_0223FCFC(SpriteResource *, u32);

void ov82_0223FD78(Window *, u32);

s32 ov82_0223FE18(ListMenuItem *);

u8 sub_02030BD0(u32, SpriteResource *);

u32 ov82_0223E2EC(BattleHallApp *param_1, s32 arg1, s32 arg2);

u32 ov82_0223E2EC(BattleHallApp *param_1, s32 arg1, s32 arg2) {
    u8 uVar1;
    s32 sVar2;
    u32 uVar3;
    s32 iVar4;
    u32 uVar5;
    Options *options;
    u32 uVar6;
    u32 uVar7;
    BOOL BVar8;
    OverlayManager *pAVar9;

    switch (param_1->subState) {
    case 0:
        param_1->summaryScreenOpened = 0;
        param_1->subState = 1;
        break;
    case 1:
        ov82_0223F300(param_1, ((u32 *)&gSystem)[0x12], arg2);
        sVar2 = ov82_0223F53C(param_1);
        if ((sVar2 != 0) || (iVar4 = ov82_0223F488(param_1), iVar4 != 0)) {
            uVar5 = ov80_02237920((u32)param_1->cursorPos);
            if (uVar5 == 0xfe) {
                PlaySE(0x5dd);
                BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, 0, 6, 1, HEAP_ID_105);
                param_1->summaryScreenOpen = 1;
                param_1->subState = 7;
            } else {
                if (ov82_0223F6E4(param_1) == 1) {
                    uVar5 = ov80_02237920((u32)param_1->cursorPos);
                    if (uVar5 != 9) {
                        PlaySE(0x5f3);
                        return 0;
                    }
                } else {
                    uVar6 = ov82_0223F6C4(param_1->cursorPos);
                    uVar3 = sub_02030BD0(uVar6, (param_1->spriteMan).resources[1][3]);
                    if (uVar3 >= 0xA) {
                        PlaySE(0x5f3);
                        return 0;
                    }
                    uVar5 = ov80_02237920((u32)param_1->cursorPos);
                    if (uVar5 == 9) {
                        PlaySE(0x5f3);
                        return 0;
                    }
                }
                PlaySE(0x5dd);
                iVar4 = ov80_0223792C(param_1->challengeType);
                if (iVar4 == 0) {
                    ov82_0223F5E0(param_1->bgConfig, param_1->cursorPos, 1);
                    ScheduleBgTilemapBufferTransfer(param_1->bgConfig, 3);
                }
                param_1->openMenuDelay = 0;
                param_1->subState = 2;
            }
        }
        break;
    case 2:
        param_1->openMenuDelay = (s16)param_1->openMenuDelay + 1;
        ov82_0223F948(-(s32)(s16)param_1->openMenuDelay);
        if ((s16)param_1->openMenuDelay == 8) {
            iVar4 = ov80_0223792C(param_1->challengeType);
            if (iVar4 == 0) {
                ov82_0223F84C(param_1);
                param_1->subState = 3;
            } else {
                uVar5 = Options_GetFrame(param_1->yesNoStrList[0].entry);
                ov82_0223FD78(param_1->windows, uVar5);
                uVar1 = ov82_0223EF7C(param_1, 0, 1);
                param_1->printerID = uVar1;
                param_1->selectedCursorPos = param_1->cursorPos;
                param_1->startMultiSelectionConfirm = 1;
                return 1;
            }
        }
        break;
    case 3:
        iVar4 = ov82_0223FE18((param_1->yesNoMenuTemplate).choices);
        if (iVar4 != 0) {
            switch (iVar4) {
            case 1:
                ov82_0223F834(param_1);
                iVar4 = ov82_0223F6CC(param_1);
                if (iVar4 == 1) {
                    iVar4 = ov82_0223F6E4(param_1);
                    if (iVar4 == 0) {
                        param_1->savedCursorPos = param_1->cursorPos;
                        param_1->subState = 5;
                        break;
                    }
                }
                StopSE(0x5dc, 0);
                PlaySE(0x623);
                if (param_1->savedCursorPos != 0x75) {
                    param_1->cursorPos = param_1->savedCursorPos;
                }
                return 1;
            case 2:
                ov82_0223F834(param_1);
                param_1->subState = 4;
                break;
            }
        }
        break;
    case 4:
        param_1->openMenuDelay = (s16)param_1->openMenuDelay - 1;
        ov82_0223F948(-(s32)(s16)param_1->openMenuDelay);
        if ((s16)param_1->openMenuDelay <= 0) {
            ov82_0223F8E4(param_1);
            if (param_1->matronBattleAvailable == 1) {
                param_1->subState = 0;
            } else if (param_1->matronBattleAvailable == 0) {
                param_1->subState = 1;
            } else {
                GF_AssertFail();
            }
        }
        break;
    case 5:
        uVar5 = Options_GetFrame(param_1->yesNoStrList[0].entry);
        ov82_0223FD78(param_1->windows, uVar5);
        options = Save_PlayerData_GetOptionsAddr(param_1->yesNoStrList[0].callbackFunction);
        uVar1 = Options_GetTextFrameDelay(options);
        uVar1 = ov82_0223EF1C(param_1, param_1->windows, 0x20, 1, 1, uVar1, 1, 2, 0xf, 1);
        param_1->printerID = uVar1;
        CopyWindowToVram(param_1->windows);
        param_1->subState = 6;
        break;
    case 6:
        uVar1 = TextPrinterCheckActive(param_1->printerID);
        if (uVar1 == 0) {
            param_1->matronBattleAvailable = 1;
            ov82_0223F90C(param_1);
            ov82_0223FCFC((param_1->spriteMan).resources[0][2], 1);
            ov82_0223F5E0(param_1->bgConfig, 0x13, 0);
            ov82_0223F580(param_1, param_1->bgConfig);
            param_1->cursorPos = 0x13;
            uVar6 = ov82_0223F558(param_1);
            uVar7 = ov82_0223F570(param_1);
            ov82_0223FCBC((param_1->spriteMan).resources[0][2], uVar6, uVar7);
            param_1->subState = 4;
        }
        break;
    case 7:
        BVar8 = IsPaletteFadeFinished();
        if (BVar8 == 1) {
            ov82_0223F224(param_1);
            ov82_0223E8C4(param_1);
            pAVar9 = (OverlayManager *)
                OverlayManager_New(&gOverlayTemplate_PokemonSummary,
                    param_1->yesNoStrList[1].entry,
                    HEAP_ID_105);
            param_1->monSummaryAppMan = pAVar9;
            param_1->summaryScreenOpened = 1;
            return 1;
        }
    }
    return 0;
}
