#include "global.h"
#include "overlay_41_unk_struct_ov22_02259560.h"
#include "constants/heap.h"
#include "filesystem_files_def.h"
#include "pm_string.h"
#include "fashion_case.h"
#include "options.h"
#include "pokepic.h"
#include "sys_task.h"
#include "sprite.h"
#include "overlay_41_sprite_resource_collection.h"
#include "player_data.h"
#include "game_stats.h"
#include "overlay_41_unk_struct_ov22_0225_a0_e4.h"
#include "overlay_41_unk_struct_ov22_02257964.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"
#include "overlay_41_unk_struct_ov22_0225ab54.h"
#include "overlay_41_unk_struct_ov22_0225a154.h"
#include "overlay_41_unk_struct_ov22_0225af68.h"
#include "overlay_41_unk_struct_ov22_02259484.h"
#include "overlay_41_unk_struct_ov22_02258A48.h"
#include "overlay_41_unk_struct_ov22_0225A428.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"
#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_022597BC.h"

struct OverlayManager;

struct OverlayManagerTemplate;

struct BgConfig;

struct CharTransferAllocation;

struct DressUpPhoto;

struct FashionCase;

struct TextOBJ;

struct GameStats;

struct Options;

struct Pokemon;

struct Pokepic;

struct PokemonSpriteShadow;

struct PokemonSpriteTransforms;

struct SoftwareSprite;

struct SoftwareSpriteChars;

struct SoftwareSpriteManager;

struct SoftwareSpritePalette;

struct Sprite;

struct SpriteAnimFrame;

struct SpriteList;

struct SpriteResource;

struct SpriteResourceCollection;

struct String;

struct MessageFormat;

struct SysTask;

struct SysTaskManager;

struct System;

struct TouchHitboxController;

struct PlayerProfile;

struct UnkStruct_02012744_t;

struct UnkStruct_02029C88_t;

struct UnkStruct_ov22_02258A48_t;

struct UnkStruct_ov22_02259C58_tag;

struct Window;

struct YesNoPrompt;

typedef struct OverlayManager OverlayManager;

typedef BOOL (*OverlayFunc)(OverlayManager *appMan, int *state);

typedef struct OverlayManagerTemplate {
    OverlayFunc init;
    OverlayFunc main;
    OverlayFunc exit;
    const FSOverlayID overlayID;
} OverlayManagerTemplate;

struct OverlayManager {
    OverlayManagerTemplate template;
    int execState;
    int procState;
    void *args;
    void *data;
    OverlayManager *parent;
    OverlayManager *child;
};

void *OverlayManager_GetData(OverlayManager *appMan, int *);

void *OverlayManager_GetArgs(OverlayManager *appMan);

typedef struct DressUpPhoto DressUpPhoto;

typedef struct UnkStruct_02029C88_t UnkStruct_02029C88;

typedef struct SpriteAnimFrame {
    s8 spriteFrame;
    u8 frameDelay;
    s8 xOffset;
    s8 yOffset;
} SpriteAnimFrame;

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

typedef struct CharTransferAllocation {
    u32 size;
    u32 offset;
    u16 vramType;
    u16 atEnd;
} CharTransferAllocation;

typedef struct {
    Pokemon *pokemon;
    DressUpPhoto *photo;
    const FashionCase *fashionCase;
    const Options *options;
    GameStats *records;
    const PlayerProfile *trainerInfo;
    BOOL *unk_18;
    BOOL unk_1C;
} UnkStruct_0203DA00;

typedef struct {
    u32 unk_00;
    u8 unk_04;
    u8 padding_05[3];
} UnkStruct_02095B28;

typedef struct {
    UnkStruct_02095B28 unk_00;
    UnkStruct_02095B28 unk_08;
    u8 unk_10[4];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStruct_02095C60;

enum PokemonContestRank {
    CONTEST_RANK_NORMAL = 0,
    CONTEST_RANK_GREAT = 1,
    CONTEST_RANK_ULTRA = 2,
    CONTEST_RANK_MASTER = 3,
    CONTEST_NUM_RANKS = 3,
    CONTEST_MAX_RANKS = 4,
    CONTEST_RANK_LINK = 4,
};

enum PokemonContestType {
    CONTEST_TYPE_COOL = 0,
    CONTEST_TYPE_BEAUTY = 1,
    CONTEST_TYPE_CUTE = 2,
    CONTEST_TYPE_SMART = 3,
    CONTEST_TYPE_TOUGH = 4,
    CONTEST_TYPE_MAX = 5,
};

typedef struct SoftwareSpriteChars SoftwareSpriteChars;

typedef struct SoftwareSpritePalette SoftwareSpritePalette;

typedef struct SoftwareSprite SoftwareSprite;

typedef struct SoftwareSpriteManager SoftwareSpriteManager;



typedef struct BgConfig BgConfig;

typedef struct Window Window;





void ov41_02248E44(UnkStruct_ov22_02258A48 *param0);




typedef struct UnkStruct_ov22_02259C58_tag {
    Sprite *unk_00;
    int unk_04;
    void *unk_08;
    void (*unk_0C)(struct UnkStruct_ov22_02259C58_tag *param0, void *param1);
} UnkStruct_ov22_02259C58;

typedef struct UnkStruct_02012744_t UnkStruct_02012744;

typedef struct TextOBJ TextOBJ;

typedef struct TouchHitboxController TouchHitboxController;

typedef struct {
    UnkStruct_ov22_02259C58 unk_00[4];
    UnkStruct_ov22_0225A154 unk_40;
    BOOL unk_60;
    UnkStruct_02012744 *unk_64;
    TouchHitboxController *unk_68;
    TouchscreenHitbox unk_6C[5];
} UnkStruct_ov22_02259D2C;

void ov41_0224ABF0(UnkStruct_ov22_0225A428 *param0);

#if defined(SDK_PORT) || defined(NONMATCHING)
void ov41_0224AC08(UnkStruct_ov22_0225A428 *param0, int param1, int param2, int param3);
#else
void ov41_0224AC08(UnkStruct_ov22_0225A428 *param0, int param1, u8 param2, int param3);
#endif

#if defined(SDK_PORT) || defined(NONMATCHING)
s32 ov41_0224AC40(UnkStruct_ov22_0225A428 *param0, int param1, int param2, int param3);
#else
s32 ov41_0224AC40(UnkStruct_ov22_0225A428 *param0, int param1, u8 param2, int param3);
#endif

void ov41_0224AC80(UnkStruct_ov22_0225A428 *param0);


void ov41_0224B4E8(UnkStruct_ov22_0225AF68 *param0, const UnkStruct_ov22_02257964 *param1, int param2);

void ov41_0224B50C(UnkStruct_ov22_0225AF68 *param0);

void ov22_0225AF74(UnkStruct_ov22_0225AF68 *param0);

void TextFlags_SetCanTouchSpeedUpPrint(BOOL val);

enum FadeType {
    FADE_TYPE_BRIGHTNESS_OUT = 0,
    FADE_TYPE_BRIGHTNESS_IN = 1,
    FADE_TYPE_DOWNWARD_OUT = 2,
    FADE_TYPE_DOWNWARD_IN = 3,
    FADE_TYPE_UPWARD_OUT = 4,
    FADE_TYPE_UNK_5 = 5,
    FADE_TYPE_UNK_6 = 6,
    FADE_TYPE_UNK_7 = 7,
    FADE_TYPE_UNK_8 = 8,
    FADE_TYPE_UNK_9 = 9,
    FADE_TYPE_UNK_10 = 10,
    FADE_TYPE_UNK_11 = 11,
    FADE_TYPE_UNK_12 = 12,
    FADE_TYPE_UNK_13 = 13,
    FADE_TYPE_UNK_14 = 14,
    FADE_TYPE_UNK_15 = 15,
    FADE_TYPE_CIRCLE_OUT = 16,
    FADE_TYPE_CIRCLE_IN = 17,
    FADE_TYPE_TOP_HALF_CIRCLE_OUT = 18,
    FADE_TYPE_TOP_HALF_CIRCLE_IN = 19,
    FADE_TYPE_UNK_20 = 20,
    FADE_TYPE_UNK_21 = 21,
    FADE_TYPE_UNK_22 = 22,
    FADE_TYPE_UNK_23 = 23,
    FADE_TYPE_UNK_24 = 24,
    FADE_TYPE_UNK_25 = 25,
    FADE_TYPE_UNK_26 = 26,
    FADE_TYPE_UNK_27 = 27,
    FADE_TYPE_UNK_28 = 28,
    FADE_TYPE_UNK_29 = 29,
    FADE_TYPE_UNK_30 = 30,
    FADE_TYPE_UNK_31 = 31,
    FADE_TYPE_UNK_32 = 32,
    FADE_TYPE_UNK_33 = 33,
    FADE_TYPE_UNK_34 = 34,
    FADE_TYPE_UNK_35 = 35,
    FADE_TYPE_UNK_36 = 36,
    FADE_TYPE_UNK_37 = 37,
    FADE_TYPE_UNK_38 = 38,
    FADE_TYPE_UNK_39 = 39,
    FADE_TYPE_CLAMP_OUT = 40,
    FADE_TYPE_CLAMP_IN = 41,
    FADE_TYPE_MAX = 42,
};

typedef void (*Callback)(void *);

typedef void (*UnkCallback)(void);

typedef struct System {
    Callback vBlankIntr;
    void *vBlankIntrArg;
    Callback hBlankIntr;
    void *hBlankIntrArg;
    UnkCallback unk10;
    UnkCallback unk14;
    SysTaskManager *mainTaskQueue;
    SysTaskManager *vblankTaskQueue;
    SysTaskManager *vwaitTaskQueue;
    SysTaskManager *printTaskQueue;
    u32 *unk28;
    u32 vblankCounter;
    u32 frameCounter;
    int buttonMode;
    int heldKeysRaw;
    int newKeysRaw;
    int newAndRepeatedKeysRaw;
    int heldKeys;
    int newKeys;
    int newAndRepeatedKeys;
    int keyRepeatCounter;
    int keyRepeatContinueDelay;
    int keyRepeatStartDelay;
    int simulatedInputs;
    u16 touchX;
    u16 touchY;
    u16 touchNew;
    u16 touchHeld;
    u8 touchpadReadAuto;
    u8 screensFlipped;
    u8 unk6A;
    u8 lidClosedPauseDisabled;
    u8 softResetDisabled;
    BOOL unk70;
    u32 *unk74;
} System;

extern System gSystem;

enum FadeMode {
    FADE_BOTH_SCREENS = 0,
    FADE_MAIN_THEN_SUB,
    FADE_SUB_THEN_MAIN,
    FADE_MAIN_ONLY,
    FADE_SUB_ONLY,
};

void BeginNormalPaletteFade(enum FadeMode mode, enum FadeType typeMain, enum FadeType typeSub, u16 color, int steps, int framesPerStep, enum HeapID heapID);

BOOL IsPaletteFadeFinished(void);

typedef struct MessageFormat MessageFormat;

u8 TextPrinterCheckActive(u8 printerID);

typedef struct YesNoPrompt YesNoPrompt;

typedef struct {
    u8 unk_00[0x35C]; /* HGSS layout: 0x64 smaller than Platinum's unk_00 + unk_1E8 */
    UnkStruct_ov22_02259484 unk_3C0;
    UnkStruct_ov22_02254DE0 *unk_3C8;
    UnkStruct_ov22_02257F50 unk_3CC;
    UnkStruct_ov22_02257964 unk_458;
    UnkStruct_ov22_0225AF68 unk_4E0;
    UnkStruct_ov22_02258A48 unk_4FC;
    UnkStruct_ov22_022597BC unk_518;
    UnkStruct_ov22_02259D2C unk_544;
    u8 unk_5BC[8]; /* HGSS layout */
    UnkStruct_ov22_0225A428 unk_5C4;
    int unk_70C;
    int unk_710;
    YesNoPrompt *unk_714;
    Window *unk_718;
    BOOL unk_71C;
    int unk_720;
    enum PokemonContestRank contestRank;
    int unk_728;
    enum PokemonContestType contestType;
    int unk_730;
    UnkStruct_02095C60 *unk_734;
    const Options *options;
    u32 unk_73C;
    String *unk_740;
    MessageFormat *unk_744;
} UnkStruct_ov22_02255D44;

void ov22_022566EC(UnkStruct_ov22_02255D44 *param0);

void ov41_02247578(UnkStruct_ov22_02255D44 *param0);

void ov41_022476B8(UnkStruct_ov22_02255D44 *param0, BOOL *param1);

void ov22_02256DB8(UnkStruct_ov22_02255D44 *param0, BOOL *param1);

u32 ov22_02257278(UnkStruct_ov22_02255D44 *param0);

void ov22_022575B4(UnkStruct_ov22_02255D44 *param0);

u32 ov22_02257624(UnkStruct_ov22_02255D44 *param0);

void ov22_02257564(UnkStruct_ov22_02255D44 *param0);

void ov22_02256FD8(UnkStruct_02029C88 *param0, UnkStruct_ov22_02257964 *param1, enum PokemonContestRank contestRank, const PlayerProfile *trainerInfo);

int ov41_02246F08(OverlayManager *appMan, int *param1);

void ov41_02247D44(UnkStruct_ov22_02255D44 *param0);

void ov41_02247828(UnkStruct_ov22_02255D44 *param0, BOOL *param1);

void ov41_02247D64(UnkStruct_ov22_02255D44 *param0);

u32 ov41_02247B7C(UnkStruct_ov22_02255D44 *param0);

u32 ov41_02247DF8(UnkStruct_ov22_02255D44 *param0);

void ov41_0224B518(UnkStruct_ov22_0225AF68 *param0);

void ov41_0224726C(UnkStruct_ov22_02255D44 *param0);

int ov41_02246F08(OverlayManager *appMan, int *param1) {
    UnkStruct_ov22_02255D44 *v0 = OverlayManager_GetData(appMan, param1);
    int v1 = 0;
    UnkStruct_0203DA00 *v2 = OverlayManager_GetArgs(appMan);

    switch (*param1) {
    case 0:
    case 1:
        BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_UNK_5, FADE_TYPE_UNK_5, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_13);
        (*param1) = 2;
        break;
    case 2:
        if (IsPaletteFadeFinished()) {
            (*param1)++;
        }
        break;
    case 3:
        if (v2->unk_1C == 1) {
            TextFlags_SetCanTouchSpeedUpPrint(1);

            v0->unk_73C = ov41_0224AC40(&v0->unk_5C4, 27, 471, 47);
            (*param1)++;
        } else {
            (*param1) = 6;
        }
        break;
    case 4:
        if (TextPrinterCheckActive(v0->unk_73C) == 0) {
            ov41_0224AC80(&v0->unk_5C4);
            (*param1)++;
        }
        break;
    case 5:
        if ((gSystem.newKeys & (0x0001 | 0x0002)) | (gSystem.touchNew)) {
            ov41_0224AC08(&v0->unk_5C4, 27, 471, 48);
            TextFlags_SetCanTouchSpeedUpPrint(0);
            (*param1)++;
        }
        break;
    case 6:
        if (v0->unk_70C == 3) {
            ov41_022476B8(v0, &v0->unk_710);

            (*param1) = 7;
        }

        ov41_02248E44(&v0->unk_4FC);
        ov41_02247D44(v0);
        ov41_0224ABF0(&v0->unk_5C4);
        ov41_02247578(v0);
        break;
    case 7:
        if (v0->unk_710) {
            v0->unk_710 = 0;
            (*param1) = 8;
            v0->unk_70C = 4;
            ov41_0224B4E8(&v0->unk_4E0, &v0->unk_458, 14);
        }
        break;
    case 8:
        if (v0->unk_70C == 9) {
            ov41_02247828(v0, &v0->unk_710);
            (*param1) = 10;
            break;
        }

        if (v0->unk_70C == 8) {
            (*param1) = 9;
            v0->unk_70C = 5;
            ov41_02247D64(v0);
            break;
        }

        v0->unk_70C = ov41_02247B7C(v0);
        ov41_0224B50C(&v0->unk_4E0);

        break;
    case 9:
        if (v0->unk_70C == 6) {
            v0->unk_71C = 1;
            (*param1) = 11;
            break;
        }

        if (v0->unk_70C == 7) {
            v0->unk_71C = 0;
            (*param1) = 11;
            break;
        }

        v0->unk_70C = ov41_02247DF8(v0);
        ov41_0224B50C(&v0->unk_4E0);
        break;
    case 10:
        if (v0->unk_710) {
            v0->unk_710 = 0;
            (*param1) = 6;
            v0->unk_70C = 0;

            ov41_0224B518(&v0->unk_4E0);
        }
        break;
    case 11:
        BeginNormalPaletteFade(FADE_MAIN_THEN_SUB, FADE_TYPE_BRIGHTNESS_OUT, FADE_TYPE_BRIGHTNESS_OUT, (((0) << 10) | ((0) << 5) | (0)), 6, 1, HEAP_ID_13);
        (*param1)++;
        break;
    case 12:
        if (IsPaletteFadeFinished()) {
            (*param1) = 0;
            v0->unk_70C = 10;
            v1 = 1;
            ov41_0224B518(&v0->unk_4E0);
        }
        break;
    }

    ov41_0224726C(v0);
    return v1;
}
