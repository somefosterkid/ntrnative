#include "global.h"

#include "constants/heap.h"

#include "struct_defs/resdat_narc_structs.h"

#include "bg_window.h"
#include "filesystem.h"
#include "gf_3d_loader.h"
#include "heap.h"
#include "palette.h"
#include "pokemon_types_def.h"
#include "sprite.h"
#include "sprite_system.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "unk_02009D48.h"
#include "unk_0200A090.h"
#include "overlay_07_ball_rotation.h"
#include "overlay_12_mon_encounter_data.h"
#include "battle/battle.h"
#include "overlay_07_particle_effect.h"

struct BallRotation;
struct BattleSystem;
struct OpponentData;
struct Pokepic;
struct PokepicDrawParam;
struct UnkStruct_ov12_02237F98_t;
struct UnkStruct_ov12_02238004_t;
typedef struct UISetupTaskData {
    BattleSystem *battleSys;
    UnkStruct_ov12_02237F98 *animContext;
    UnkStruct_ov12_02238004 *animObjects[3];
    u8 step;
    u8 frameCount;
    u8 fadeStep;
    u8 unused;
} UISetupTaskData;
void ov12_02260668(SysTask *task, void *data);
typedef struct SpriteSystemContext {
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    PaletteData *plttData;
} SpriteSystemContext;
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
typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;
typedef void(PokemonSpriteCallback)(Pokepic *, PokepicDrawParam *);
void Pokepic_StartPaletteFade(Pokepic *monSprite, int initAlpha, int targetAlpha, int delay, int color);
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
u8 GetNatureFromPersonality(u32 monPersonality);
void GetMonSpriteCharAndPlttNarcIdsEx(PokepicTemplate *spriteTemplate, u16 species, u8 gender, u8 face, u8 shiny, u8 form, u32 personality);
u8 GetMonPicHeightBySpeciesGenderForm(u16 species, u8 gender, u8 face, u8 form, u32 personality);
void NARC_ReadPokepicAnimScript(NARC *narc, SpriteAnimFrame *frames, u16 species, u16 clientType);
void sub_020729D8(NARC *narc, s8 *yOffset, u16 species);
void sub_020729FC(NARC *narc, s8 *xOffsetShadow, u16 species);
void sub_02072A20(NARC *narc, u8 *shadowSize, u16 species);
typedef struct MonEncounterMessage {
    u8 command;
    u8 gender : 2;
    u8 isShiny : 1;
    u8 formNum : 5;
    u16 species;
    u32 personality;
    enum PokemonCryMod cryModulation;

    u16 moves[4];
    u16 curPP[4];
    u16 maxPP[4];
    u16 nickname[10 + 1];
    u8 padding_3A[2];
} MonEncounterMessage;
u32 BattleSystem_GetBattleType(BattleSystem *battleSys);
PokepicManager *BattleSystem_GetPokepicManager(BattleSystem *battleSys);
UnkBattleSystemSub17C *ov12_0223A8F4(BattleSystem *battleSys, int side);
u32 BattleSystem_GetBattleSpecial(BattleSystem *battleSys);
extern const s16 ov07_022377DC[][2];
extern const s16 ov07_022377F4[][3];
void sub_02005B58(u8 allowed);
void ov12_0225B494(SysTask *task, void *data);
void ov12_0225B7B8(SysTask *task, void *data);
Pokepic *ov12_022612A4(BattleSystem *battleSys, PokepicManager *monSpriteMan, PokepicTemplate *spriteTemplate, int x, int y, int z, int yOffset, int height, int xOffset, int shadowSize, int battler, SpriteAnimFrame *animFrames, PokemonSpriteCallback *callback);

void ov12_02259944(BattleSystem *battleSys, OpponentData *battlerData);
void ov12_02259968(BattleSystem *battleSys, OpponentData *battlerData, MonEncounterMessage *message);

void ov12_02259944(BattleSystem *battleSys, OpponentData *battlerData) {
    UISetupTaskData *uiSetupTaskData = (UISetupTaskData *)Heap_Alloc(HEAP_ID_BATTLE, sizeof(UISetupTaskData));
    uiSetupTaskData->battleSys = battleSys;
    uiSetupTaskData->step = 0;
    uiSetupTaskData->frameCount = 0;
    uiSetupTaskData->fadeStep = 0;

    SysTask_CreateOnMainQueue(ov12_02260668, uiSetupTaskData, 0);
}

void ov12_02259968(BattleSystem *battleSys, OpponentData *battlerData, MonEncounterMessage *message) {
    BOOL isShiny;
    PokepicTemplate spriteTemplate;
    PokepicManager *spriteMan = BattleSystem_GetPokepicManager(battleSys);
    SpriteAnimFrame animFrames[10];
    MonEncounterData *monEncounterData;
    int battleType = BattleSystem_GetBattleType(battleSys);
    u8 yOffset;
    s8 height;
    s8 shadowXOffset;
    u8 shadowSize;

    monEncounterData = Heap_Alloc(HEAP_ID_BATTLE, sizeof(MonEncounterData));

    monEncounterData->state = 0;

    if (battlerData->battlerType & 1) {
        monEncounterData->face = 2;
        monEncounterData->terrain = ov12_0223A8F4(battleSys, 1);
        ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, ov07_022377F4[battlerData->battlerType & 1][0], 8 * 11);
    } else {
        monEncounterData->face = 0;
        monEncounterData->terrain = ov12_0223A8F4(battleSys, 0);
        ManagedSprite_SetPositionXY(monEncounterData->terrain->unk0, ov07_022377F4[battlerData->battlerType & 1][0], 128 + 8);
    }

    isShiny = !!message->isShiny;

    GetMonSpriteCharAndPlttNarcIdsEx(&spriteTemplate, message->species, message->gender, monEncounterData->face, isShiny, message->formNum, message->personality);

    yOffset = GetMonPicHeightBySpeciesGenderForm(message->species, message->gender, monEncounterData->face, message->formNum, message->personality);

    sub_020729D8(battlerData->narc, &height, message->species);
    sub_020729FC(battlerData->narc, &shadowXOffset, message->species);
    sub_02072A20(battlerData->narc, &shadowSize, message->species);
    NARC_ReadPokepicAnimScript(battlerData->narc, &animFrames[0], message->species, battlerData->battlerType);

    monEncounterData->sprite = battlerData->monSprite = ov12_022612A4(battleSys,
        spriteMan,
        &spriteTemplate,
        ov07_022377F4[battlerData->battlerType][0],
        ov07_022377F4[battlerData->battlerType][1],
        ov07_022377F4[battlerData->battlerType][2],
        yOffset,
        height,
        shadowXOffset,
        shadowSize,
        battlerData->battler,
        &animFrames[0],
        0L);

    if (monEncounterData->face == 2) {
        Pokepic_StartPaletteFade(monEncounterData->sprite, 8, 8, 0, 0);
    }

    if (monEncounterData->face == 2 && (BattleSystem_GetBattleSpecial(battleSys) & (1 << 6))) {
        int v10 = ((24 * 8) - -80) / 2;
        int spriteYCenter = Pokepic_GetAttr(monEncounterData->sprite, MON_SPRITE_Y_CENTER);

        Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_SHADOW_SIZE, 0);
        Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_X_CENTER, 256 - 64);
        Pokepic_SetAttr(monEncounterData->sprite, MON_SPRITE_Y_CENTER, spriteYCenter - v10);

        monEncounterData->targetPos = spriteYCenter;
    } else {
        monEncounterData->targetPos = ov07_022377DC[battlerData->battlerType][0];
    }

    monEncounterData->battleSys = battleSys;
    monEncounterData->battlerData = battlerData;
    monEncounterData->command = message->command;
    monEncounterData->battler = battlerData->battler;
    monEncounterData->species = message->species;
    monEncounterData->formNum = message->formNum;
    monEncounterData->cryMod = message->cryModulation;
    monEncounterData->battlerType = battlerData->battlerType;
    monEncounterData->nature = GetNatureFromPersonality(message->personality);
    monEncounterData->isShiny = message->isShiny;

    if (monEncounterData->face == 2 && (BattleSystem_GetBattleSpecial(battleSys) & (1 << 6))) {
        SysTask_CreateOnMainQueue(ov12_0225B7B8, monEncounterData, 0);
    } else {
        SysTask_CreateOnMainQueue(ov12_0225B494, monEncounterData, 0);
    }

    sub_02005B58(1);
}
