#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "pm_string.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sys_task.h"
#include "heap.h"
#include "sys_task_api.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "gf_gfx_planes.h"
#include "overlay_102_easy_chat_app.h"
#include "sprite.h"
#include "mail_misc.h"
#include "overlay_102_easy_chat_categories_context.h"
#include "message_printer.h"
#include "touch_hitbox_controller.h"
#include "overlay_102_app_021e937c.h"
#include "overlay_102_app_021ebc34.h"


struct BoxApplication;
struct BoxApplicationManager;
struct BoxCompareGraphics;
struct BoxDialGraphics;
struct BoxGraphics;
struct BoxItemIconGraphics;
struct EasyChatApp;
struct EasyChatArgs;
struct EasyChatWordList;
struct MessagePrinter;
struct MsgData;
struct Pokepic;
struct PokepicManager;
struct PokepicDrawParam;
struct SpriteList;
struct MessageFormat;
struct TouchHitboxController;
struct UnkStruct_ov20_021D2128_t;
struct EasyChatSentenceView;
struct UnkStruct_ov20_021D3980_t;
struct UnkStruct_ov20_021D4210_t;
struct UnkStruct_ov20_021D4AD4_t;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
typedef struct MonIcon {
    Sprite *sprite;
    NNSG2dImageProxy imageProxy;
    u32 spriteIndex;
    u16 plttIndex;
    u16 markings;
    u16 heldItem;
    u8 padding_32[2];
} MonIcon;
typedef struct BoxGraphics BoxGraphics;
typedef struct BoxItemIconGraphics BoxItemIconGraphics;
typedef struct BoxApplication BoxApplication;
typedef struct BoxApplicationManager BoxApplicationManager;
typedef struct BoxButtonsGraphics {
    SpriteList *spriteList;
    Sprite *sprite;
    void *cellBankBuffer;
    void *animBankBuffer;
    NNSG2dCellDataBank *cellBank;
    NNSG2dAnimBankData *animBank;
    NNSG2dImageProxy imageProxy;
    SpriteList *unusedSpriteList;
    const BoxApplication *unusedBoxApp;
    BoxGraphics *boxGraphics;
} BoxButtonsGraphics;
typedef struct MonIconHelper {
    void *iconAnimBankBuffer;
    NNSG2dAnimBankData *iconAnimBank;
    void *iconCellBankBuffer;
    u8 unused_0C;
    NNSG2dCellDataBank *iconCellBank;
    void *itemIconCellBankBuffer;
    void *itemIconAnimBankBuffer;
    NNSG2dCellDataBank *itemIconCellBank;
    NNSG2dAnimBankData *itemIconAnimBank;
    SpriteList *spriteList;
    u8 unused_28[24];
    const BoxApplication *boxApp;
    BoxGraphics *boxGraphics;
    NARC *iconNarc;
    u8 iconCharBuffer[(0x80 + ((4 * 4) * (8 * (8 / 2))))];
} MonIconHelper;
typedef struct BoxCompareGraphics BoxCompareGraphics;
typedef struct BoxView {
    u8 boxID;
    u8 unk_01;
    u8 currentPage;
    u8 previousPage;
    u32 scrollTicksRemaining;
    s32 scrollStartX;
    s32 bgScrollX;
    s32 scrollStepX;
    s32 scrollTargetX;
    u8 padding_18[24];
    u32 pendingBoxID;
    u32 pendingPage;
    fx32 unk_38[2][6];
    fx32 unk_68[2][6];
    u32 scrollStep;
    BOOL scrollDone;
    u16 boxCursorPosition;
    u8 wallpaperChangeStep;
    u8 taskBusy;
    u8 releaseAnimStep;
    u8 padding_A5[3];
    MonIcon icons[2][(5 * 6)];
    u8 iconTileData[(5 * 6)][(0x80 + ((4 * 4) * (8 * (8 / 2))))];
    u16 iconSpriteIndex[(5 * 6)];
    u16 species[(5 * 6)];
    u8 padding_5850[4];
    int unk_5854;
    int unk_5858;
    int compareModeXOffset;
    u16 unk_5860[9][16];
    MonIconHelper *monIconHelper;
    BgConfig *bgConfig;
    const BoxApplication *boxApp;
    BoxGraphics *boxGraphics;
} BoxView;
typedef struct BoxPartyGraphics {
    BoxGraphics *boxGraphics;
    BgConfig *bgConfig;
    SpriteList *unusedSpriteList;
    const BoxApplication *boxApp;
    BoxView *boxView;
    void *panelScreenBuffer;
    NNSG2dScreenData *panelScreenData;
    VecFx32 *shiftPositions;
    fx32 moveStepX;
    fx32 moveStepY;
    u8 animSlot;
    u8 shiftCount;
    u8 animFrame;
    u8 hiddenRows;
    BOOL isAnimating;
    u32 tilemapLeft;
    int xOffset;
    MonIconHelper *monIconHelper;
    MonIcon monIcons[6];
    u8 releaseAnimStep;
    u8 padding_175[3];
} BoxPartyGraphics;
typedef struct CoordinatesFX32 {
    fx32 x;
    fx32 y;
} CoordinatesFX32;
typedef struct BoxCursorGraphics {
    SpriteList *spriteList;
    Sprite *cursorSprite;
    Sprite *shadowSprite;
    Sprite *leftArrowSprite;
    Sprite *rightArrowSprite;
    void *cellBankBuffer;
    void *animBankBuffer;
    NNSG2dCellDataBank *cellBank;
    NNSG2dAnimBankData *animBank;
    NNSG2dImageProxy imageProxy;
    MonIconHelper *monIconHelper;
    MonIcon monIcon[(5 * 6)];
    CoordinatesFX32 heldIconOffsets[(5 * 6)];
    u32 heldIconCount;
    BoxView *box;
    BoxPartyGraphics *party;
    BoxItemIconGraphics *itemIconGraphics;
    s32 posX;
    s32 posY;
    s32 targetX;
    s32 targetY;
    s32 animX;
    s32 animY;
    s32 stepX;
    s32 stepY;
    u8 animPhase;
    u8 cursorLocation;
    u8 newCursorLocation;
    u8 moveStepsRemaining;
    u8 isAnimating;
    s8 boxCol;
    s8 boxRow;
    s8 partyPosition;
    u8 hasHeldMonIcon;
    u8 isHoldingItem;
    u8 shadowFollowsCursor;
    u8 padding_78F;
    const BoxApplication *boxApp;
    BoxGraphics *boxGraphics;
} BoxCursorGraphics;
typedef struct BoxDialGraphics BoxDialGraphics;
struct BoxItemIconGraphics {
    BOOL isMoveItemsMode;
    BoxGraphics *boxGraphics;
    const BoxApplication *boxApp;
    BgConfig *unusedBgConfig;
    SpriteList *spriteList;
    MonIconHelper *monIconHelper;
    BoxCursorGraphics *cursorGraphics;
    Sprite *sprites[3];
    int spriteRoles[3];
    int activeMoveCount;
    int shrinkingSprite;
};
typedef struct BoxMenuGraphics {
    BgConfig *bgConfig;
    u8 unused_04[4];
    const BoxApplication *unusedBoxApp;
    BoxGraphics *unusedBoxGraphics;
    MsgData *boxMessagesLoader;
    MsgData *menuItemsLoader;
    Window *windows;
    u16 isMessageBoxOpen;
    u16 isMenuOpen;
    int optionsFrame;
    void *markingsCharDataBuffer;
    NNSG2dCharacterData *markingsCharData;
    const MessageFormat *messageVariableBuffer;
    String *tempString;
    String *formattedString;
} BoxMenuGraphics;
typedef struct BoxPopupGraphics {
    BoxGraphics *boxGraphics;
    BgConfig *bgConfig;
    SpriteList *spriteList;
    const BoxApplication *boxApp;
    Window *windows;
    void *thumbnailCharDataBuffer;
    NNSG2dCharacterData *thumbnailCharData;
    void *thumbnailCellBankBuffer;
    NNSG2dCellDataBank *thumbnailCellBank;
    u8 unused_24[8];
    void *boxSelectionScreenBuffer;
    NNSG2dScreenData *boxSelectionScreenData;
    void *itemInfoScreenBuffer;
    NNSG2dScreenData *itemInfoScreenData;
    Bitmap thumbnailBitmap;
    MessagePrinter *monCountChars;
    Sprite *thumbnailSprites[18];
    Sprite *itemIconSprite;
    u32 boxMonCounts[18];
    SysTask *animTask;
    s32 selectedBoxID;
    u16 thumbnailFadePltts[8][(3 * (16))];
    u8 thumbnailCache[18][32 * 32];
    u8 isThumbnailCached[18];
    u8 padding_4BF6[2];
    MsgData *itemDescriptionsLoader;
    String *string;
} BoxPopupGraphics;
typedef struct PCMonPreviewDisplay {
    BOOL isActive;
    Window *windows;
    BgConfig *bgConfig;
    SpriteList *spriteList;
    const BoxApplication *boxApp;
    BoxGraphics *boxGraphics;
    MessagePrinter *dexNumChars;
    MessagePrinter *levelChars;
    PokepicManager *monSpriteManager;
    Pokepic *monSprite;
    void *infoFrameCellBankBuffer;
    NNSG2dCellDataBank *infoFrameCellBank;
    Sprite *infoFrameSprite;
    void *typeIconCellBankBuffer;
    NNSG2dCellDataBank *typeIconCellBank;
    Sprite *typeIconSprites[2];
    String *maleGenderText;
    String *femaleGenderText;
    SysTask *rotationTask;
    u8 padding_50[4];
} PCMonPreviewDisplay;
struct BoxGraphics {
    SysTask *drawSceneTask;
    SysTask *vBlankTask;
    SysTask *taskSlots[4];
    SpriteList *spriteList;
    G2dRenderer renderer;
    NNSG2dImagePaletteProxy imagePaletteProxy;
    PokepicManager *monSpriteManager;
    BgConfig *bgConfig;
    const BoxApplication *boxApp;
    MonIconHelper monIconHelper;
    BoxView box;
    BoxCursorGraphics cursorGraphics;
    BoxButtonsGraphics buttonsGraphics;
    PCMonPreviewDisplay previewDisplay;
    BoxMenuGraphics menuGraphics;
    BoxPopupGraphics popupGraphics;
    BoxPartyGraphics partyGraphics;
    BoxDialGraphics *dialGraphics;
    BoxItemIconGraphics *itemIconGraphics;
    BoxCompareGraphics *compareGraphics;
    const BoxApplicationManager *boxAppMan;
};
typedef struct UnkStruct_ov20_021D2128_t UnkStruct_ov20_021D2128;
typedef struct EasyChatWordList EasyChatWordList;
typedef int (*EasyChatMainFunc)(EasyChatApp *easyChatApp, int *state);
typedef void (*EasyChatTaskFunc)(EasyChatApp *easyChatApp, int *state);
typedef struct {
    u8 unk_00;
    u8 unk_01;
} UnkStruct_ov20_021D1F34;
typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 unk_06;
    u8 unk_07;
    int unk_08;
} UnkStruct_ov20_021D18BC;
typedef struct {
    s16 unk_00;
    s8 unk_02;
    s8 unk_03;
} UnkStruct_ov20_021D0F64;
typedef struct {
    BgConfig *unk_00;
    u32 unk_04;
    int unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    fx32 unk_14;
    u16 unk_18;
    u16 unk_1A;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4E8C;
typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    int unk_0C;
    int unk_10;
    int unk_14;
    int unk_18;
    SysTask *unk_1C;
} UnkStruct_ov20_021D4FF0;
typedef struct EasyChatSentenceView EasyChatSentenceView;
typedef struct UnkStruct_ov20_021D3980_t UnkStruct_ov20_021D3980;
typedef struct UnkStruct_ov20_021D4210_t UnkStruct_ov20_021D4210;
typedef struct UnkStruct_ov20_021D4AD4_t UnkStruct_ov20_021D4AD4;
struct UnkStruct_ov20_021D2128_t {
    SysTask *unk_00;
    SysTask *unk_04;
    SysTask *unk_08[4];
    const EasyChatApp *unk_18;
    const EasyChatWordList *unk_1C;
    BgConfig *unk_20;
    SpriteList *unk_24;
    G2dRenderer unk_28;
    NNSG2dImageProxy unk_1B4[2];
    NNSG2dImagePaletteProxy unk_1FC[2];
    NNSG2dCellDataBank *unk_224[2];
    NNSG2dCellAnimBankData *unk_22C[2];
    void *unk_234[2];
    void *unk_23C[2];
    EasyChatSentenceView *unk_244;
    UnkStruct_ov20_021D3980 *unk_248;
    EasyChatCategoriesContext *unk_24C;
    UnkStruct_ov20_021D4210 *unk_250;
    UnkStruct_ov20_021D4AD4 *unk_254;
};
typedef struct {
    UnkStruct_ov20_021D2128 *unk_00;
    const EasyChatApp *unk_04;
    const EasyChatWordList *unk_08;
    u32 unk_0C;
    int unk_10;
    int unk_14;
} UnkStruct_ov20_021D2238;

BOOL ov102_021E9464(BoxGraphics *boxGraphics);
BOOL ov102_021E947C(UnkStruct_ov20_021D2128 *param0, int param1);

void ov102_021E94A4(void *arg0);

s32 ov102_021E91C4(void *);
s32 ov102_021E95E0(void *, NARC *);
s32 ov102_021E978C(void *);
void *ov102_021EA314(void *, struct EasyChatApp *, struct EasyChatWordList *);
s32 ov102_021EA41C(void *, NARC *);
void *ov102_021EB1A4(void *, struct EasyChatApp *, struct EasyChatWordList *);
s32 ov102_021EB1F8(void *, NARC *);
void *ov102_021EB694(void *, struct EasyChatApp *, struct EasyChatWordList *);
s32 ov102_021EB6E0(s32 *, NARC *);
void *ov102_021EBC34(void *, struct EasyChatApp *, struct EasyChatWordList *);
void ov102_021EBD00(Ov102App_021EBC34 *);
void *ov102_021EC20C(void *, struct EasyChatApp *, struct EasyChatWordList *);
s32 ov102_021EC248(void *, NARC *);
void ov102_021E94CC(SysTask *param0, UnkStruct_ov102_021E978C *param1);

BOOL ov102_021E9464(BoxGraphics *boxGraphics) {
    for (int i = 0; i < 4; i++) {
        if (boxGraphics->taskSlots[i] != 0L) {
            return 0;
        }
    }

    return 1;
}

BOOL ov102_021E947C(UnkStruct_ov20_021D2128 *param0, int param1) {
    int v0;

    for (v0 = 0; v0 < 4; v0++) {
        if (param0->unk_08[v0] != 0L) {
            UnkStruct_ov20_021D2238 *v1 = SysTask_GetData(param0->unk_08[v0]);

            if (v1->unk_0C == param1) {
                return 0;
            }
        }
    }

    return 1;
}

void ov102_021E94A4(void *ptr) {
    SysTask_Destroy(*(SysTask **)((u8 *)(*(u8 **)ptr + (*(s32 *)((u8 *)ptr + 0x10) * 4)) + 8));
    *(s32 *)((u8 *)(*(u8 **)ptr + (*(s32 *)((u8 *)ptr + 0x10) * 4)) + 8) = 0;
    Heap_Free(ptr);
}

void ov102_021E94CC(SysTask *param0, UnkStruct_ov102_021E978C *param1) {
    NARC *narc;

    GfGfx_DisableEngineAPlanes();
    GfGfx_DisableEngineBPlanes();
    *(s32 *)REG_DISPCNT_ADDR &= 0xFFFFE0FF;
    *(s32 *)REG_DB_DISPCNT_ADDR &= 0xFFFFE0FF;
    *(s32 *)REG_DISPCNT_ADDR = 0x100010 | (*(s32 *)REG_DISPCNT_ADDR & 0xFFCFFFEF);
    *(s32 *)REG_DB_DISPCNT_ADDR = 0x10 | (*(s32 *)REG_DB_DISPCNT_ADDR & 0xFFCFFFEF);
    narc = NARC_New(NARC_a_0_7_6, HEAP_ID_35);
    ov102_021E978C(param1);
    ov102_021E95E0(param1, narc);
    ov102_021E91C4(param1->unk_0);
    *(void **)((u8 *)(param1->unk_0) + 0x1E0) = ov102_021EA314(param1->unk_0, param1->unk_4, param1->unk_8);
    ov102_021EA41C(*(void **)((u8 *)(param1->unk_0) + 0x1E0), narc);
    *(void **)((u8 *)(param1->unk_0) + 0x1E4) = ov102_021EB1A4(param1->unk_0, param1->unk_4, param1->unk_8);
    ov102_021EB1F8(*(void **)((u8 *)(param1->unk_0) + 0x1E4), narc);
    *(void **)((u8 *)(param1->unk_0) + 0x1E8) = ov102_021EB694(param1->unk_0, param1->unk_4, param1->unk_8);
    ov102_021EB6E0(*(void **)((u8 *)(param1->unk_0) + 0x1E8), narc);
    *(void **)((u8 *)(param1->unk_0) + 0x1EC) = ov102_021EBC34(param1->unk_0, param1->unk_4, param1->unk_8);
    ov102_021EBD00(*(void **)((u8 *)(param1->unk_0) + 0x1EC));
    *(void **)((u8 *)(param1->unk_0) + 0x1F0) = ov102_021EC20C(param1->unk_0, param1->unk_4, param1->unk_8);
    ov102_021EC248(*(void **)((u8 *)(param1->unk_0) + 0x1F0), narc);
    GfGfx_EngineATogglePlanes(0x10U, 1U);
    GfGfx_EngineBTogglePlanes(0x10U, 1U);
    GX_DispOn();
    NARC_Delete(narc);
    ov102_021E94A4(param1);
}
