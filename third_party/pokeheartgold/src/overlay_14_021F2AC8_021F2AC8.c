#include "global.h"
#include "overlay_14_unk_struct_ov14_boxsys.h"
#include "pm_string.h"
#include "options.h"
#include "sprite.h"
#include "sprite_system.h"
#include "pokemon_summary_app.h"
#include "list_menu.h"
#include "overlay_manager.h"
#include "sav_chatot.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_14_box_funcs.h"

void ov14_021F2AC8(UnkStruct_ov14_boxsys *self);

struct OverlayManager;

struct BgConfig;

struct SOUND_CHATOT;

struct ListMenuCursor;

struct ListMenu;

struct ManagedSprite;

struct ListMenu2D;

struct MsgData;

struct MoveReminderController;

struct MoveReminderData;

struct Options;

struct SpriteManager;

struct SpriteSystem;

struct String;

struct ListMenuItem;

struct MessageFormat;

struct Window;

SpriteSystem *SpriteSystem_Alloc(enum HeapID heapID);

SpriteManager *SpriteManager_New(SpriteSystem *spriteSys);

BOOL SpriteSystem_Init(SpriteSystem *spriteSys, const OamManagerParam *oamTemplate, const OamCharTransferParam *transferTemplate, int plttCapacity);

BOOL SpriteSystem_InitSprites(SpriteSystem *spriteSys, SpriteManager *spriteMan, int maxSprites);

BOOL SpriteSystem_InitManagerWithCapacities(SpriteSystem *spriteSys, SpriteManager *spriteMan, const SpriteResourceCountsListUnion *capacities);

void GfGfx_EngineATogglePlanes(u8 layerMask, u8 enable);

typedef struct ListMenuCursor ListMenuCursor;

typedef struct ListMenu2D ListMenu2D;

typedef struct MoveReminderData MoveReminderData;

void GF_CreateVramTransferManager(u32 capacity, enum HeapID heapID);

enum {
    MOVE_REMINDER_WIN_LABEL_BATTLE_MOVES = 0,
    MOVE_REMINDER_WIN_LABEL_CONTEST_MOVES,
    MOVE_REMINDER_WIN_LABEL_CATEGORY,
    MOVE_REMINDER_WIN_LABEL_POWER,
    MOVE_REMINDER_WIN_LABEL_ACCURACY,
    MOVE_REMINDER_WIN_LABEL_PP,
    MOVE_REMINDER_WIN_LABEL_APPEAL_PTS,
    MOVE_REMINDER_WIN_MOVE_POWER,
    MOVE_REMINDER_WIN_MOVE_ACCURACY,
    MOVE_REMINDER_WIN_MOVE_PP,
    MOVE_REMINDER_WIN_MOVE_BATTLE_DESCRIPTION,
    MOVE_REMINDER_WIN_MOVE_CONTEST_DESCRIPTION,
    MOVE_REMINDER_WIN_MESSAGE_BOX,
    MOVE_REMINDER_WIN_MOVES_NAMES,
    MOVE_REMINDER_WIN_YES_NO_MENU,
    MAX_MOVE_REMINDER_WIN
};

enum {
    MOVE_REMINDER_SPRITE_SIDE_ARROW_LEFT = 0,
    MOVE_REMINDER_SPRITE_SIDE_ARROW_RIGHT,
    MOVE_REMINDER_SPRITE_SCROLL_ARROW_UP,
    MOVE_REMINDER_SPRITE_SCROLL_ARROW_DOWN,
    MOVE_REMINDER_SPRITE_MOVE_SELECTOR,
    MOVE_REMINDER_SPRITE_CATEGORY,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_0,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_1,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_2,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_3,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_4,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_5,
    MOVE_REMINDER_SPRITE_TYPE_MOVE_6,
    MAX_MOVE_REMINDER_SPRITE
};

typedef struct MoveReminderController {
    MoveReminderData *data;
    BgConfig *bgConfig;
    Window windows[MAX_MOVE_REMINDER_WIN];
    MsgData *messageLoader;
    MessageFormat *stringTemplate;
    String *string;
    ListMenu *listMenu;
    ListMenuItem *stringList;
    ListMenu2D *yesNoMenu;
    SpriteSystem *spriteSystem;
    SpriteManager *spriteManager;
    ManagedSprite *managedSprites[MAX_MOVE_REMINDER_SPRITE];
    struct { void *monData; struct Options *options; u16 *OTName; u32 OTID; u8 OTGender; u8 dataType; u8 mode; u8 monMax; u8 monIndex; u8 pageFlags; u8 selectedMoveSlot; u8 returnMode; u16 move; u8 padding_1A[2]; u32 dexMode; void *specialRibbons; void *poffin; struct SOUND_CHATOT *chatotCry; BOOL showContest; } summary;
    OverlayManager *appMan;
    int nextState;
    u8 numMoves;
    u8 textPrinterID;
    u8 yesNoCallback;
    u8 pad_187[1];
    u16 unk_188;
    u8 pad_18A[14];
    u32 unk_198;
    u32 unk_19C;
    u32 unk_1A0;
    u8 pad_1A4[8];
    u32 unk_1AC;
    u8 pad_1B0[9];
    u8 unk_1B9;
    u8 pad_1BA[6];
    u32 unk_1C0;
} MoveReminderController;

extern const SpriteResourceCountsListUnion ov14_021F80D4;

extern const OamManagerParam ov14_021F80EC;

extern const OamCharTransferParam ov14_021F80A8;

void ov14_021F2AC8(UnkStruct_ov14_boxsys *self) {
    SpriteResourceCountsListUnion capacities = ov14_021F80D4;

    self->unk_2F4 = SpriteSystem_Alloc((enum HeapID)10);
    self->unk_2F8 = SpriteManager_New(self->unk_2F4);

    OamManagerParam renderOamTemplate = ov14_021F80EC;

    OamCharTransferParam charTransferTemplate = ov14_021F80A8;

    SpriteSystem_Init(self->unk_2F4, &renderOamTemplate, &charTransferTemplate, 32);
    SpriteSystem_InitSprites(self->unk_2F4, ((SpriteManager *)self->unk_2F8), 0x67);
    SpriteSystem_InitManagerWithCapacities(self->unk_2F4, ((SpriteManager *)self->unk_2F8), &capacities);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(self->unk_2F4), 0, 0x200000);
}
