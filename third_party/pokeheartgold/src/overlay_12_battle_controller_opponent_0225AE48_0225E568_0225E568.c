#include "global.h"

#include "battle/battler_sprite_context.h"

#include "constants/heap.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"

#include "bg_window.h"
#include "filesystem.h"
#include "filesystem_files_def.h"
#include "palette.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "struct_defs/resdat_narc_structs.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "battle/battle_cursor.h"
#include "overlay_87_scratch_off_card_app.h"

struct ActionSelectData;

struct BattleMonOBJData;

struct BattleSubscreen;

struct BattleSystem;

struct OpponentData;

struct BattlerDataSysTaskFuncs;

struct BgConfig;

struct CharTransferAllocation;

struct CoordinatesS16;


struct TextOBJ;

struct FontOAMEntry;

struct GiveUpMoveData;

struct BattleHpBar;

struct Indicator;

struct ManagedSprite;

struct MenuButtonPressAnimState;

struct MenuCursor;

struct MoveDisplayData;

struct MoveDisplayInfo;

struct MoveSelectData;

struct NARC;

struct PaletteData;

struct SelectedPaletteData;

struct PartyBallAnimState;

struct Pokepic;

struct PokepicShadow;

struct PokepicDrawParam;

struct SavedCursorPosition;

struct SpriteAnimFrame;

struct SpriteManager;

struct SpriteResourcesHeader;

struct SpriteResourcesHeaderList;

struct SpriteSystem;

struct SpriteSystemContext;

struct SubscreenTilemapRect;

struct SysTask;

struct SysTaskQueue;

struct TargetMonData;

struct TargetSelectData;

struct TargetSelectMenuData;

struct TextWindowLayout;

struct UnkStruct_02012B20_t;

struct Window;

typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

void ov12_02264EB4(BattleHpBar *healthbox);

void ov12_02265D74(BattleHpBar *healthbox);

typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);

typedef struct BattleMonOBJData {
    enum HeapID heapID;
    int battlerIdx;
    SpriteSystemContext spriteContext;
    int resourceIDs[4];
    ManagedSprite *managedSprites[4];
    PokemonSpriteData *pokemonSpriteDataArray[4];
    u8 battlerTypes[4];
    Pokepic *pokemonSprites[4];
} BattleMonOBJData;

typedef struct SavedCursorPosition {
    s8 actionMenuX;
    s8 actionMenuY;
    s8 moveMenuX;
    s8 moveMenuY;
    s8 targetSelectX;
    s8 targetSelectY;
    s8 targetingLayout;
    u8 padding;
} SavedCursorPosition;

typedef struct BattlerDataSysTaskFuncs {
    SysTaskFunc setCommandSelection;
    SysTaskFunc showMoveSelectMenu;
    SysTaskFunc showTargetSelectMenu;
    SysTaskFunc showBagMenu;
    SysTaskFunc showPartyMenu;
    SysTaskFunc showYesNoMenu;
} BattlerDataSysTaskFuncs;

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;


typedef struct Indicator {
    ManagedSprite *sprite;
    SysTask *animTask;
    int x;
    int y;
    int bounceAngle;
    fx32 subscreenOffset;
    s16 exitTimer;
    u8 exitPending;
    u8 hasDropped;
    u8 isExiting;
    u8 exitStep;
    u8 exitFrameCount;
} Indicator;

typedef struct SubscreenTilemapRect {
    u8 top;
    u8 bottom;
    u8 left;
    u8 right;
} SubscreenTilemapRect;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
} UnusedStruct_ov16_0226A3F4;

typedef struct {
    SysTask *unk_00;
    u8 *unk_04;
    void *unk_08;
    u8 *unk_0C;
    UnusedStruct_ov16_0226A3F4 unk_10;
    u16 unk_1C;
} UnusedStruct_ov16_02268A14_sub3_sub1;

typedef struct MenuButtonPressAnimState {
    s16 phase;
    s16 waitTimer;
    union {
        struct {
            const s16 *keyframes;
            const SubscreenTilemapRect *subscreenTilemapRect;
            int selectedButton;
            u8 srcBufIndex;
            u8 fontOAMIdx;
            u8 spriteIndex;
            u8 unread;
            u8 freeSprites;
            CoordinatesS16 ov12_0226E2D8;
        } ButtonPressAnimationState;
        struct {
            UnusedStruct_ov16_02268A14_sub3_sub1 *unused_00[4];
            UnusedStruct_ov16_02268A14_sub3_sub1 *unused_10;
            NNSG2dCharacterData *unused_14;
            void *unused_18;
            int selectedTarget;
        } TargetSelectAnimationState;
    };
} MenuButtonPressAnimState;

typedef struct {
    int unk_00;
    s16 unk_04;
    s16 unk_06;
    u8 unk_08;
    u8 unk_09;
} UnusedStruct_ov16_02268A14_sub4;

typedef struct {
    const SubscreenTilemapRect *unk_00;
    u8 unk_04;
    s8 unk_05;
} UnusedStruct_ov16_02268A14_sub1;

typedef struct FontOAMEntry {
    TextOBJ *fontOAM;
    CharTransferAllocation allocation;
    u16 textWidth;
} FontOAMEntry;

typedef struct PartyBallAnimState {
    s16 xOffset;
    u8 expPercent;
    u8 phase;
    u8 unused_04;
    u8 delay;
    u8 frameCount;
    u8 duration;
    u8 bounceCount;
} PartyBallAnimState;

typedef struct MenuCursor {
    u8 isActive;
    s8 y;
    s8 x;
    u8 unused_03;
} MenuCursor;

typedef struct ActionSelectData {
    u8 battlerType;
    u8 battler;
    u8 partySlot;
    u8 ballStatusBattler;
    s16 curHp;
    u16 maxHp;
    u8 isWaitingForPartner;
} ActionSelectData;

typedef struct MoveSelectData {
    u16 moveIDs[4];
    u8 currPP[4];
    u8 maxPP[4];
    u8 battlerType;
} MoveSelectData;

typedef struct TargetMonData {
    u8 partySlot;
    u8 gender : 2;
    u8 isPresent : 1;
    u8 : 5;
    u8 stockStatus;
    u8 padding_03;
    s16 curHP;
    u16 maxHP;
} TargetMonData;

typedef struct TargetSelectData {
    TargetMonData targetMonData[4];
    u8 battlerType;
    u8 targetingLayout;
} TargetSelectData;

typedef struct GiveUpMoveData {
    u16 move;
    u16 unused_02;
} GiveUpMoveData;

typedef union ActiveMenuData {
    ActionSelectData actionData;
    MoveSelectData moveData;
    TargetSelectData targetSelectData;
    GiveUpMoveData giveUpMoveData;
} ActiveMenuData;

typedef struct TextWindowLayout {
    Window window;
    u16 width;
    u16 height;
} TextWindowLayout;

typedef struct MoveDisplayInfo {
    u16 move[4];
    u16 curPP[4];
    u16 maxPP[4];
} MoveDisplayInfo;

typedef struct MoveDisplayData {
    MoveDisplayInfo moveDisplayInfo;
    u16 *moveIcons[4];
    TextWindowLayout moveNameWindows[4];
    TextWindowLayout ppCountWindows[4];
    TextWindowLayout ppLabelWindows[4];
} MoveDisplayData;

typedef struct BattleSubscreen {
    BattleSystem *battleSys;
    u8 *suppressActivationSfxPtr;
    SysTask *animatePartyBallsTask;
    SysTask *menuTransitionTask;
    SysTask *unused_10;
    UnusedStruct_ov16_02268A14_sub1 unused_14;
    ActiveMenuData activeMenuData;
    u16 *tilemapBuffers[7];
    u16 *subscreenPaletteBuf;
    u16 *normalMovePalBuf;
    u16 *speedUpMovePalBuf;
    SysTask *speedUpPaletteTask;
    MoveDisplayData moveDisplayData[4];
    UnkStruct_02012744 *fontOAMManager;
    FontOAMEntry fontOAMEntry[13];
    UnkStruct_02012B20 *unused_5B8;
    ManagedSprite *playerPartyBallSprites[6];
    ManagedSprite *opponentPartyBallSprites[6];
    ManagedSprite *moveSelectSprites[4];
    ManagedSprite *categoryIconSprites[4];
    ManagedSprite *targetSelectPokeIconSprites[4];
    SysTask *pokeIconAnimTasks[4];
    PartyBallAnimState partyBallAnimState[6];
    SysTask *unused_664;
    u8 unused_668;
    u8 unused_669;
    u8 battlerType;
    s8 activeMenuConfigIndex;
    u8 targetingLayout;
    u8 trainerGender;
    u8 isPanelSliding;
    u8 hasCancelButton;
    u8 unused_670;
    s16 unused_672;
    u8 pulseCursorDirection;
    s16 pulseCursorBlendValue;
    SysTask *pulseCursorTask;
    MenuButtonPressAnimState menuButtonPressAnimState;
    UnusedStruct_ov16_02268A14_sub4 unused_6A0;
    s32 panelScrollPos;
    s32 slideSpeed;
    s32 targetOffset;
    BattleCursor *cursorRenderer;
    MenuCursor cursor;
    u8 suppressActivationSfx;
    u8 isWaitingForPartner;
    struct {
        Indicator *indicator;
        u8 step;
        u8 phase;
        u8 delay;
    } catchTutorialState;
} BattleSubscreen;

void BattleInput_ChangeMenu(NARC *unused, NARC *spriteNarc, BattleSubscreen *btlSubscreen, int menuConfigIndex, int forceRedraw, void *battleDisplayData);

int BattleInput_CheckTouch(BattleSubscreen *btlSubscreen);

BOOL BattleInput_CheckFeedbackDone(BattleSubscreen *btlSubscreen);

int ov12_02266C84(int range, int battlerType);

void BattleInput_Deadstriped_022698AC(BattleSubscreen *btlSubscreen, int param1);

BOOL ov12_022698B0(BattleSubscreen *btlSubscreen);

BgConfig *BattleSystem_GetBgConfig(BattleSystem *battleSys);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

OpponentData *BattleSystem_GetOpponentData(BattleSystem *battleSys, int battler);

BattleSubscreen *BattleSystem_GetBattleInput(BattleSystem *battleSys);

int BattleSystem_GetBattlerIdPartner(BattleSystem *battleSys, int battler);

void *BattleSystem_GetHpBar(BattleSystem *battleSys, int battler);

void ov12_0223C1A0(BattleSystem *battleSys, u8 *param1);

void ov12_02262014(OpponentData *battlerData);

BOOL PlaySE(u16 seqID);

typedef struct TargetSelectMenuData {
    BattleSystem *battleSys;
    void *healthbox;
    int input;
    u8 command;
    u8 battler;
    u8 battlerType;
    u8 state;
    TargetMonData targetMon[4];
    u16 range;
    u8 shouldHidePanel;
    u8 unused;
} TargetSelectMenuData;

void ov12_0225E568(SysTask *task, void *data);

void ov12_0225E568(SysTask *task, void *data) {
    TargetSelectMenuData *targetSelectMenuData = data;
    BgConfig *bgConfig = BattleSystem_GetBgConfig(targetSelectMenuData->battleSys);
    BattleHpBar *healthbox;
    OpponentData *battlerData = BattleSystem_GetOpponentData(targetSelectMenuData->battleSys, targetSelectMenuData->battler);
    BattleSubscreen *btlSubscreen = BattleSystem_GetBattleInput(targetSelectMenuData->battleSys);
    int partner = BattleSystem_GetBattlerIdPartner(targetSelectMenuData->battleSys, targetSelectMenuData->battler);

    if (partner != targetSelectMenuData->battler) {
        healthbox = BattleSystem_GetHpBar(targetSelectMenuData->battleSys, partner);
    } else {
        healthbox = 0L;
    }

    switch (targetSelectMenuData->state) {
    case 0:
        if (BattleInput_CheckFeedbackDone(btlSubscreen) == 0) {
            break;
        }

        TargetSelectData targetSelectData;
        NARC *bgNarc = NARC_New((NarcId)(NARC_a_0_0_7), HEAP_ID_BATTLE);
        NARC *objNarc = NARC_New((NarcId)((enum NarcId)8), HEAP_ID_BATTLE);

        for (int i = 0; i < 4; i++) {
            targetSelectData.targetMonData[i] = targetSelectMenuData->targetMon[i];
        }

        targetSelectData.battlerType = targetSelectMenuData->battlerType;
        targetSelectData.targetingLayout = ov12_02266C84(targetSelectMenuData->range, targetSelectMenuData->battlerType);

        BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 12, 0, &targetSelectData);
        NARC_Delete(bgNarc);
        NARC_Delete(objNarc);

        targetSelectMenuData->state++;
        break;
    case 1:
        targetSelectMenuData->input = BattleInput_CheckTouch(btlSubscreen);

        if (targetSelectMenuData->input != 0xFFFFFFFF) {
            PlaySE(1501);
            targetSelectMenuData->state++;
        }
        break;
    case 2:
        if (targetSelectMenuData->input != 0xFF) {
            ov12_02264EB4(targetSelectMenuData->healthbox);
            ov12_02262014(battlerData);
            ov12_02265D74(healthbox);

            if (targetSelectMenuData->shouldHidePanel == 1) {
                BattleInput_Deadstriped_022698AC(btlSubscreen, 0);
            }
        }

        targetSelectMenuData->state++;
        break;
    case 3:
        targetSelectMenuData->state++;
    default:
        if (ov12_022698B0(btlSubscreen) == 1) {
            u8 v10[6];
            int input = targetSelectMenuData->input;
            u32 battleType = BattleSystem_GetBattleType(targetSelectMenuData->battleSys);

            if (input != 0xFF) {
                ov12_0223C1A0(targetSelectMenuData->battleSys, &v10[0]);

                if (battleType & (1 << 1)) {
                    input = v10[2 + (targetSelectMenuData->input - 1)] + 1;
                } else {
                    input = v10[0 + (targetSelectMenuData->input - 1)] + 1;
                }
            }

            BattleController_EmitSelectedTarget(targetSelectMenuData->battleSys, targetSelectMenuData->battler, input);
            BattleController_EmitClearCommand(targetSelectMenuData->battleSys, targetSelectMenuData->battler, targetSelectMenuData->command);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
