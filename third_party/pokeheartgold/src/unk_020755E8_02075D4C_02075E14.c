#include "global.h"
#include "constants/heap.h"
#include "gf_gfx_planes.h"
#include "palette.h"
#include "bg_window.h"
#include "egg_hatch_particle_system.h"
#include "evolution_data.h"
#include "pokemon_summary_args.h"
#include "pokepic_anim_script.h"
#include "unk_02016EDC.h"
#include "unk_020163E0.h"
#include "camera.h"
#include "library/spl_particle.h"
#include "library/spl_resource.h"
#include "library/spl_emitter.h"
#include "library/spl_texture.h"
#include "library/spl_manager.h"
#include "particle_system.h"
#include "pm_string.h"
#include "options.h"
#include "overlay_55.h"
#include "transform_data.h"
#include "pokemon_anim.h"
#include "bag_types_def.h"
#include "list_menu_items.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct Bag;

struct BagItem;

struct BallCapsule;

struct BallSeal;

struct BgConfig;

struct BoxPokemon;

struct Camera;

struct CameraAngle;

struct CameraLookAt;

struct CameraPerspectiveProjection;

struct CameraPositionHistory;

struct ChatotCry;

struct ListMenuCursor;

struct MailMessage;

struct EncodingSeed;


struct GameStats;

struct Mail;

struct ListMenu2D;

struct ListMenu2DTemplate;

struct MessageBank;

struct MessageBankEntry;

struct MsgData;

struct NARC;

struct Options;

struct PaletteData;

struct PaletteFadeControl;

struct ParticleSystem;

struct Party;

struct PartyPokemon;

struct Pokedex;

struct Pokemon;

struct PokemonAnim;

struct PokemonDataBlockA;

struct PokemonDataBlockB;

struct PokemonDataBlockC;

struct PokemonDataBlockD;

struct Pokepic;

struct PokepicManager;

struct PokemonSpriteShadow;

struct PokepicTemplate;

struct PokemonSpriteTransforms;

struct Poketch;

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

struct String;

struct ListMenuItem;

struct MessageFormat;

struct StringTemplateArg;

struct StringTemplateArgHeader;

struct SysTask;

struct SysTaskManager;

struct TransformData;

struct Window;

struct WindowTemplate;

typedef struct CameraPerspectiveProjection {
    fx32 sinFovY;
    fx32 cosFovY;
    fx32 aspectRatio;
    fx32 nearClip;
    fx32 farClip;
} CameraPerspectiveProjection;

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


typedef struct ChatotCry {
    BOOL valid;
    s8 audioData[1000];
} ChatotCry;

typedef u16 charcode_t;

typedef struct MessageBankEntry {
    u32 offset;
    u32 length;
} MessageBankEntry;

typedef struct MessageBank {
    u16 count;
    u16 seed;

    MessageBankEntry entries[];
} MessageBank;



typedef struct BallSeal {
    u8 type;
    u8 x;
    u8 y;
} BallSeal;

typedef struct PaletteFadeControl {
    u16 unfadedMask;
    u16 wait : 6;
    u16 cur : 5;
    u16 end : 5;
    u16 target : 15;
    u16 sign : 1;
    u16 step : 4;
    u16 waitStep : 6;
    u16 unused : 6;
} PaletteFadeControl;

u8 PaletteData_BeginPaletteFade(PaletteData *paletteData, u16 buffersToFade, u16 palettesToFade, s8 wait, u8 cur, u8 end, u16 target);

u16 PaletteData_GetSelectedBuffersBitmask(PaletteData *paletteData);

enum PokemonSpriteAttribute {
    MON_SPRITE_X_CENTER = 0,
    MON_SPRITE_Y_CENTER = 1,
    MON_SPRITE_Z_CENTER = 2,
    MON_SPRITE_X_OFFSET = 3,
    MON_SPRITE_Y_OFFSET = 4,
    MON_SPRITE_Z_OFFSET = 5,
    MON_SPRITE_HIDE = 6,
    MON_SPRITE_ROTATION_X = 7,
    MON_SPRITE_ROTATION_Y = 8,
    MON_SPRITE_ROTATION_Z = 9,
    MON_SPRITE_X_PIVOT = 10,
    MON_SPRITE_Y_PIVOT = 11,
    MON_SPRITE_SCALE_X = 12,
    MON_SPRITE_SCALE_Y = 13,
    MON_SPRITE_PARTIAL_DRAW = 14,
    MON_SPRITE_DRAW_X_OFFSET = 15,
    MON_SPRITE_DRAW_Y_OFFSET = 16,
    MON_SPRITE_DRAW_WIDTH = 17,
    MON_SPRITE_DRAW_HEIGHT = 18,
    MON_SPRITE_SHADOW_X = 19,
    MON_SPRITE_SHADOW_Y = 20,
    MON_SPRITE_SHADOW_X_OFFSET = 21,
    MON_SPRITE_SHADOW_Y_OFFSET = 22,
    MON_SPRITE_ALPHA = 23,
    MON_SPRITE_DIFFUSE_R = 24,
    MON_SPRITE_DIFFUSE_G = 25,
    MON_SPRITE_DIFFUSE_B = 26,
    MON_SPRITE_AMBIENT_R = 27,
    MON_SPRITE_AMBIENT_G = 28,
    MON_SPRITE_AMBIENT_B = 29,
    MON_SPRITE_FADE_ACTIVE = 30,
    MON_SPRITE_FADE_TARGET_COLOR = 31,
    MON_SPRITE_FADE_INIT_ALPHA = 32,
    MON_SPRITE_FADE_TARGET_ALPHA = 33,
    MON_SPRITE_FADE_DELAY_COUNTER = 34,
    MON_SPRITE_FLIP_H = 35,
    MON_SPRITE_FLIP_V = 36,
    MON_SPRITE_HIDE_2 = 37,
    MON_SPRITE_CURR_SPRITE_FRAME = 38,
    MON_SPRITE_DUMMY = 39,
    MON_SPRITE_MOSAIC_INTENSITY = 40,
    MON_SPRITE_SHADOW_HEIGHT = 41,
    MON_SPRITE_SHADOW_PLTT_SLOT = 42,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_X = 43,
    MON_SPRITE_SHADOW_SHOULD_FOLLOW_Y = 44,
    MON_SPRITE_SHADOW_IS_AFFINE = 45,
    MON_SPRITE_SHADOW_SIZE = 46,
};

typedef struct PokemonSpriteTransforms PokemonSpriteTransforms;

typedef void(PokemonSpriteCallback)(Pokepic *, PokemonSpriteTransforms *);

struct PokemonSpriteTransforms {
    s16 xCenter;
    s16 yCenter;
    int zCenter;
    s16 xOffset;
    s16 yOffset;
    int zOffset;
    s16 scaleX;
    s16 scaleY;
    u16 rotationX;
    u16 rotationY;
    u16 rotationZ;
    u16 padding_1A;
    s16 xPivot;
    s16 yPivot;
    u8 drawXOffset;
    u8 drawYOffset;
    u8 drawWidth;
    u8 drawHeight;
    u8 fadeInitAlpha;
    u8 fadeTargetAlpha;
    u8 fadeDelayCounter;
    u8 fadeDelayLength;
    u32 fadeTargetColor;
    u32 diffuseR : 5;
    u32 diffuseG : 5;
    u32 diffuseB : 5;
    u32 ambientR : 5;
    u32 ambientG : 5;
    u32 ambientB : 5;
    u32 padding_2C_30 : 2;
    u32 hide : 1;
    u32 partialDraw : 1;
    u32 alpha : 5;
    u32 padding_30_07 : 2;
    u32 flipH : 1;
    u32 flipV : 1;
    u32 hide2 : 1;
    u32 fadeActive : 1;
    u32 mosaicIntensity : 4;
    u32 padding_30_17 : 15;
};

typedef struct PokemonSpriteShadow {
    u16 plttSlot : 2;
    u16 shouldFollowX : 1;
    u16 shouldFollowY : 1;
    u16 isAffine : 1;
    u16 size : 2;
    u16 padding_00 : 9;
    s8 height;
    u8 padding_03;
    s16 x;
    s16 y;
    s16 xOffset;
    s16 yOffset;
} PokemonSpriteShadow;


void Pokepic_StartAnim(Pokepic *monSprite, int dummy);

void Pokepic_SetAnimScript(Pokepic *monSprite, PokepicAnimScript *animFrames);

BOOL Pokepic_IsAnimActive(Pokepic *monSprite);




void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);

void Pokepic_StartPaletteFadeAll(PokepicManager *monSpriteMan, int initAlpha, int targetAlpha, int delay, int color);

void Pokepic_ScheduleReloadFromNarc(Pokepic *monSprite);

typedef struct SysTaskManager SysTaskManager;

struct SysTaskManager {
    u16 maxTasks;
    u16 stackPointer;
    SysTask sentinelTask;
    SysTask **taskStack;
    SysTask *tasks;
    BOOL locked;
    SysTask *currentTask;
    SysTask *nextTask;
};

void CalcMonLevelAndStats(Pokemon *mon);



BoxPokemon *Mon_GetBoxMon(Pokemon *mon);

void MonSetMoveInSlot(Pokemon *mon, u16 moveID, u8 moveSlot);

u32 MonTryLearnMoveOnLevelUp(Pokemon *mon, int *index, u16 *moveID);

void UpdateMonAbility(Pokemon *mon);

void NARC_ReadPokepicAnimScript(NARC *narc, PokepicAnimScript *frames, u16 species, u16 clientType);

void sub_0207294C(NARC *narc, PokepicAnimManager *monAnimMan, Pokepic *sprite, u16 species, int face, int flipSprite, int frame);

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

void BufferBoxMonSpeciesName(MessageFormat *template, u32 idx, BoxPokemon *boxMon);

void BufferBoxMonNickname(MessageFormat *template, u32 idx, BoxPokemon *boxMon);


typedef u32 TextColor;

u8 TextPrinterCheckActive(u8 printerID);

enum SummaryMode {
    SUMMARY_MODE_NORMAL = 0,
    SUMMARY_MODE_LOCK_MOVES,
    SUMMARY_MODE_SELECT_MOVE,
    SUMMARY_MODE_FEED_POFFIN,
    SUMMARY_MODE_SHOW_CONDITION_CHANGE,
};

enum SummaryDataType {
    SUMMARY_DATA_MON = 0,
    SUMMARY_DATA_PARTY_MON,
    SUMMARY_DATA_BOX_MON,
};

typedef struct BagItem {
    u16 item;
    u16 quantity;
} BagItem;

void Pokedex_SetMonCaughtFlag(Pokedex *pokedex, Pokemon *mon);

typedef struct EncodingSeed {
    u16 byteSum;
    u16 modifier;
} EncodingSeed;

typedef struct GameStats {
    u32 recordsU32[71];
    u16 recordsU16[77];
    u16 padding;
    EncodingSeed seed;
} GameStats;

u32 GameStats_Inc(GameStats *records, int id);

void GameStats_AddScore(GameStats *records, int scoreID);

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

void OverlayManager_Delete(OverlayManager *appMan);

BOOL OverlayManager_Run(OverlayManager *appMan);

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

void Poketch_PokemonHistoryEnqueue(Poketch *poketch);

enum GameRecord {
    RECORD_STEPS = 0,
    RECORD_TRAINER_SCORE = 1,
    RECORD_UNK_002 = 2,
    RECORD_UNK_003 = 3,
    RECORD_BERRIES_PLANTED = 4,
    RECORD_UNK_005 = 5,
    RECORD_UNK_006 = 6,
    RECORD_WILD_BATTLES_FOUGHT = 7,
    RECORD_TRAINER_BATTLES_FOUGHT = 8,
    RECORD_CAUGHT_POKEMON = 9,
    RECORD_CAUGHT_FISH = 10,
    RECORD_EGGS_HATCHED = 11,
    RECORD_POKEMON_EVOLVED = 12,
    RECORD_UNK_013 = 13,
    RECORD_UNK_014 = 14,
    RECORD_BATTLE_TOWER_CHALLENGES = 15,
    RECORD_UNK_016 = 16,
    RECORD_UNK_017 = 17,
    RECORD_UNK_018 = 18,
    RECORD_LOCAL_LINK_TRADES = 19,
    RECORD_UNK_020 = 20,
    RECORD_LOCAL_LINK_BATTLE_WINS = 21,
    RECORD_LOCAL_LINK_BATTLE_LOSSES = 22,
    RECORD_UNK_023 = 23,
    RECORD_WIFI_TRADES = 24,
    RECORD_UNK_025 = 25,
    RECORD_WIFI_BATTLE_WINS = 26,
    RECORD_WIFI_BATTLE_LOSSES = 27,
    RECORD_UNK_028 = 28,
    RECORD_BATTLE_TOWER_VICTORIES = 29,
    RECORD_UNK_030 = 30,
    RECORD_UNK_031 = 31,
    RECORD_UNK_032 = 32,
    RECORD_UNK_033 = 33,
    RECORD_UNK_034 = 34,
    RECORD_MONEY_SPENT = 35,
    RECORD_UNK_036 = 36,
    RECORD_UNK_037 = 37,
    RECORD_UNK_038 = 38,
    RECORD_UNK_039 = 39,
    RECORD_DEPOSITED_IN_DAYCARE = 40,
    RECORD_FAINTED_IN_BATTLE = 41,
    RECORD_UNK_042 = 42,
    RECORD_UNK_043 = 43,
    RECORD_UNK_044 = 44,
    RECORD_TIMES_MAIL_WRITTEN = 45,
    RECORD_UNK_046 = 46,
    RECORD_SPHERES_BURIED = 47,
    RECORD_WATCHED_TV = 48,
    RECORD_POKEMON_NICKNAMED = 49,
    RECORD_UNK_050 = 50,
    RECORD_UNK_051 = 51,
    RECORD_UNK_052 = 52,
    RECORD_UNK_053 = 53,
    RECORD_TIMES_DRESSED_UP_POKEMON = 54,
    RECORD_UNK_055 = 55,
    RECORD_TIMES_STUCK_IN_DEEP_MUD = 56,
    RECORD_TIMES_BATTLED_AT_BATTLEGROUND = 57,
    RECORD_TIMES_STARTED_BATTLE_FRONTIER_CHALLENGE = 58,
    RECORD_UNK_059 = 59,
    RECORD_BATTLE_FACTORY_VICTORIES = 60,
    RECORD_BATTLE_CASTLE_VICTORIES = 61,
    RECORD_BATTLE_HALL_VICTORIES = 62,
    RECORD_BATTLE_ARCADE_VICTORIES = 63,
    RECORD_BATTLE_FACTORY_TRADES = 64,
    RECORD_CASTLE_POINTS_RECEIVED = 65,
    RECORD_UNK_066 = 66,
    RECORD_UNK_067 = 67,
    RECORD_BATTLE_POINTS_RECEIVED = 68,
    RECORD_BATTLE_POINTS_SPENT = 69,
    RECORD_STAR_PIECES_TRADED = 70,
    RECORD_UNK_071 = 71,
    RECORD_UNK_072 = 72,
    RECORD_TIMES_ENTERED_HALL_OF_FAME = 73,
    RECORD_UNK_074 = 74,
    RECORD_UNK_075 = 75,
    RECORD_USED_SPLASH = 76,
    RECORD_UNK_077 = 77,
    RECORD_USED_SELFDESTRUCT = 78,
    RECORD_USED_EXPLOSION = 79,
    RECORD_UNK_080 = 80,
    RECORD_UNK_081 = 81,
    RECORD_UNK_082 = 82,
    RECORD_UNK_083 = 83,
    RECORD_UNK_084 = 84,
    RECORD_UNK_085 = 85,
    RECORD_UNK_086 = 86,
    RECORD_UNK_087 = 87,
    RECORD_UNK_088 = 88,
    RECORD_UNK_089 = 89,
    RECORD_SUPER_CONTEST_PARTICIPATIONS = 90,
    RECORD_LINK_CONTEST_PARTICIPATIONS = 91,
    RECORD_OFFICIAL_CONTEST_WINS = 92,
    RECORD_LINK_CONTEST_WINS = 93,
    RECORD_RIBBONS_WON = 94,
    RECORD_USED_INEFFECTIVE_MOVE = 95,
    RECORD_FAINTED_ENEMY_MON = 96,
    RECORD_ATTACKED_ALLY = 97,
    RECORD_FAILED_ESCAPE = 98,
    RECORD_WILD_MON_FLED = 99,
    RECORD_FISH_GOT_AWAY = 100,
    RECORD_UNK_101 = 101,
    RECORD_UNK_102 = 102,
    RECORD_UNK_103 = 103,
    RECORD_UNK_104 = 104,
    RECORD_UNK_105 = 105,
    RECORD_UNK_106 = 106,
    RECORD_UNK_107 = 107,
    RECORD_UNK_108 = 108,
    RECORD_UNK_109 = 109,
    RECORD_UNK_110 = 110,
    RECORD_UNK_111 = 111,
    RECORD_UNK_112 = 112,
    RECORD_UNK_113 = 113,
    RECORD_TIMES_SIGNED_TRAINER_CARD = 114,
    RECORD_POKEMON_RECEIVED_FROM_FOSSIL_REVIVAL = 115,
    RECORD_CHECKED_FOOTPRINTS = 116,
    RECORD_HONEY_TREE_POKEMON_ENCOUNTERED = 117,
    RECORD_UNK_118 = 118,
    RECORD_UNK_119 = 119,
    RECORD_UNK_120 = 120,
    RECORD_UNK_121 = 121,
    RECORD_UNK_122 = 122,
    RECORD_UNK_123 = 123,
    RECORD_UNK_124 = 124,
    RECORD_UNK_125 = 125,
    RECORD_UNK_126 = 126,
    RECORD_UNK_127 = 127,
    RECORD_UNK_128 = 128,
    RECORD_UNK_129 = 129,
    RECORD_UNK_130 = 130,
    RECORD_UNK_131 = 131,
    RECORD_UNK_132 = 132,
    RECORD_UNK_133 = 133,
    RECORD_UNK_134 = 134,
    RECORD_UNK_135 = 135,
    RECORD_UNK_136 = 136,
    RECORD_UNK_137 = 137,
    RECORD_UNK_138 = 138,
    RECORD_UNK_139 = 139,
    RECORD_UNK_140 = 140,
    RECORD_UNK_141 = 141,
    RECORD_UNK_142 = 142,
    RECORD_UNK_143 = 143,
    RECORD_UNK_144 = 144,
    RECORD_UNK_145 = 145,
    RECORD_UNK_146 = 146,
    RECORD_UNK_147 = 147,
    MAX_RECORDS = 148,
};

typedef struct ListMenuCursor {
    TextColor color;
    String *string;
} ListMenuCursor;

typedef struct ListMenu2DTemplate {
    ListMenuItem *choices;
    Window *window;
    u8 fontID;
    u8 xSize;
    u8 ySize;
    u8 lineSpacing : 4;
    u8 suppressCursor : 2;
    u8 loopAround : 2;
} ListMenu2DTemplate;

typedef struct ListMenu2D {
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
} ListMenu2D;

ListMenu2D *Std_CreateYesNoMenu(BgConfig *bgConfig, const WindowTemplate *winTemplate, u16 borderTileStart, u8 borderPalette, u32 heapID);

u32 Handle2dMenuInput_DeleteOnFinish(ListMenu2D *menu, u32 heapID);

void sub_0203A880(void);

void DrawFrameAndWindow2(Window *window, s32 skipTransfer, u16 tile, u8 palette);

void SetMasterBrightnessNeutral(enum GFScreen screen);

void sub_0200FBF4(enum GFScreen screen, u16 color);

BOOL Sound_PlayBasicBGM(u16 bgmID);

void StopBGM(u16 bgmID, int fadeOutFrames);

BOOL PlaySE(u16 seqID);

BOOL Sound_PlayDelayedPokemonCry(u16 species, u8 delay, u8 form);

BOOL IsCryFinished(void);

BOOL IsFanfarePlaying(void);

EggHatchParticleSystem *sub_0207C894(EggHatchParticleSystemArgs *param0);

void sub_02077634(EggHatchParticleSystem *param0, int param1);

BOOL sub_02077650(EggHatchParticleSystem *param0);

void sub_0207C8F4(EggHatchParticleSystem *param0);

void Evolution_ProcessEvolutionEffects(EvolutionData *evolutionData);

void Evolution_InitGraphics(EvolutionData *evolutionData, BgConfig *bgConfig);

void Evolution_FreeBgs(BgConfig *bgConfig);

u8 sub_020772F8(EvolutionData *evolutionData, int entryID);

void sub_02077394(EvolutionData *evolutionData);

extern const u8 _020FFEC0[3];

EvolutionData *Evolution_Begin(Party *party, Pokemon *mon, int targetSpecies, Options *options, int showContest, Pokedex *pokedex, Bag *bag, GameStats *records, Poketch *poketch, int method, int flags, enum HeapID heapID);

enum EvolutionState {
    EVOLUTION_STATE_START_DELAY,
    EVOLUTION_STATE_START_RESET_SCREEN_BRIGHTNESS,
    EVOLUTION_STATE_PRINT_WHAT,
    EVOLUTION_STATE_WAIT_PRINT_WHAT,
    EVOLUTION_STATE_PRINT_POKEMON_IS_EVOLVING,
    EVOLUTION_STATE_WAIT_PRINT_POKEMON_IS_EVOLVING,
    EVOLUTION_STATE_START_FADE,
    EVOLUTION_STATE_CLAMP_IN,
    EVOLUTION_STATE_ANIMATION_ALTERNATE_POKEMON,
    EVOLUTION_STATE_CLAMP_OUT_AND_FADE,
    EVOLUTION_STATE_PLAY_EVOLVED_POKEMON_ANIMATION_AND_CRY,
    EVOLUTION_STATE_SET_POKEMON_VALUES_AND_PRINT_CONGRATULATIONS,
    EVOLUTION_STATE_LOG_EVOLVED_POKEMON,
    EVOLUTION_STATE_CHECK_LEARN_MOVE,
    EVOLUTION_STATE_PRINT_WANTS_TO_LEARN_MOVE,
    EVOLUTION_STATE_WAIT_PRINT_WANTS_TO_LEARN_MOVE,
    EVOLUTION_STATE_PRINT_ALREADY_KNOWS_FOUR_MOVES,
    EVOLUTION_STATE_WAIT_PRINT_ALREADY_KNOWS_FOUR_MOVES,
    EVOLUTION_STATE_PRINT_SHOULD_A_MOVE_BE_DELETED,
    EVOLUTION_STATE_WAIT_PRINT_SHOULD_A_MOVE_BE_DELETED,
    EVOLUTION_STATE_PRINT_YES_NO_SHOULD_A_MOVE_BE_DELETED,
    EVOLUTION_STATE_PROCESS_INPUT_SHOULD_A_MOVE_BE_DELETED,
    EVOLUTION_STATE_SET_SUMMARY_DATA,
    EVOLUTION_STATE_CHOOSE_MOVE_TO_DELETE,
    EVOLUTION_STATE_WAIT_FADE_FROM_SUMMARY_SCREEN,
    EVOLUTION_STATE_PRINT_ONE_TWO_THREE_POOF,
    EVOLUTION_STATE_WAIT_PRINT_ONE_TWO_THREE_POOF,
    EVOLUTION_STATE_PRINT_FORGOT_HOW_TO_USE_MOVE,
    EVOLUTION_STATE_WAIT_PRINT_FORGOT_HOW_TO_USE_MOVE,
    EVOLUTION_STATE_PRINT_AND_DOT_DOT_DOT,
    EVOLUTION_STATE_WAIT_PRINT_AND_DOT_DOT_DOT,
    EVOLUTION_STATE_PRINT_POKEMON_LEARNED_MOVE,
    EVOLUTION_STATE_PRINT_STOP_TRYING_TO_TEACH_MOVE,
    EVOLUTION_STATE_WAIT_PRINT_STOP_TRYING_TO_TEACH_MOVE,
    EVOLUTION_STATE_PRINT_YES_NO_STOP_TRYING_TO_TEACH_MOVE,
    EVOLUTION_STATE_PROCESS_INPUT_STOP_TRYING_TO_TEACH_MOVE,
    EVOLUTION_STATE_WAIT_PRINT_DID_NOT_LEARN_MOVE,
    EVOLUTION_STATE_WAIT_PRINT_POKEMON_LEARNED_MOVE,
    EVOLUTION_STATE_WAIT_LEARNED_MOVE_FANFARE,
    EVOLUTION_STATE_NO_MOVE_TO_LEARN,
    EVOLUTION_STATE_END,
    EVOLUTION_STATE_CANCEL_EVOLUTION,
    EVOLUTION_STATE_CANCEL_EVOLUTION_ANIMATION,
    EVOLUTION_STATE_PRINT_POKEMON_STOPPED_EVOLVING,
    EVOLUTION_STATE_FADE_CANCELED_EVOLUTION,
    EVOLUTION_STATE_END_CANCELED_EVOLUTION
};

void sub_02075E14(EvolutionData *evolutionData);


BOOL sub_02005D10(u16 bgmID);

EggHatchParticleSystem *sub_02077604(EggHatchParticleSystemArgs *param0);

void sub_0207584C(EvolutionData *evolutionData, u32 arg);

u8 sub_02075A04(EvolutionData *evolutionData);

void sub_020771A0(BgConfig *bgConfig);

void sub_02089D40(void *, const u8 *);

void sub_02076E64(EvolutionData *evolutionData, BgConfig *bgConfig);

void sub_02075770(EvolutionData *evolutionData);

void sub_02077664(void *arg0);

void sub_02076C90(EvolutionData *evolutionData);

void sub_02075630(EvolutionData *);

void sub_02075E14(EvolutionData *evolutionData) {
    if (evolutionData->shrinkAndGrowState) {
        if ((evolutionData->shrinkAndGrowState & 1) == 0) {
            Pokepic_AddAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_X, evolutionData->attributeDelta * -1);
            Pokepic_AddAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_Y, evolutionData->attributeDelta * -1);
            Pokepic_AddAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_X, evolutionData->attributeDelta);
            Pokepic_AddAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_Y, evolutionData->attributeDelta);

            if (Pokepic_GetAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_X) == 0) {
                evolutionData->shrinkAndGrowState ^= 1;
            }
        } else {
            Pokepic_AddAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_X, evolutionData->attributeDelta);
            Pokepic_AddAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_Y, evolutionData->attributeDelta);
            Pokepic_AddAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_X, evolutionData->attributeDelta * -1);
            Pokepic_AddAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_Y, evolutionData->attributeDelta * -1);

            if (Pokepic_GetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_X) == 0) {
                evolutionData->shrinkAndGrowState ^= 1;

                if (evolutionData->attributeDelta < 64) {
                    evolutionData->attributeDelta *= 2;
                }
            }
        }
    }

    if ((evolutionData->flags & 0x1) && evolutionData->state == EVOLUTION_STATE_ANIMATION_ALTERNATE_POKEMON && (gSystem.newKeys & (0x0002))) {
        PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xC00 ^ 0xFFFF, 0, 0, 16, 0x7FFF);
        evolutionData->state = EVOLUTION_STATE_CANCEL_EVOLUTION;
    }

    switch (evolutionData->state) {
    case EVOLUTION_STATE_START_DELAY:
        if (--evolutionData->delay == 0) {
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_START_RESET_SCREEN_BRIGHTNESS:
        SetMasterBrightnessNeutral(SCREEN_MAIN);
        SetMasterBrightnessNeutral(SCREEN_SUB);
        sub_0201649C(evolutionData->unk_58, 0);

        if (evolutionData->flags & 0x2) {
            evolutionData->state = EVOLUTION_STATE_PRINT_WHAT;
        } else {
            evolutionData->state = EVOLUTION_STATE_PRINT_POKEMON_IS_EVOLVING;
        }
        break;
    case EVOLUTION_STATE_PRINT_WHAT:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            evolutionData->printerID = sub_020772F8(evolutionData, 916);
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_WAIT_PRINT_WHAT:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0) {
            evolutionData->state = EVOLUTION_STATE_PRINT_POKEMON_IS_EVOLVING;
        }
        break;
    case EVOLUTION_STATE_PRINT_POKEMON_IS_EVOLVING:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            PokepicAnimScript animFrames[10];

            sub_0207294C(evolutionData->narc, evolutionData->monAnimMan, evolutionData->monSprites[0], evolutionData->currentSpecies, 2, 0, 0);
            NARC_ReadPokepicAnimScript(evolutionData->narc, &animFrames[0], evolutionData->currentSpecies, 1);
            Pokepic_SetAnimScript(evolutionData->monSprites[0], &animFrames[0]);
            Pokepic_StartAnim(evolutionData->monSprites[0], 0L);

            PlayCry(evolutionData->currentSpecies, evolutionData->form);
            BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));

            if (evolutionData->flags & 0x2) {
                evolutionData->printerID = sub_020772F8(evolutionData, 917);
            } else {
                evolutionData->printerID = sub_020772F8(evolutionData, 915);
            }

            evolutionData->state = EVOLUTION_STATE_WAIT_PRINT_POKEMON_IS_EVOLVING;
        }
        break;
    case EVOLUTION_STATE_WAIT_PRINT_POKEMON_IS_EVOLVING:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0
            && IsCryFinished() == 0
            && sub_02017068(evolutionData->monAnimMan, 0) == 1
            && Pokepic_IsAnimActive(evolutionData->monSprites[0]) == 0) {
            sub_0201649C(evolutionData->unk_58, 1);
            sub_02005D10(1011);
            evolutionData->delay = 20;
            evolutionData->state = EVOLUTION_STATE_START_FADE;
        }
        break;
    case EVOLUTION_STATE_START_FADE:
        if (--evolutionData->delay == 0) {
            EggHatchParticleSystemArgs v1;

            v1.heapID = evolutionData->heapID;
            v1.narcIdx = 0;
            evolutionData->unk_30 = sub_02077604(&v1);

            sub_02077634(evolutionData->unk_30, 0);
            Pokepic_StartPaletteFade(evolutionData->monSprites[0], 0, 16, 4, 0x7FFF);
            Pokepic_StartPaletteFade(evolutionData->monSprites[1], 0, 16, 4, 0x7FFF);

            ((HeapExp_FndGetTotalFreeSize(evolutionData->heapID) > 0x8000) ? (void)0 : GF_AssertFail());
            PlaySE(1528);

            evolutionData->delay = 40;
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_CLAMP_IN:
        if (evolutionData->windowTop < 40) {
            evolutionData->windowTop += 2;
            evolutionData->windowBottom -= 2;
        }

        if (--evolutionData->delay == 0) {
            sub_02077634(evolutionData->unk_30, 1);
            sub_02077634(evolutionData->unk_30, 2);
            sub_02077634(evolutionData->unk_30, 7);
            sub_02077634(evolutionData->unk_30, 8);
            sub_02077634(evolutionData->unk_30, 9);
            sub_02077634(evolutionData->unk_30, 11);
            PlaySE(1529);
            evolutionData->shrinkAndGrowState = 0x10;
            evolutionData->attributeDelta = 8;
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_ANIMATION_ALTERNATE_POKEMON:
        if (sub_02077650(evolutionData->unk_30) == 0) {
            sub_02077634(evolutionData->unk_30, 3);
            sub_02077634(evolutionData->unk_30, 4);
            sub_02077634(evolutionData->unk_30, 5);
            sub_02077634(evolutionData->unk_30, 6);
            sub_02077634(evolutionData->unk_30, 10);
            PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xC00 ^ 0xFFFF, 2, 0, 16, 0x7FFF);
            Pokepic_SetAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_X, 0);
            Pokepic_SetAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_Y, 0);
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_X, (1 << 8));
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_Y, (1 << 8));
            PlaySE(1530);
            evolutionData->shrinkAndGrowState = 0;
            evolutionData->delay = 8;
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_CLAMP_OUT_AND_FADE:
        if (evolutionData->windowTop) {
            evolutionData->windowTop -= 2;
            evolutionData->windowBottom += 2;
        }

        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            if (--evolutionData->delay == 0) {
                sub_02077634(evolutionData->unk_30, 12);
                PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xC00 ^ 0xFFFF, 4, 16, 0, 0x7FFF);
                Pokepic_StartPaletteFadeAll(evolutionData->monSpriteMan, 16, 0, 3, 0x7FFF);
                PlaySE(1531);
                evolutionData->state++;
            }
        }
        break;
    case EVOLUTION_STATE_PLAY_EVOLVED_POKEMON_ANIMATION_AND_CRY:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0 && sub_02077650(evolutionData->unk_30) == 0) {
            PokepicAnimScript animFrames[10];

            sub_0207294C(evolutionData->narc, evolutionData->monAnimMan, evolutionData->monSprites[1], evolutionData->targetSpecies, 2, 0, 0);
            NARC_ReadPokepicAnimScript(evolutionData->narc, &animFrames[0], evolutionData->targetSpecies, 1);
            Pokepic_SetAnimScript(evolutionData->monSprites[1], &animFrames[0]);
            Pokepic_StartAnim(evolutionData->monSprites[1], 0L);

            PlayCry(evolutionData->targetSpecies, evolutionData->form);
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_SET_POKEMON_VALUES_AND_PRINT_CONGRATULATIONS:
        if (IsCryFinished() == 0 && sub_02017068(evolutionData->monAnimMan, 0) == 1 && Pokepic_IsAnimActive(evolutionData->monSprites[1]) == 0) {
            SetMonData(evolutionData->mon, MON_DATA_SPECIES, (u8 *)&evolutionData->targetSpecies);
            UpdateMonAbility(evolutionData->mon);
            CalcMonLevelAndStats(evolutionData->mon);
            BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
            BufferBoxMonSpeciesName(evolutionData->strTemplate, 1, Mon_GetBoxMon(evolutionData->mon));
            evolutionData->printerID = sub_020772F8(evolutionData, 918);
            evolutionData->delay = 40;
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_LOG_EVOLVED_POKEMON:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0) {
            if (--evolutionData->delay == 0) {
                Pokedex_SetMonCaughtFlag(evolutionData->pokedex, evolutionData->mon);
                GameStats_Inc(evolutionData->records, RECORD_UNK_013);
                GameStats_AddScore(evolutionData->records, 21);

                if (GetMonData(evolutionData->mon, MON_DATA_HAS_NICKNAME, 0L) == 0) {
                    SetMonData(evolutionData->mon, MON_DATA_SPECIES_NAME, 0L);
                }

                evolutionData->state++;
            }
        }
        break;
    case EVOLUTION_STATE_CHECK_LEARN_MOVE:
        u16 moveID;

        switch (MonTryLearnMoveOnLevelUp(evolutionData->mon, &evolutionData->levelUpMoveIndex, &moveID)) {
        case 0:
            evolutionData->state = EVOLUTION_STATE_NO_MOVE_TO_LEARN;
            break;
        case 0xFFFE:
            break;
        case 0xFFFF:
            evolutionData->moveID = moveID;
            evolutionData->state = EVOLUTION_STATE_PRINT_WANTS_TO_LEARN_MOVE;
            break;
        default:
            BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
            BufferMoveName(evolutionData->strTemplate, 1, (int)(moveID));
            evolutionData->printerID = sub_020772F8(evolutionData, 4);
            evolutionData->delay = 30;
            evolutionData->state = EVOLUTION_STATE_WAIT_PRINT_POKEMON_LEARNED_MOVE;
            break;
        }
        break;
    case EVOLUTION_STATE_PRINT_WANTS_TO_LEARN_MOVE:
        BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
        BufferMoveName(evolutionData->strTemplate, 1, (int)(evolutionData->moveID));
        evolutionData->printerID = sub_020772F8(evolutionData, 1193);
        evolutionData->delay = 30;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PRINT_ALREADY_KNOWS_FOUR_MOVES:
        BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
        evolutionData->printerID = sub_020772F8(evolutionData, 1194);
        evolutionData->delay = 30;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PRINT_SHOULD_A_MOVE_BE_DELETED:
        evolutionData->printerID = sub_020772F8(evolutionData, 1195);
        evolutionData->delay = 1;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_WAIT_PRINT_WANTS_TO_LEARN_MOVE:
    case EVOLUTION_STATE_WAIT_PRINT_ALREADY_KNOWS_FOUR_MOVES:
    case EVOLUTION_STATE_WAIT_PRINT_SHOULD_A_MOVE_BE_DELETED:
    case EVOLUTION_STATE_WAIT_PRINT_ONE_TWO_THREE_POOF:
    case EVOLUTION_STATE_WAIT_PRINT_FORGOT_HOW_TO_USE_MOVE:
    case EVOLUTION_STATE_WAIT_PRINT_AND_DOT_DOT_DOT:
    case EVOLUTION_STATE_WAIT_PRINT_STOP_TRYING_TO_TEACH_MOVE:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0) {
            if (--evolutionData->delay == 0) {
                evolutionData->state++;
            }
        }
        break;
    case EVOLUTION_STATE_PRINT_YES_NO_SHOULD_A_MOVE_BE_DELETED:
        sub_0207584C(evolutionData, 1);
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PROCESS_INPUT_SHOULD_A_MOVE_BE_DELETED:
        switch (sub_02075A04(evolutionData)) {
        case 1:
            evolutionData->state = EVOLUTION_STATE_SET_SUMMARY_DATA;
            PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, 1, 0, 16, 0);
            Pokepic_StartPaletteFadeAll(evolutionData->monSpriteMan, 0, 16, 0, 0);
            break;
        case 2:
            evolutionData->state = EVOLUTION_STATE_PRINT_STOP_TRYING_TO_TEACH_MOVE;
            break;
        }
        break;
    case EVOLUTION_STATE_SET_SUMMARY_DATA:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            sub_0200FBF4(SCREEN_MAIN, (((0) << 10) | ((0) << 5) | (0)));
            sub_0200FBF4(SCREEN_SUB, (((0) << 10) | ((0) << 5) | (0)));
            sub_020771A0(evolutionData->bgConfig);
            sub_02075770(evolutionData);
            Pokepic_SetAttr(evolutionData->monSprites[0], MON_SPRITE_HIDE, 1);
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_HIDE, 1);
            evolutionData->monSummary->monData = evolutionData->mon;
            evolutionData->monSummary->options = evolutionData->options;
            evolutionData->monSummary->unk11 = SUMMARY_DATA_MON;
            evolutionData->monSummary->partySlot = 0;
            evolutionData->monSummary->partyCount = 1;
            evolutionData->monSummary->moveToLearn = evolutionData->moveID;
            evolutionData->monSummary->unk12 = SUMMARY_MODE_SELECT_MOVE;
            evolutionData->monSummary->unk28 = 0L;
            evolutionData->monSummary->menuInputStatePtr = NULL;
            sub_02089D40(evolutionData->monSummary, _020FFEC0);
            sub_02077394(evolutionData);
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_CHOOSE_MOVE_TO_DELETE:
        if (OverlayManager_Run(evolutionData->summaryScreenAppMan)) {
            OverlayManager_Delete(evolutionData->summaryScreenAppMan);
            evolutionData->summaryScreenAppMan = NULL;
            sub_02076E64(evolutionData, evolutionData->bgConfig);
            sub_02075630(evolutionData);
            DrawFrameAndWindow2(evolutionData->window, 0, 1, 10);
            Pokepic_SetAttr(evolutionData->monSprites[0], MON_SPRITE_HIDE, 0);
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_HIDE, 0);
            Pokepic_ScheduleReloadFromNarc(evolutionData->monSprites[0]);
            Pokepic_ScheduleReloadFromNarc(evolutionData->monSprites[1]);
            PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, 1, 16, 0, 0);
            Pokepic_StartPaletteFadeAll(evolutionData->monSpriteMan, 16, 0, 0, 0);
            sub_0203A880();
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_WAIT_FADE_FROM_SUMMARY_SCREEN:
        SetMasterBrightnessNeutral(SCREEN_MAIN);
        SetMasterBrightnessNeutral(SCREEN_SUB);

        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            if (evolutionData->monSummary->unk16 == 4) {
                evolutionData->state = EVOLUTION_STATE_PRINT_STOP_TRYING_TO_TEACH_MOVE;
            } else {
                evolutionData->moveSlot = evolutionData->monSummary->unk16;
                evolutionData->state = EVOLUTION_STATE_PRINT_ONE_TWO_THREE_POOF;
            }
        }
        break;
    case EVOLUTION_STATE_PRINT_STOP_TRYING_TO_TEACH_MOVE:
        BufferMoveName(evolutionData->strTemplate, 0, (int)(evolutionData->moveID));
        evolutionData->printerID = sub_020772F8(evolutionData, 1197);
        evolutionData->delay = 1;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PRINT_YES_NO_STOP_TRYING_TO_TEACH_MOVE:
        sub_0207584C(evolutionData, 0);
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PROCESS_INPUT_STOP_TRYING_TO_TEACH_MOVE:
        switch (sub_02075A04(evolutionData)) {
        case 1:
            BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
            BufferMoveName(evolutionData->strTemplate, 1, (int)(evolutionData->moveID));
            evolutionData->printerID = sub_020772F8(evolutionData, 1198);
            evolutionData->delay = 30;
            evolutionData->state = EVOLUTION_STATE_WAIT_PRINT_DID_NOT_LEARN_MOVE;
            break;
        case 2:
            evolutionData->state = EVOLUTION_STATE_PRINT_WANTS_TO_LEARN_MOVE;
            break;
        }
        break;
        break;
    case EVOLUTION_STATE_WAIT_PRINT_DID_NOT_LEARN_MOVE:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0) {
            if (--evolutionData->delay == 0) {
                evolutionData->state = EVOLUTION_STATE_CHECK_LEARN_MOVE;
            }
        }
        break;
    case EVOLUTION_STATE_PRINT_ONE_TWO_THREE_POOF:
        evolutionData->printerID = sub_020772F8(evolutionData, 1199);
        evolutionData->delay = 30;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PRINT_FORGOT_HOW_TO_USE_MOVE:
        BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
        BufferMoveName(evolutionData->strTemplate, 1, (int)(GetMonData(evolutionData->mon, (int)(MON_DATA_MOVE1 + evolutionData->moveSlot), 0L)));
        evolutionData->printerID = sub_020772F8(evolutionData, 1200);
        evolutionData->delay = 30;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PRINT_AND_DOT_DOT_DOT:
        evolutionData->printerID = sub_020772F8(evolutionData, 1201);
        evolutionData->delay = 30;
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_PRINT_POKEMON_LEARNED_MOVE:
        BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
        BufferMoveName(evolutionData->strTemplate, 1, (int)(evolutionData->moveID));
        evolutionData->printerID = sub_020772F8(evolutionData, 1202);
        evolutionData->delay = 0;
        SetMonData(evolutionData->mon, (int)(MON_DATA_MOVE1_PP_UPS + evolutionData->moveSlot), &evolutionData->delay);
        MonSetMoveInSlot(evolutionData->mon, evolutionData->moveID, evolutionData->moveSlot);
        evolutionData->delay = 30;
        evolutionData->state = EVOLUTION_STATE_WAIT_PRINT_POKEMON_LEARNED_MOVE;
        break;
    case EVOLUTION_STATE_WAIT_PRINT_POKEMON_LEARNED_MOVE:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0) {
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_WAIT_LEARNED_MOVE_FANFARE:
        if (IsFanfarePlaying() == 0) {
            if (--evolutionData->delay == 0) {
                evolutionData->state = EVOLUTION_STATE_CHECK_LEARN_MOVE;
            }
        }
        break;
    case EVOLUTION_STATE_NO_MOVE_TO_LEARN:
        PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, 1, 0, 16, 0);
        Pokepic_StartPaletteFadeAll(evolutionData->monSpriteMan, 0, 16, 0, 0);
        evolutionData->state++;
        break;
    case EVOLUTION_STATE_END:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            sub_02077664(evolutionData->unk_30);
            sub_02076C90(evolutionData);
            evolutionData->done = 1;
        }
        break;
    case EVOLUTION_STATE_CANCEL_EVOLUTION:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            Pokepic_SetAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_X, (1 << 8));
            Pokepic_SetAttr(evolutionData->monSprites[0], MON_SPRITE_SCALE_Y, (1 << 8));
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_X, 0);
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_SCALE_Y, 0);
            Pokepic_SetAttr(evolutionData->monSprites[1], MON_SPRITE_HIDE, 1);
            PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xC00 ^ 0xFFFF, 0, 16, 0, 0x7FFF);
            Pokepic_StartPaletteFadeAll(evolutionData->monSpriteMan, 16, 0, 0, 0x7FFF);
            evolutionData->windowLeft = 0;
            evolutionData->windowTop = 0;
            evolutionData->windowRight = 256 - 1;
            evolutionData->windowBottom = 160;
            evolutionData->shrinkAndGrowState = 0;
            StopBGM(1011, 0);
            sub_02077664(evolutionData->unk_30);
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_CANCEL_EVOLUTION_ANIMATION:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            PokepicAnimScript animFrames[10];

            sub_0207294C(evolutionData->narc, evolutionData->monAnimMan, evolutionData->monSprites[0], evolutionData->currentSpecies, 2, 0, 0);
            NARC_ReadPokepicAnimScript(evolutionData->narc, &animFrames[0], evolutionData->currentSpecies, 1);
            Pokepic_SetAnimScript(evolutionData->monSprites[0], &animFrames[0]);
            Pokepic_StartAnim(evolutionData->monSprites[0], 0L);

            PlayCry(evolutionData->currentSpecies, evolutionData->form);
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_PRINT_POKEMON_STOPPED_EVOLVING:
        if (IsCryFinished() == 0 && sub_02017068(evolutionData->monAnimMan, 0) == 1 && Pokepic_IsAnimActive(evolutionData->monSprites[0]) == 0) {
            BufferBoxMonNickname(evolutionData->strTemplate, 0, Mon_GetBoxMon(evolutionData->mon));
            evolutionData->printerID = sub_020772F8(evolutionData, 919);
            evolutionData->delay = 20;
            evolutionData->state++;
        }
        break;
    case EVOLUTION_STATE_FADE_CANCELED_EVOLUTION:
        if (TextPrinterCheckActive(evolutionData->printerID) == 0) {
            if (--evolutionData->delay == 0) {
                PaletteData_BeginPaletteFade(evolutionData->paletteData, (1 << PLTTBUF_MAIN_BG) | (1 << PLTTBUF_SUB_BG) | (1 << PLTTBUF_MAIN_OBJ) | (1 << PLTTBUF_SUB_OBJ), 0xFFFF, 1, 0, 16, 0);
                Pokepic_StartPaletteFadeAll(evolutionData->monSpriteMan, 0, 16, 0, 0);
                evolutionData->state++;
            }
        }
        break;
    case EVOLUTION_STATE_END_CANCELED_EVOLUTION:
        if (PaletteData_GetSelectedBuffersBitmask(evolutionData->paletteData) == 0) {
            evolutionData->done = 1;
        }
        break;
    }
}
