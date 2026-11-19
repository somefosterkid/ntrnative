#include "global.h"

#include "battle/battler_sprite_context.h"
#include "sprite_resources_header.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "overlay_manager.h"
#include "struct_defs/resdat_narc_structs.h"
#include "constants/battle.h"
#include "constants/pokemon.h"
#include "game_stats.h"
#include "battle/battle.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "overlay_41_sprite_resource_collection.h"
#include "overlay_12_field_battle_dto.h"
#include "battle/trainer_intro_data.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_93_unk_struct_ov117_02261280_t.h"
#include "battle/party_gauge.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "overlay_07_bg_scroll_context.h"
#include "overlay_07_battle_anim_system.h"
#include "render_window.h"
#include "overlay_87_scratch_off_card_app.h"
#include "overlay_55.h"
#include "overlay_12_menu_task_data.h"
#include "overlay_07_battle_anim_context.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_anim_tracking.h"

struct AIContext;
struct ActionSelectData;
struct Background;
struct Bag;
struct BagCursor;
struct BagItem;
struct BallCapsule;
struct UnkBallData;
struct BallSeal;
struct BattleAnimContext;
struct BattleAnimScriptLoop;
struct BattleAnimSystem;
struct BattleBackgroundReference;
struct BattleBagCursor;
struct BattleBgAnim;
struct BattleContext;
struct BattleMessage;
struct BattleMon;
struct BattleMonOBJData;
struct BattleRecords;
struct BattleScriptTaskData;
struct BattleSubscreen;
struct BattleSystem;
struct OpponentData;
struct OpponentDataSysTaskFuncs;
struct BgConfig;
struct BgTrackingTask;
struct BgWaveStrip;
struct BoxPokemon;
struct Camera;
struct CameraAngle;
struct CameraLookAt;
struct CameraPerspective;
struct CameraPositionHistory;
struct CaptureAttempt;
struct CharTransferAllocation;
struct SOUND_CHATOT;
struct CoordinatesS16;
struct BattleCursor;
struct CustomBgScrollContext;
struct MailMessage;
struct EncodingSeed;
struct FieldBagCursor;
struct FieldBattleDTO;
struct FieldConditions;
struct TextOBJ;
struct FontOAMEntry;
struct MessagePrinter;
struct G2dRenderer;
struct G3DPipelineBuffers;
struct GameStats;
struct GiveUpMoveData;
struct BattleHpBar;
struct Indicator;
struct ItemData;
struct ItemPartyParam;
struct JournalEntry;
struct JournalEntryMon;
struct JournalEntryTitle;
struct JournalEntryTrainer;
struct Mail;
struct ManagedSprite;
struct MenuButtonPressAnimState;
struct MenuCursor;
struct MessageBank;
struct MessageBankEntry;
struct MsgData;
struct MoveContestData;
struct MoveDisplayData;
struct MoveDisplayInfo;
struct MoveEffectsData;
struct MoveFailFlags;
struct MoveSelectData;
struct MoveTable;
struct NARC;
struct Options;
struct PCBoxes;
struct SavePalPad;
struct PaletteAnimator;
struct PaletteBuffer;
struct PaletteData;
struct SelectedPaletteData;
struct ParticleSystem;
struct Party;
struct PartyBallAnimState;
struct PartyGauge;
struct PartyPokemon;
struct Pokedex;
struct Pokemon;
struct PokemonAnim;
struct PokemonAnimManager;
struct PokemonDataBlockA;
struct PokemonDataBlockB;
struct PokemonDataBlockC;
struct PokemonDataBlockD;
struct Pokepic;
struct PokemonSpriteDrawContext;
struct PokepicManager;
struct PokepicShadow;
struct PokepicTemplate;
struct PokemonSpriteTrackingTask;
struct PokepicDrawParam;
struct Poketch;
struct GF_2DGfxRawResObj;
struct GF_2DGfxRawResMan;
struct SPLAlphaAnim;
struct SPLBehavior;
struct SPLChildResource;
struct SPLColorAnim;
struct SPLEmitter;
struct SPLEmitterList;
struct SPLEmitterState;
struct SPLManager;
struct SPLParticle;
struct SPLParticleList;
struct SPLResource;
struct SPLResourceHeader;
struct SPLScaleAnim;
struct SPLTexAnim;
struct SPLTexture;
struct SaveBlockInfo;
struct SaveData;
struct SaveDataBody;
struct SaveDataState;
struct SavePageInfo;
struct OpponentDataCursorPos;
struct SelfTurnFlags;
struct SideConditions;
struct Sprite;
struct SpriteAnimFrame;
struct SpriteList;
struct SpriteManager;
struct SpriteResource;
struct SpriteResourceCollection;
struct GF_2DGfxResObjList;
struct GF_2DGfxResHeader;
struct SpriteResourcesHeader;
struct SpriteResourcesHeaderList;
struct SpriteSystem;
struct SpriteSystemContext;
struct ManagedSpriteTemplate;
struct SpriteTrackingTask;
struct String;
struct MessageFormat;
struct StringTemplateArg;
struct StringTemplateArgHeader;
struct SubscreenTilemapRect;
struct SysTask;
struct SysTaskQueue;
struct TargetMonData;
struct TargetSelectData;
struct TextWindowLayout;
struct Trainer;
struct TrainerData;
struct PlayerProfile;
struct TrainerIntroData;
struct TransformData;
struct TurnFlags;
struct UnkStruct_02012744_t;
struct UnkStruct_02012B20_t;
struct WaitingIcon;
struct WaveAnimContext;
struct WiFiHistory;
struct Window;
enum BattleBackground {
    BACKGROUND_PLAIN = 0,
    BACKGROUND_WATER,
    BACKGROUND_CITY,
    BACKGROUND_FOREST,
    BACKGROUND_MOUNTAIN,
    BACKGROUND_SNOW,
    BACKGROUND_INDOORS_1,
    BACKGROUND_INDOORS_2,
    BACKGROUND_INDOORS_3,
    BACKGROUND_CAVE_1,
    BACKGROUND_CAVE_2,
    BACKGROUND_CAVE_3,
    BACKGROUND_AARON,
    BACKGROUND_BERTHA,
    BACKGROUND_FLINT,
    BACKGROUND_LUCIAN,
    BACKGROUND_CYNTHIA,
    BACKGROUND_DISTORTION_WORLD,
    BACKGROUND_BATTLE_TOWER,
    BACKGROUND_BATTLE_FACTORY,
    BACKGROUND_BATTLE_ARCADE,
    BACKGROUND_BATTLE_CASTLE,
    BACKGROUND_BATTLE_HALL,
};
enum BattleAction {
    BATTLE_ACTION_PICK_COMMAND = 0,
    BATTLE_ACTION_CHOOSE_TARGET = 1,
    BATTLE_ACTION_TEMP_VALUE = 2,
    BATTLE_ACTION_SELECTED_COMMAND = 3,
    MAX_BATTLE_ACTIONS = 4,
};
typedef u16 charcode_t;
typedef struct WiFiHistory {
    u32 unused_00;
    u8 hasInteractedOutsideOfJapan;
    u8 country;
    u8 region;
    u8 geonetCommunicationMap[4080];
} WiFiHistory;
enum ItemBattleCategory {
    ITEM_BATTLE_CATEGORY_RECOVER_HP = 0,
    ITEM_BATTLE_CATEGORY_RECOVER_STATUS = 1,
    ITEM_BATTLE_CATEGORY_POKE_BALLS = 2,
    ITEM_BATTLE_CATEGORY_BATTLE_ITEMS = 3,
    ITEM_BATTLE_CATEGORY_MAX = 4,
};
enum SaveBlockID {
    SAVE_BLOCK_ID_NORMAL = 0,
    SAVE_BLOCK_ID_BOXES,

    SAVE_BLOCK_ID_MAX
};
typedef struct SaveBlockInfo {
    u8 saveBlockID;
    u8 sectorStartPos;
    u8 sectorsInUse;
    u32 offset;
    u32 size;
} SaveBlockInfo;
typedef struct SavePageInfo {
    int pageID;
    u32 size;
    u32 location;
    u16 checksum;
    u16 blockID;
} SavePageInfo;
typedef struct SaveDataBody {
    u8 data[(0x1000) * 32];
} SaveDataBody;
typedef struct SaveDataState {
    BOOL fullSaveMode;
    int startBlock;
    int currentBlock;
    int endBlock;
    s32 lockID;
    int mainSequence;
    u32 globalCounterBackup;
    u32 blockCounterBackup[SAVE_BLOCK_ID_MAX];
    volatile BOOL locked;
} SaveDataState;
typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;
typedef struct FieldBagCursor {
    u8 scroll[8];
    u8 index[8];
    u16 pocket;
    u16 dummy12;
} FieldBagCursor;
typedef struct BattleBagCursor {
    u8 scroll[ITEM_BATTLE_CATEGORY_MAX + 1];
    u8 index[ITEM_BATTLE_CATEGORY_MAX + 1];
    u16 lastUsedItemID;
    u16 lastUsedCategory;
    u16 currentCategory;
} BattleBagCursor;
typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
enum TaskState {
    TASK_STATE_ACTIVE = 0,
    TASK_STATE_INACTIVE,
};
typedef struct PokemonAnimManager {
    PokemonAnim *anims;
    enum HeapID heapID;
    u8 flipSprite;
    u8 animCount;
} PokemonAnimManager;
typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;
typedef struct SpriteResourcesHeaderList {
    SpriteResourcesHeader *headers;
    int length;
} SpriteResourcesHeaderList;
typedef struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
} EncodingSeed;
typedef struct JournalEntryTitle {
    u32 year : 7;
    u32 month : 4;
    u32 week : 3;
    u32 day : 5;
    u32 mapID : 13;
} JournalEntryTitle;
typedef struct JournalEntryMon {
    u8 battleResult;
    u8 stringVariant : 2;
    u8 timeOfDay : 4;
    u8 gender : 2;
    u16 species;
} JournalEntryMon;
typedef struct JournalEntryTrainer {
    u16 standard : 1;
    u16 trainerID : 15;
    u16 mapID;
} JournalEntryTrainer;
typedef struct JournalEntry {
    JournalEntryTitle title;
    u32 locationEvents[4];
    JournalEntryMon mon;
    JournalEntryTrainer trainer;
    u8 onlineEvents[2][42];
} JournalEntry;
typedef struct PCBoxes {
    u32 currentBoxID;
    BoxPokemon boxMons[18][(5 * 6)];
    u16 names[18][20];
    u8 wallpapers[18];
    u8 unlockedWallpapers;
} PCBoxes;
typedef struct Poketch {
    u8 poketchEnabled : 1;
    u8 pedometerEnabled : 1;
    u8 dotArtModifiedByPlayer : 1;
    u8 screenColor : 3;

    s8 appCount;
    s8 appIndex;
    u8 appRegistry[32];

    u32 stepCount;

    u16 alarmSet : 1;
    u16 alarmHour : 5;
    u16 alarmMinute : 6;

    u8 dotArtData[120];

    u32 calendarMarkBitmap;
    u8 calendarMonth;

    struct {
        u8 x;
        u8 y;
    } markMapPositions[6];

    struct {
        u16 species;
        u16 icon;
        u32 form;
    } pokemonHistoryQueue[12];
} Poketch;
enum TimeOfDay {
    TIMEOFDAY_MORNING = 0,
    TIMEOFDAY_DAY = 1,
    TIMEOFDAY_TWILIGHT = 2,
    TIMEOFDAY_NIGHT = 3,
    TIMEOFDAY_LATE_NIGHT = 4,
};
typedef struct BattleRecords {
    int totalTurns;
    int totalFainted;
    int totalDamage;
} BattleRecords;
typedef struct CaptureAttempt {
    int resultMask;
    u16 ballsThrown;
    u16 species;
    u8 gender;
    u8 language;
    u8 metGame;
    u8 hasNickname;
    u16 nickname[11];
    u16 pokeballItemID;
} CaptureAttempt;
enum BattleCommRecipient {
    COMM_RECIPIENT_SERVER,
    COMM_RECIPIENT_CLIENT,
    COMM_RECIPIENT_QUEUE,
};
typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;
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
typedef struct MoveContestData {
    u8 effect;
    u8 type;
    u8 padding02[2];
} MoveContestData;
typedef struct MoveTable {
    u16 effect;
    u8 class;
    u8 power;
    u8 type;
    u8 accuracy;
    u8 pp;
    u8 effectChance;
    u16 range;
    s8 priority;
    u8 flags;
    MoveContestData contest;
} MoveTable;
typedef struct MoveEffectsData {
    u32 disabledTurns : 3;
    u32 encoredTurns : 3;
    u32 chargedTurns : 2;
    u32 tauntedTurns : 3;
    u32 protectSuccessTurns : 2;
    u32 perishSongTurns : 2;
    u32 rolloutCount : 3;
    u32 furyCutterCount : 3;
    u32 stockpileCount : 3;
    u32 stockpileDefBoosts : 3;
    u32 stockpileSpDefBoosts : 3;
    u32 truant : 1;
    u32 flashFire : 1;

    u32 lockOnTarget : 2;
    u32 mimickedMoveSlot : 4;
    u32 bindTarget : 2;
    u32 meanLookTarget : 2;
    u32 lastResortCount : 3;
    u32 magnetRiseTurns : 3;
    u32 healBlockTurns : 3;
    u32 embargoTurns : 3;
    u32 canUnburden : 1;
    u32 metronomeTurns : 4;
    u32 micleBerry : 1;
    u32 custapBerry : 1;
    u32 quickClaw : 1;
    u32 meFirst : 1;
    u32 padding0004_1F : 1;

    int rechargeTurnNumber;
    int fakeOutTurnNumber;
    int slowStartTurnNumber;
    int meFirstTurnNumber;
    int substituteHP;
    u32 transformedPID;

    u16 disabledMove;
    u16 bindingMove;
    u16 encoredMove;
    u16 encoredMoveSlot;
    u16 lastResortMoves[4];
    u16 choiceLockedMove;
    u16 transformedGender;

    int itemHPRecovery;
} MoveEffectsData;
typedef struct FieldConditions {
    u32 weatherTurns;
    u8 futureSightTurns[4];
    u8 wishTurns[4];
    u16 futureSightMove[4];
    int futureSightAttacker[4];
    s32 futureSightDamage[4];
    u8 wishTarget[4];
} FieldConditions;
typedef struct SideConditions {
    u32 reflectUser : 2;
    u32 reflectTurns : 3;
    u32 lightScreenUser : 2;
    u32 lightScreenTurns : 3;
    u32 mistUser : 2;
    u32 mistTurns : 3;
    u32 safeguardUser : 2;
    u32 safeguardTurns : 3;
    u32 followMe : 1;
    u32 followMeUser : 2;
    u32 knockedOffItemsMask : 6;
    u32 padding00_1D : 3;

    u32 spikesLayers : 2;
    u32 toxicSpikesLayers : 2;
    u32 padding04_04 : 28;
} SideConditions;
typedef struct TurnFlags {
    u32 struggling : 1;
    u32 ppDecremented : 1;
    u32 protecting : 1;
    u32 helpingHand : 1;
    u32 magicCoat : 1;
    u32 snatching : 1;
    u32 roosting : 1;
    u32 fleeing : 2;
    u32 enduring : 1;
    u32 padding00_0A : 22;

    int physicalDamageTakenFrom[4];
    int physicalDamageLastAttacker;
    int physicalDamageAttackerMask;

    int specialDamageTakenFrom[4];
    int specialDamageLastAttacker;
    int specialDamageAttackerMask;

    int lastDamageTaken;
    int lastAttacker;
    int assuranceDamageMask;
} TurnFlags;
typedef struct SelfTurnFlags {
    u32 skipPressureCheck : 1;
    u32 lightningRodActivated : 1;
    u32 stormDrainActivated : 1;
    u32 moldBreakerActivated : 1;
    u32 trickRoomActivated : 1;
    u32 focusItemActivated : 1;
    u32 repeatedMoveCount : 3;
    u32 padding00_09 : 23;

    int physicalDamageTaken;
    int physicalDamageLastAttacker;
    int specialDamageTaken;
    int specialDamageLastAttacker;
    int statusFlags;
    int shellBellDamageDealt;
} SelfTurnFlags;
typedef struct BattleScriptTaskData {
    BattleSystem *battleSys;
    BattleContext *battleCtx;
    BallRotation *ballRotation;
    ManagedSprite *sprites[2];
    TextOBJ *fontOAM;
    CharTransferAllocation charTransferAllocation;
    int flag;
    int seqNum;
    int ball;
    int tmpData[8];
    void *tmpPtr[2];
} BattleScriptTaskData;
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
enum PartyGaugeSide {
    PARTY_GAUGE_OURS,
    PARTY_GAUGE_THEIRS,
    PARTY_GAUGE_COUNT
};
enum HidePartyGaugeType {
    HIDE_PARTY_GAUGE_BATTLE_START,
    HIDE_PARTY_GAUGE_MID_BATTLE,
};
enum PartyGaugePosition {
    PARTY_GAUGE_POSITION_HIGH,
    PARTY_GAUGE_POSITION_MIDDLE,
    PARTY_GAUGE_POSITION_LOW,
};
enum HideArrowType {
    HIDE_ARROW_FADE_AND_SCROLL,
    HIDE_ARROW_FADE_IN_PLACE,
};
typedef struct BattleBackgroundReference {
    int narcID;
    int tilesNarcMemberIdx;
    int paletteNarcMemberIdx;
    int tilemapNarcMemberIdx;
    int paletteDestStart;
    int paletteSrcSize;
} BattleBackgroundReference;
typedef struct CameraPositionHistory {
    int historySize;
    int currentHistoryIndex;
    int nextHistoryIndex;
    int delay;
    BOOL delayReached;
    BOOL delayX;
    BOOL delayY;
    BOOL delayZ;
    VecFx32 *positions;
} CameraPositionHistory;
typedef struct SPLBehavior {
    void (*applyFunc)(const void *, struct SPLParticle *, VecFx32 *, struct SPLEmitter *);
    const void *object;
} SPLBehavior;
typedef union SPLResourceFlags {
    u32 all;
    struct {
        u32 emissionType : 4;
        u32 drawType : 2;
        u32 circleAxis : 2;
        u32 hasScaleAnim : 1;
        u32 hasColorAnim : 1;
        u32 hasAlphaAnim : 1;
        u32 hasTexAnim : 1;
        u32 hasRotation : 1;
        u32 randomInitAngle : 1;

        u32 selfMaintaining : 1;
        u32 followEmitter : 1;
        u32 hasChildResource : 1;
        u32 polygonRotAxis : 2;
        u32 polygonReferencePlane : 1;
        u32 randomizeLoopedAnim : 1;
        u32 drawChildrenFirst : 1;
        u32 hideParent : 1;
        u32 useViewSpace : 1;
        u32 hasGravityBehavior : 1;
        u32 hasRandomBehavior : 1;
        u32 hasMagnetBehavior : 1;
        u32 hasSpinBehavior : 1;
        u32 hasCollisionPlaneBehavior : 1;
        u32 hasConvergenceBehavior : 1;
        u32 hasFixedPolygonID : 1;
        u32 childHasFixedPolygonID : 1;
    };
} SPLResourceFlags;
typedef union SPLChildResourceFlags {
    u16 all;
    struct {
        u16 usesBehaviors : 1;
        u16 hasScaleAnim : 1;
        u16 hasAlphaAnim : 1;
        u16 rotationType : 2;
        u16 followEmitter : 1;
        u16 useChildColor : 1;
        u16 drawType : 2;
        u16 polygonRotAxis : 2;
        u16 polygonReferencePlane : 1;
        u16 : 4;
    };
} SPLChildResourceFlags;
typedef union SPLCurveInOut {
    u16 all;
    struct {
        u16 in : 8;
        u16 out : 8;
    };
} SPLCurveInOut;
typedef union SPLCurveInPeakOut {
    u32 all;
    struct {
        u32 in : 8;
        u32 peak : 8;
        u32 out : 8;
        u32 : 8;
    };
} SPLCurveInPeakOut;
typedef struct SPLResourceHeader {
    SPLResourceFlags flags;
    VecFx32 emitterBasePos;
    fx32 emissionCount;
    fx32 radius;
    fx32 length;
    VecFx16 axis;
    GXRgb color;
    fx32 initVelPosAmplifier;
    fx32 initVelAxisAmplifier;
    fx32 baseScale;
    fx16 aspectRatio;
    u16 startDelay;
    s16 minRotation;
    s16 maxRotation;
    u16 initAngle;
    u16 reserved;
    u16 emitterLifeTime;
    u16 particleLifeTime;

    struct {
        u32 baseScale : 8;
        u32 lifeTime : 8;
        u32 initVel : 8;
        u32 : 8;
    } randomAttenuation;

    struct {
        u32 emissionInterval : 8;
        u32 baseAlpha : 8;
        u32 airResistance : 8;
        u32 textureIndex : 8;
        u32 loopFrames : 8;
        u32 dbbScale : 16;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 scaleAnimDir : 3;
        u32 dpolFaceEmitter : 1;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 : 30;
    } misc;
    fx16 polygonX;
    fx16 polygonY;
    struct {
        u32 flags : 8;
        u32 : 24;
    } userData;
} SPLResourceHeader;
typedef struct SPLScaleAnim {
    fx16 start;
    fx16 mid;
    fx16 end;
    SPLCurveInOut curve;
    struct {
        u16 loop : 1;
        u16 : 15;
    } flags;
    u16 padding;
} SPLScaleAnim;
typedef struct SPLColorAnim {
    GXRgb start;
    GXRgb end;
    SPLCurveInPeakOut curve;
    struct {
        u16 randomStartColor : 1;
        u16 loop : 1;
        u16 interpolate : 1;
        u16 : 13;
    } flags;
    u16 padding;
} SPLColorAnim;
typedef struct SPLAlphaAnim {
    union {
        u16 all;
        struct {
            u16 start : 5;
            u16 mid : 5;
            u16 end : 5;
            u16 : 1;
        };
    } alpha;
    struct {
        u16 randomRange : 8;
        u16 loop : 1;
        u16 : 7;
    } flags;
    SPLCurveInOut curve;
    u16 padding;
} SPLAlphaAnim;
typedef struct SPLTexAnim {
    u8 textures[8];
    struct {
        u32 frameCount : 8;
        u32 step : 8;
        u32 randomizeInit : 1;
        u32 loop : 1;
        u32 : 14;
    } param;
} SPLTexAnim;
typedef struct SPLChildResource {
    SPLChildResourceFlags flags;
    fx16 randomInitVelMag;
    fx16 endScale;
    u16 lifeTime;
    u8 velocityRatio;
    u8 scaleRatio;
    GXRgb color;
    struct {
        u32 emissionCount : 8;
        u32 emissionDelay : 8;
        u32 emissionInterval : 8;
        u32 texture : 8;
        u32 textureTileCountS : 2;
        u32 textureTileCountT : 2;
        u32 flipTextureS : 1;
        u32 flipTextureT : 1;
        u32 dpolFaceEmitter : 1;
        u32 : 25;
    } misc;
} SPLChildResource;
enum SPLUpdateCallbackType {
    SPL_CALLBACK_PRE_UPDATE = 0,
    SPL_CALLBACK_POST_UPDATE = 1,
};
typedef void (*SPLEmitterUpdateCallback)(struct SPLEmitter *emitter, enum SPLUpdateCallbackType type);
typedef struct SPLEmitterState {
    union {
        u32 all;
        struct
        {
            u32 terminate : 1;
            u32 emissionPaused : 1;
            u32 paused : 1;
            u32 renderingDisabled : 1;
            u32 started : 1;
            u32 : 27;
        };
    };
} SPLEmitterState;
typedef union SPLTextureParam {
    u32 all;
    struct {
        u32 format : 4;
        u32 s : 4;
        u32 t : 4;
        u32 repeat : 2;
        u32 flip : 2;
        u32 palColor0 : 1;
        u32 useSharedTexture : 1;
        u32 sharedTexID : 8;
        u32 : 6;
    };
} SPLTextureParam;
typedef void *(*SPLAllocFunc)(u32);
enum BattleAnimSystemArc {
    BATTLE_ANIM_SYSTEM_ARC_BATT_BG = 0,
    BATTLE_ANIM_SYSTEM_ARC_BATT_OBJ,
    BATTLE_ANIM_SYSTEM_ARC_WECHAR,
    BATTLE_ANIM_SYSTEM_ARC_WEPLTT,
    BATTLE_ANIM_SYSTEM_ARC_WECELL,
    BATTLE_ANIM_SYSTEM_ARC_WECELLANM,

    BATTLE_ANIM_SYSTEM_ARC_COUNT,
};
typedef struct BattleAnimScriptLoop {
    u32 *startPos;
    u8 curIteration;
    u8 maxIterations;
    BOOL isActive;
} BattleAnimScriptLoop;
typedef struct PokemonSpriteDrawContext {
    struct BattleAnimSystem *battleAnimSys;
    SpriteManager *spriteManager;
    ManagedSprite *sprite;
    BOOL active;
} PokemonSpriteDrawContext;
typedef struct G3DPipelineBuffers {
    enum HeapID heapID;
    void *paletteBuffer;
    void *textureBuffer;
} G3DPipelineBuffers;
typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;
typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;
typedef struct StringTemplateArgHeader {
    u8 unused_00;
    u8 unused_01;
    u8 unused_02;
    u8 unused_03;
} StringTemplateArgHeader;
typedef struct StringTemplateArg {
    StringTemplateArgHeader header;
    String *string;
} StringTemplateArg;
typedef struct PaletteAnimator PaletteAnimator;
typedef void (*UnkFuncPtr_020157E4)(PaletteAnimator *, u16 *);
struct PaletteAnimator {
    SysTask *unk_00;
    UnkFuncPtr_020157E4 unk_04;
    PaletteData *unk_08;
    u16 unk_0C[16];
    u16 unk_2C[16];
    u8 unk_4C;
    u8 unk_4D;
    u8 unk_4E;
    u8 unk_4F;
};
void BattleSystem_TryRecvMessage(BattleSystem *battleSys, int recipient);
BOOL BattleContext_Main(BattleSystem *battleSys, BattleContext *battleCtx);
void BattleSystem_ExecuteBattlerCommand(BattleSystem *battleSys, OpponentData *battlerData);

int ov12_02238358(OverlayManager *appMan);

int ov12_02238358(OverlayManager *appMan) {
    BattleSystem *battleSys = OverlayManager_GetData(appMan);
    int battler;

    if (battleSys->battleType & (1 << 2) && (battleSys->battleSpecial & (1 << 4)) == 0) {
        if (battleSys->unk23FC) {
            BattleContext_Main(battleSys, battleSys->ctx);
        }

        for (battler = 0; battler < battleSys->maxBattlers; battler++) {
            BattleSystem_ExecuteBattlerCommand(battleSys, battleSys->opponentData[battler]);
        }
    } else {
        if (battleSys->unk23FC) {
            battleSys->unk23FE = BattleContext_Main(battleSys, battleSys->ctx);
            BattleSystem_TryRecvMessage(battleSys, COMM_RECIPIENT_CLIENT);
        }

        for (battler = 0; battler < battleSys->maxBattlers; battler++) {
            BattleSystem_ExecuteBattlerCommand(battleSys, battleSys->opponentData[battler]);
            BattleSystem_TryRecvMessage(battleSys, COMM_RECIPIENT_SERVER);
        }

        if (battleSys->unk23FE == 0) {
            if (battleSys->unk23FC) {
                battleSys->unk23FE = BattleContext_Main(battleSys, battleSys->ctx);
                BattleSystem_TryRecvMessage(battleSys, COMM_RECIPIENT_CLIENT);
            }

            for (battler = 0; battler < battleSys->maxBattlers; battler++) {
                BattleSystem_ExecuteBattlerCommand(battleSys, battleSys->opponentData[battler]);
                BattleSystem_TryRecvMessage(battleSys, COMM_RECIPIENT_SERVER);
            }
        }
    }

    return battleSys->unk23FE;
}
