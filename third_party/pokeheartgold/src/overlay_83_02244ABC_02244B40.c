#include "global.h"
#include "battle_castle_app_sprite_manager.h"
#include "overlay_83_battle_castle_opponent_app.h"
#include "battle_castle_save.h"

struct ApplicationManager;

struct ApplicationManagerTemplate;

struct BattleCastleAppSprite;

struct BattleCastleOpponentApp;

struct BattleCastlePersistentSave;

struct BattleCastleSave;

struct BattleFrontierSave;

struct BgConfig;

struct ColoredArrow;

struct FontSpecialCharsContext;

struct G2dRenderer;

struct ListMenu;

struct ListMenuTemplate;

struct Menu;

struct MenuTemplate;

struct MessageLoader;

struct NARC;

struct Options;

struct PaletteData;

struct Party;

struct SaveData;

struct Sprite;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct StringList;

struct StringTemplate;

struct TrainerInfo;

struct Window;

typedef struct ApplicationManager ApplicationManager;

typedef BOOL (*OverlayFunc)(ApplicationManager *appMan, int *state);

typedef struct ApplicationManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} ApplicationManagerTemplate;

struct ApplicationManager {
    ApplicationManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    ApplicationManager *parent;
    ApplicationManager *child;
};

enum PaddingMode {
    PADDING_MODE_NONE = 0,
    PADDING_MODE_SPACES,
    PADDING_MODE_ZEROES,
};

typedef u16 charcode_t;

typedef struct String String;

typedef struct SaveData SaveData;

typedef struct Options Options;

typedef struct NARC NARC;

typedef struct PaletteData PaletteData;

typedef struct Party Party;

typedef struct SpriteList SpriteList;

typedef struct Sprite {
    VecFx32 position;
    VecFx32 affineTranslation;
    VecFx32 affineScale;
    u16 affineZRotation;
    u8 affineOverwriteMode;
    u8 flip;
    u8 overwriteFlags;
    u8 explicitPalette;
    u8 explicitPaletteOffset;
    BOOL explicitMosaic;
    GXOamMode explicitOamMode;
    u8 draw;
    u8 animate;
    fx32 animSpeed;
    SpriteList *list;

    u32 animData[29];
    NNSG2dImageProxy imageProxy;
    NNSG2dImagePaletteProxy paletteProxy;
    u32 type;
    u16 activeAnimID;
    u8 explicitPriority;
    u16 priority;
    enum NNS_G2D_VRAM_TYPE vramType;
    struct Sprite *prev;
    struct Sprite *next;
} Sprite;

struct SpriteList {
    Sprite *sprites;
    int capacity;
    Sprite **freeSprites;
    int freeSpriteHead;
    Sprite sentinelData;
    NNSG2dRendererInstance *renderer;
    void *rawAnimData;
    NNSG2dCellAnimBankData *defaultAnimBank;
    BOOL active;
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

typedef struct SpriteResource SpriteResource;

typedef struct SpriteResourceCollection SpriteResourceCollection;

typedef struct TrainerInfo {
    charcode_t name[7 + 1];
    u32 id;
    u32 money;
    u8 gender;
    u8 language;
    u8 badgeMask;
    u8 appearance;
    u8 gameCode;
    u8 isMainStoryCleared : 1;
    u8 hasNationalDex : 1;
    u8 : 6;
} TrainerInfo;

u32 PlayerProfile_GetTrainerGender(const TrainerInfo *info);

enum Gender {
    GENDER_MALE = 0,
    GENDER_FEMALE = 1,
    GENDER_NONE = 2,
};

typedef struct StringTemplate StringTemplate;

void BufferPlayersName(StringTemplate *template, u32 idx, const TrainerInfo *playerInfo);



typedef struct BattleFrontierSave BattleFrontierSave;

typedef struct BattleCastleAppSprite BattleCastleAppSprite;

enum BgLayer {
    BG_LAYER_MAIN_0 = 0,
    BG_LAYER_MAIN_1,
    BG_LAYER_MAIN_2,
    BG_LAYER_MAIN_3,
    BG_LAYER_SUB_0,
    BG_LAYER_SUB_1,
    BG_LAYER_SUB_2,
    BG_LAYER_SUB_3,

    BG_LAYER_MAX,
};

typedef struct BgConfig BgConfig;

typedef struct Window {
    BgConfig *bgConfig;
    u8 bgLayer;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 palette;
    u16 baseTile : 15;
    u16 colorMode : 1;
    void *pixels;
} Window;

enum BattleCastleOpponentAppWindow {
    OPPONENT_APP_WINDOW_HEADER = 0,
    OPPONENT_APP_WINDOW_EXIT,
    OPPONENT_APP_WINDOW_LEVELS,
    OPPONENT_APP_WINDOW_HP_BARS,
    OPPONENT_APP_WINDOW_SUMMARY_SCREEN,
    OPPONENT_APP_WINDOW_MON_OPTIONS_MENU,
    OPPONENT_APP_WINDOW_SUMMARY_MENU,
    OPPONENT_APP_WINDOW_MSG_BOX,
    OPPONENT_APP_WINDOW_MON_SELECTION_MSG_BOX,
    OPPONENT_APP_WINDOW_MON_OPTION_MSG_BOX,
    OPPONENT_APP_WINDOW_YES_NO_MENU,
    OPPONENT_APP_WINDOW_LEVEL_MENU,
    NUM_OPPONENT_APP_WINDOWS,
};

enum Font {
    FONT_SYSTEM = 0,
    FONT_MESSAGE,
    FONT_SUBSCREEN,
    FONT_UNOWN,

    FONT_MAX,
};

typedef u32 TextColor;

enum TextAlignment {
    TEXT_ALIGN_LEFT,
    TEXT_ALIGN_CENTER,
    TEXT_ALIGN_RIGHT
};

typedef struct ColoredArrow ColoredArrow;

typedef struct MessageLoader MessageLoader;

typedef struct StringList {
    void *entry;
    union {
        u32 index;
        void *callbackFunction;
    };
} StringList;

typedef struct ListMenu ListMenu;

typedef void (*CursorCallback)(ListMenu *menu, u32 index, u8 onInit);

typedef void (*PrintCallback)(ListMenu *menu, u32 index, u8 yOffset);

typedef struct ListMenuTemplate {
    const StringList *choices;
    CursorCallback cursorCallback;
    PrintCallback printCallback;
    Window *window;
    u16 count;
    u16 maxDisplay;
    u8 headerXOffset;
    u8 textXOffset;
    u8 cursorXOffset;
    u8 yOffset : 4;
    u8 textColorFg : 4;
    u8 textColorBg : 4;
    u8 textColorShadow : 4;
    u16 letterSpacing : 3;
    u16 lineSpacing : 4;
    u16 pagerMode : 2;
    u16 fontID : 6;
    u16 cursorType : 1;
    void *parent;
} ListMenuTemplate;

struct ListMenu {
    ListMenuTemplate template;
    u8 cursorPal : 4;
    u8 fillValue : 4;
    u8 cursorShadowPal : 4;
    u8 unk_21_4 : 4;
    u8 lettersSpacing : 6;
    u8 unk_23;
    u8 fontId : 7;
    u8 overrideEnabled : 1;
    struct ListMenuCursor *cursor;
    u16 cursorPos;
    u16 itemsAbove;
    u8 unk_30;
    u8 unk_31;
    u8 taskId;
    u8 unk_33;
    u8 heapID;
};

typedef struct MenuTemplate {
    StringList *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} MenuTemplate;

typedef struct Menu Menu;

TrainerInfo *sub_02034818(int netId);

u16 sub_0203769C(void);

typedef struct FontSpecialCharsContext FontSpecialCharsContext;

enum BattleCastleAppState {
    STATE_FADE_IN = 0,
    STATE_MAIN_FLOW,
    STATE_SYNC_PURCHASE,
    STATE_SYNC_BEFORE_EXIT,
    STATE_FADE_OUT
};

extern u8 ov83_0224484C(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font);

void LoadMainBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

void LoadSummaryBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

void LoadMovesListBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

void LoadSubScreenBackground(BattleCastleOpponentApp *app, enum BgLayer bgLayer);

u8 ov81_0224080C(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

u8 PrintMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u32 xOffset, u32 yOffset, u32 renderDelay, u8 fgColor, u8 shadowColor, u8 bgColor, u8 font, enum TextAlignment alignment);

void PrintStaticMessage(BattleCastleOpponentApp *app, Window *window, int entryID, u16 xOffset, u16 yOffset, enum TextAlignment alignment);

void ov82_0223EFB4(BattleCastleOpponentApp *app, u32 idx, s32 num, u32 maxDigits, enum PaddingMode paddingMode);

void ChangeState(BattleCastleOpponentApp *app, int *state, enum BattleCastleAppState newState);

void ov83_02244B40(BattleCastleOpponentApp *app, Window *window, u32 xOffset, u32 yOffset, u8 unused);

void ov83_02244B40(BattleCastleOpponentApp *app, Window *window, u32 xOffset, u32 yOffset, u8 unused) {
    TrainerInfo *partnerInfo = sub_02034818(sub_0203769C() ^ 1);

    TextColor textColor;
    if (PlayerProfile_GetTrainerGender(partnerInfo) == GENDER_MALE) {
        textColor = ((TextColor)(((7 & 0xFF) << 16) | ((8 & 0xFF) << 8) | ((0 & 0xFF) << 0)));
    } else {
        textColor = ((TextColor)(((3 & 0xFF) << 16) | ((4 & 0xFF) << 8) | ((0 & 0xFF) << 0)));
    }

    BufferPlayersName(app->strTemplate, 0, partnerInfo);
    ov83_0224484C(app, window, 1, xOffset, yOffset, 0xFF, (((textColor) >> 16) & 0xFF), (((textColor) >> 8) & 0xFF), (((textColor) >> 0) & 0xFF), FONT_SYSTEM);
}
