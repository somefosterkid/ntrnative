#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "struct_defs/resdat_narc_structs.h"
#include "filesystem.h"
#include "palette.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "system.h"
#include "overlay_07_ball_rotation.h"
#include "pokepic.h"
#include "screen_fade_internal.h"
#include "battle/battle.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_12_menu_task_data.h"

struct ActionSelectData;

struct Background;

struct UnkBallData;

struct BattleMessage;

struct BattleMonOBJData;

struct BattleSubscreen;

struct BattleSystem;

struct OpponentData;

struct OpponentDataSysTaskFuncs;

struct BgConfig;

struct CharTransferAllocation;

struct CommandSetData;

struct CoordinatesS16;

struct BattleCursor;

struct TextOBJ;

struct FontOAMEntry;

struct GiveUpMoveData;

struct BattleHpBar;

struct Indicator;

struct ManagedSprite;

struct MenuButtonPressAnimState;

struct MenuCursor;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

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

struct OpponentDataCursorPos;

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

struct TextWindowLayout;

struct UnkStruct_02012744_t;

struct UnkStruct_02012B20_t;

struct Window;

NARC *NARC_New(enum NarcId narcID, enum HeapID heapID);

void NARC_Delete(NARC *narc);

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

void ov12_02264C84(BattleHpBar *healthbox);

void ov12_02264E84(BattleHpBar *healthbox);

void ov12_02264EB4(BattleHpBar *healthbox);

void dummy_02268468(BattleHpBar *healthbox);

void dummy_0226846C(BattleHpBar *healthbox);

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

enum BattleMessageTag {
    TAG_NONE = 0,
    TAG_NONE_SIDE_CONSCIOUS = 1,
    TAG_NICKNAME = 2,
    TAG_MOVE = 3,
    TAG_STAT = 4,
    TAG_ITEM = 5,
    TAG_NUMBER = 6,
    TAG_NUMBERS = 7,
    TAG_TRNAME = 8,
    TAG_NICKNAME_NICKNAME = 9,
    TAG_NICKNAME_MOVE = 10,
    TAG_NICKNAME_ABILITY = 11,
    TAG_NICKNAME_STAT = 12,
    TAG_NICKNAME_TYPE = 13,
    TAG_NICKNAME_POKE = 14,
    TAG_NICKNAME_ITEM = 15,
    TAG_NICKNAME_POFFIN = 16,
    TAG_NICKNAME_NUM = 17,
    TAG_NICKNAME_TRNAME = 18,
    TAG_NICKNAME_BOX = 19,
    TAG_MOVE_SIDE = 20,
    TAG_MOVE_NICKNAME = 21,
    TAG_MOVE_MOVE = 22,
    TAG_ABILITY_NICKNAME = 23,
    TAG_ITEM_MOVE = 24,
    TAG_NUMBER_NUMBER = 25,
    TAG_TRNAME_TRNAME = 26,
    TAG_TRNAME_NICKNAME = 27,
    TAG_TRNAME_ITEM = 28,
    TAG_TRNAME_NUM = 29,
    TAG_TRCLASS_TRNAME = 30,
    TAG_NICKNAME_NICKNAME_MOVE = 31,
    TAG_NICKNAME_NICKNAME_ABILITY = 32,
    TAG_NICKNAME_NICKNAME_ITEM = 33,
    TAG_NICKNAME_MOVE_MOVE = 34,
    TAG_NICKNAME_MOVE_NUMBER = 35,
    TAG_NICKNAME_ABILITY_NICKNAME = 36,
    TAG_NICKNAME_ABILITY_MOVE = 37,
    TAG_NICKNAME_ABILITY_ITEM = 38,
    TAG_NICKNAME_ABILITY_STAT = 39,
    TAG_NICKNAME_ABILITY_TYPE = 40,
    TAG_NICKNAME_ABILITY_STATUS = 41,
    TAG_NICKNAME_ABILITY_NUMBER = 42,
    TAG_NICKNAME_ITEM_NICKNAME = 43,
    TAG_NICKNAME_ITEM_MOVE = 44,
    TAG_NICKNAME_ITEM_STAT = 45,
    TAG_NICKNAME_ITEM_STATUS = 46,
    TAG_NICKNAME_BOX_BOX = 47,
    TAG_ITEM_NICKNAME_FLAVOR = 48,
    TAG_TRNAME_NICKNAME_NICKNAME = 49,
    TAG_TRCLASS_TRNAME_NICKNAME = 50,
    TAG_TRCLASS_TRNAME_ITEM = 51,
    TAG_NICKNAME_ABILITY_NICKNAME_MOVE = 52,
    TAG_NICKNAME_ABILITY_NICKNAME_ABILITY = 53,
    TAG_NICKNAME_ABILITY_NICKNAME_STAT = 54,
    TAG_NICKNAME_ITEM_NICKNAME_ITEM = 55,
    TAG_TRNAME_NICKNAME_TRNAME_NICKNAME = 56,
    TAG_TRCLASS_TRNAME_NICKNAME_NICKNAME = 57,
    TAG_TRCLASS_TRNAME_NICKNAME_TRNAME = 58,
    TAG_TRCLASS_TRNAME_TRCLASS_TRNAME = 59,
    TAG_TRCLASS_TRNAME_NICKNAME_TRCLASS_TRNAME_NICKNAME = 60,
};

typedef struct UnkStruct_02012B20_t UnkStruct_02012B20;

void SysTask_Destroy(SysTask *task);


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
    struct BattleCursor *cursorRenderer;
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

void BattleInput_SetPartyExpPercents(BattleSubscreen *btlSubscreen, const u8 *expPercents);

extern void BattleInput_UpdateBallGaugeAnimation(BattleSubscreen *btlSubscreen, u8 playerBallStatuses[], u8 opponentBallStatuses[]);

void BattleSubscreen_ShowBallSprites(BattleSubscreen *btlSubscreen);

void BattleInput_DisableBallGauge(BattleSubscreen *btlSubscreen);

int BattleInput_CheckTouch(BattleSubscreen *btlSubscreen);

BOOL BattleInput_CheckFeedbackDone(BattleSubscreen *btlSubscreen);

void BattleSubscreen_UpdateMoveDisplay(BattleSubscreen *btlSubscreen, int battlerSlot, const MoveDisplayInfo *moveDisplayInfo);

void BattleInput_Deadstriped_022698AC(BattleSubscreen *btlSubscreen, int param1);

BOOL ov12_022698B0(BattleSubscreen *btlSubscreen);

u8 BattleInput_GetCancelRunFlag(BattleSubscreen *btlSubscreen);

enum PartyGaugeSide {
    PARTY_GAUGE_OURS,
    PARTY_GAUGE_THEIRS,
    PARTY_GAUGE_COUNT
};

void BattleController_EmitClearCommand(BattleSystem *battleSys, int battler, int command);

void Heap_Free(void *ptr);

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;

BgConfig *BattleSystem_GetBgConfig(BattleSystem *battleSys);

u32 BattleSystem_GetBattleType(BattleSystem *battleSys);

OpponentData *BattleSystem_GetOpponentData(BattleSystem *battleSys, int battler);

int BattleSystem_GetMaxBattlers(BattleSystem *battleSys);

BattleSubscreen *BattleSystem_GetBattleSubscreen(BattleSystem *battleSys);

MsgData *BattleSystem_GetMessageLoader(BattleSystem *battleSys);

u16 BattleSystem_GetTrainerIndex(BattleSystem *battleSys, int battler);

u32 BattleSystem_GetTrainerGender(BattleSystem *battleSys, int battler);

int BattleSystem_GetBattlerIdPartner(BattleSystem *battleSys, int battler);

u32 BattleSystem_GetBattleSpecial(BattleSystem *battleSys);

u8 ov12_0223B580(BattleSystem *battleSys, int battler, u8 param2);

u8 BattleSystem_GetTextFrameDelay(BattleSystem *battleSys);

u8 ov12_0223BB04(BattleSystem *battleSys);

void ov12_0223BB64(BattleSystem *battleSys, int flags);

void *BattleSystem_GetHealthBox(BattleSystem *battleSys, int battler);

u32 BattleSystem_PrintBattleMessage(BattleSystem *battleSys, MsgData *msgLoader, BattleMessage *battleMsg, u8 renderDelay);

void ov12_02261FD4(OpponentData *battlerData, BattleSystem *battleSys);

void ov12_02262014(OpponentData *battlerData);

BOOL PlaySE(u16 sndseq);

typedef void (*UnkCallback)(void);



u8 TextPrinterCheckActive(u8 printerID);

typedef struct CommandSetData {
    BattleSystem *battleSys;
    void *healthbox;
    u8 command;
    u8 battler;
    u8 state;
    s8 unused_0B;
    int input;
    u8 ballStatus[PARTY_GAUGE_COUNT][6];
    u8 expPercents[6];
    u8 unused_22;
    u8 partySlot;
    u16 moves[4];
    u8 curPP[4];
    u8 maxPP[4];
    u8 battlerType;
    u8 msgIdx;
    s16 curHP;
    u16 maxHP;
    u8 ballStatusBattler;
    u8 switchingOrCanPickCommandMask;
} CommandSetData;

void BattleController_EmitSelectedCommand(BattleSystem *battleSystem, int battler, int command);

void ov12_0225DAD4(SysTask *task, void *data);

void *BattleSystem_GetBattleInput(BattleSystem *battleSystem);

void *BattleSystem_GetHpBar(BattleSystem *battleSystem, int battlerId);

void BattleInput_LoadFightMenuText(void *, int battlerId, void *);

void BattleInput_EnableBallGauge(void *);

void ov12_02265D70(void *);

void ov12_02265D74(void *);

void ov12_0225DAD4(SysTask *task, void *data) {
    CommandSetData *commandSetData = data;
    BgConfig *bgConfig = BattleSystem_GetBgConfig(commandSetData->battleSys);
    BattleSubscreen *btlSubscreen = BattleSystem_GetBattleInput(commandSetData->battleSys);
    OpponentData *battlerData = BattleSystem_GetOpponentData(commandSetData->battleSys, commandSetData->battler);
    u32 battleType = BattleSystem_GetBattleType(commandSetData->battleSys);
    int partner = BattleSystem_GetBattlerIdPartner(commandSetData->battleSys, commandSetData->battler);
    BattleHpBar *healthbox;

    if (partner != commandSetData->battler) {
        healthbox = BattleSystem_GetHpBar(commandSetData->battleSys, partner);
    } else {
        healthbox = 0L;
    }

    switch (commandSetData->state) {
    case 0:
        MoveDisplayInfo moveDisplayInfo;
        int i;

        for (i = 0; i < 4; i++) {
            moveDisplayInfo.move[i] = commandSetData->moves[i];
            moveDisplayInfo.curPP[i] = commandSetData->curPP[i];
            moveDisplayInfo.maxPP[i] = commandSetData->maxPP[i];
        }

        BattleInput_LoadFightMenuText(BattleSystem_GetBattleInput(commandSetData->battleSys), commandSetData->battlerType, &moveDisplayInfo);
        commandSetData->state++;

    case 1: {
        MsgData *msgLoader = BattleSystem_GetMessageLoader(commandSetData->battleSys);
        BattleMessage battleMsg;

        if (battleType & (1 << 10)) {
            switch (ov12_0223BB04(commandSetData->battleSys)) {
            case 0:
                battleMsg.tag = TAG_NICKNAME;
                battleMsg.param[0] = commandSetData->battler | (commandSetData->partySlot << 8);
                battleMsg.id = 921;
                BattleSystem_PrintBattleMessage(commandSetData->battleSys, msgLoader, &battleMsg, 0);
                commandSetData->state = 3;
                break;
            case 1:
                battleMsg.tag = TAG_NONE;
                battleMsg.id = 1226 + BattleSystem_GetTrainerGender(commandSetData->battleSys, 0);
                commandSetData->msgIdx = BattleSystem_PrintBattleMessage(commandSetData->battleSys, msgLoader, &battleMsg, BattleSystem_GetTextFrameDelay(commandSetData->battleSys));
                commandSetData->state = 2;
                break;
            }
        } else {
            if (battleType & (1 << 5)) {
                battleMsg.tag = TAG_TRNAME;
                battleMsg.param[0] = BattleSystem_GetTrainerIndex(commandSetData->battleSys, commandSetData->battler);
                battleMsg.id = 922;
            } else if (battleType & (1 << 9)) {
                battleMsg.tag = TAG_TRNAME;
                battleMsg.param[0] = BattleSystem_GetTrainerIndex(commandSetData->battleSys, commandSetData->battler);
                battleMsg.id = 1222;
            } else {
                battleMsg.tag = TAG_NICKNAME;
                battleMsg.param[0] = commandSetData->battler | (commandSetData->partySlot << 8);
                battleMsg.id = 921;
            }

            BattleSystem_PrintBattleMessage(commandSetData->battleSys, msgLoader, &battleMsg, 0);
            commandSetData->state = 3;
        }
    } break;
    case 2:
        if (TextPrinterCheckActive(commandSetData->msgIdx) == 0) {
            commandSetData->state = 3;
        }
        break;
    case 3:
        ov12_02264E84(commandSetData->healthbox);
        ov12_02261FD4(battlerData, commandSetData->battleSys);
        commandSetData->state = 4;
    case 4:
        if (BattleInput_CheckFeedbackDone(btlSubscreen) == 0) {
            break;
        }

        ActionSelectData actionData;

        NARC *bgNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
        NARC *objNarc = NARC_New((enum NarcId)8, HEAP_ID_BATTLE);

        actionData.battler = commandSetData->battler;
        actionData.battlerType = commandSetData->battlerType;
        actionData.partySlot = commandSetData->partySlot;
        actionData.curHp = commandSetData->curHP;
        actionData.maxHp = commandSetData->maxHP;
        actionData.ballStatusBattler = commandSetData->ballStatusBattler;
        actionData.isWaitingForPartner = ov12_0223B580(commandSetData->battleSys, commandSetData->battler, commandSetData->switchingOrCanPickCommandMask);

        if (battlerData->commandUIReady) {
            BattleInput_Deadstriped_022698AC(btlSubscreen, 1);

            if (BattleSystem_GetBattleSpecial(commandSetData->battleSys) & (1 << 0)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 6, 0, &actionData);
            } else if (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 9)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 10, 0, &actionData);
            } else if (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 5)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 8, 0, &actionData);
            } else if (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 12)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 20, 0, &actionData);
            } else if (actionData.battlerType == 4 && (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 3)) == 0) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 4, 0, &actionData);
            } else {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 3, 0, &actionData);
            }

            ov12_0223BB64(commandSetData->battleSys, 1);
        } else {
            if (BattleSystem_GetBattleSpecial(commandSetData->battleSys) & (1 << 0)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 5, 0, &actionData);
            } else if (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 9)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 9, 0, &actionData);
            } else if (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 5)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 7, 0, &actionData);
            } else if (BattleSystem_GetBattleType(commandSetData->battleSys) & (1 << 12)) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 19, 0, &actionData);
            } else if (actionData.battlerType != 4) {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 1, 0, &actionData);
            } else {
                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 2, 0, &actionData);
            }

            battlerData->commandUIReady = 1;
        }

        BattleInput_SetPartyExpPercents(btlSubscreen, commandSetData->expPercents);
        BattleInput_UpdateBallGaugeAnimation(btlSubscreen, commandSetData->ballStatus[0], commandSetData->ballStatus[1]);
        BattleInput_EnableBallGauge(btlSubscreen);
        NARC_Delete(bgNarc);
        NARC_Delete(objNarc);

        if (healthbox != 0L) {
            ov12_02265D70(healthbox);
        }

        commandSetData->state = 5;
        break;
    case 5:
        if (gSystem.newKeys & 0x0008) {
            OpponentData *battlerData;

            for (int i = 0; i < BattleSystem_GetMaxBattlers(commandSetData->battleSys); i++) {
                battlerData = BattleSystem_GetOpponentData(commandSetData->battleSys, i);
                ov12_02264C84(&battlerData->healthbox);
            }
        }

        commandSetData->input = BattleInput_CheckTouch(btlSubscreen);

        if (commandSetData->input != 0xFFFFFFFF) {
            commandSetData->unused_0B = 10;
            PlaySE(0x5dd);
            commandSetData->state = 6;
        }
        break;
    case 6:
        if (BattleInput_CheckFeedbackDone(btlSubscreen) == 1 || commandSetData->input == 1) {
            switch (commandSetData->input) {
            case 1:
                if (BattleSystem_GetBattleType(commandSetData->battleSys) & ((1 << 5) | (1 << 9))) {
                    commandSetData->state = 7;
                }
                break;
            case 2: {
                NARC *bgNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *objNarc = NARC_New((enum NarcId)8, HEAP_ID_BATTLE);

                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 0, 0, 0L);
                BattleInput_Deadstriped_022698AC(btlSubscreen, 0);
                ov12_02265D74(healthbox);
                ov12_02264EB4(commandSetData->healthbox);
                ov12_02262014(battlerData);
                NARC_Delete(bgNarc);
                NARC_Delete(objNarc);
            } break;
            case 3: {
                NARC *bgNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *objNarc = NARC_New((enum NarcId)8, HEAP_ID_BATTLE);

                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 0, 0, 0L);
                BattleInput_Deadstriped_022698AC(btlSubscreen, 0);
                ov12_02265D74(healthbox);
                ov12_02264EB4(commandSetData->healthbox);
                ov12_02262014(battlerData);
                NARC_Delete(bgNarc);
                NARC_Delete(objNarc);
            } break;
            case 4: {
                NARC *bgNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
                NARC *objNarc = NARC_New((enum NarcId)8, HEAP_ID_BATTLE);

                if (BattleInput_GetCancelRunFlag(btlSubscreen) == 1) {
                    ov12_02265D74(healthbox);
                    commandSetData->input = 0xFF;
                }

                BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 0, 0, 0L);
                NARC_Delete(bgNarc);
                NARC_Delete(objNarc);
            } break;
            default:
                ((0) ? (void)0 : GF_AssertFail());
                break;
            }
            commandSetData->state = 8;
        }
        break;
    case 7:
        if (BattleInput_CheckFeedbackDone(btlSubscreen) == 1) {
            NARC *bgNarc = NARC_New(NARC_a_0_0_7, HEAP_ID_BATTLE);
            NARC *objNarc = NARC_New((enum NarcId)8, HEAP_ID_BATTLE);

            BattleInput_ChangeMenu(bgNarc, objNarc, btlSubscreen, 0, 0, 0L);
            BattleInput_Deadstriped_022698AC(btlSubscreen, 0);
            ov12_02265D74(healthbox);
            ov12_02264EB4(commandSetData->healthbox);
            ov12_02262014(battlerData);
            BattleInput_DisableBallGauge(btlSubscreen);

            commandSetData->state = 8;

            NARC_Delete(bgNarc);
            NARC_Delete(objNarc);
        }
        break;
    case 8:
        if (ov12_022698B0(btlSubscreen) == 1) {
            BattleController_EmitSelectedCommand(commandSetData->battleSys, commandSetData->battler, commandSetData->input);
            BattleController_EmitClearCommand(commandSetData->battleSys, commandSetData->battler, commandSetData->command);
            Heap_Free(data);
            SysTask_Destroy(task);
        }
        break;
    }
}
