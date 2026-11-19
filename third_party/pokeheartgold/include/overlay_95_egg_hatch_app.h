#ifndef POKEHEARTGOLD_OVERLAY_95_EGG_HATCH_APP_H
#define POKEHEARTGOLD_OVERLAY_95_EGG_HATCH_APP_H

#include "constants/heap.h"

#include "bg_window.h"
#include "egg_hatch_particle_system.h"
#include "filesystem.h"
#include "gf_3d_vramman.h"
#include "message_format.h"
#include "options.h"
#include "palette.h"
#include "player_data.h"
#include "pokemon_types_def.h"
#include "pokepic.h"
#include "sprite.h"
#include "sprite_system.h"
#include "unk_02016EDC.h"

struct ListMenu2D;
struct ListMenuItem;
struct NamingScreenArgs;

typedef struct EggHatchArgs {
    Pokemon *mon;
    Options *options;
    PlayerProfile *trainerInfo;
    u16 bgmID;
} EggHatchArgs;

// The overlay manager's args/data of the egg hatch app (HatchEggApp_Init gets it from OverlayManager_GetArgs)
typedef struct EggHatchApp {
    int state;
    BOOL nicknameMon;
    struct NamingScreenArgs *nameScreenArgs;
    EggHatchArgs args;
} EggHatchApp;

// The two touch-screen text lines ("yes"/"no") of the nickname prompt, made by ov95_021E7020. sizeof == 0x3C on the DS.
typedef struct EggHatchPrompt {
    enum HeapID heapID;
    SpriteSystem *spriteSys;
    SpriteManager *spriteMan;
    BgConfig *bgConfig;
    ManagedSprite *cursorSprite;
    MessageFormat *msgFormat;
    u8 state;
    u8 selection;
    Window windows[2];
} EggHatchPrompt;

typedef struct EggHatchGraphics {
    BgConfig *bgConfig;
    PaletteData *plttData;
    int printerID;
    int renderDelay;
    int frame;
    Window windows[2];
    GF3DVramMan *vramMan;
    PokepicManager *monSpriteMan;
    NARC *monDataNarc;
    EggHatchParticleSystem *eps;
    struct ListMenuItem *strList;
    struct ListMenu2D *yesNoMenu;
    SpriteManager *spriteMan;
    SpriteSystem *spriteSys;
    PokepicAnimManager *animMan;
} EggHatchGraphics;

typedef struct EggHatchShakeAnimation {
    int isAnimFinished;
    int shakeProgress;
} EggHatchShakeAnimation;

// The heap object of the egg hatch overlay (HatchEggApp_Init allocates it, the V-blank callback ov95_021E5B24 reads
// it). sizeof == 0x8C on the DS.
typedef struct EggHatchCutscene {
    EggHatchApp *app;
    EggHatchGraphics graphics;
    int isManaphy;
    int state;
    int subStateTimer;
    int subState;
    u8 cryDelay;
    Pokepic *monSprite;
    ManagedSprite *eggSprite;
    ManagedSprite *topBarSprite;
    ManagedSprite *bottomBarSprite;
    EggHatchShakeAnimation shake;
    EggHatchPrompt *prompt;
} EggHatchCutscene;

// The DS layout (ILP32): the checks run only under mwcc; the parts add up to the whole, so every offset is pinned.
#ifdef __MWERKS__
typedef char EggHatchApp_size_check[(sizeof(EggHatchApp) == 0x1C) ? 1 : -1];
typedef char EggHatchPrompt_size_check[(sizeof(EggHatchPrompt) == 0x3C) ? 1 : -1];
typedef char EggHatchGraphics_size_check[(sizeof(EggHatchGraphics) == 0x58) ? 1 : -1];
typedef char EggHatchShakeAnimation_size_check[(sizeof(EggHatchShakeAnimation) == 0x8) ? 1 : -1];
typedef char EggHatchCutscene_size_check[(sizeof(EggHatchCutscene) == 0x8C) ? 1 : -1];
#endif

#endif // POKEHEARTGOLD_OVERLAY_95_EGG_HATCH_APP_H
