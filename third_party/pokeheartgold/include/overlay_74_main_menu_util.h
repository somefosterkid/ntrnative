#ifndef POKEHEARTGOLD_OVERLAY_74_MAIN_MENU_UTIL_H
#define POKEHEARTGOLD_OVERLAY_74_MAIN_MENU_UTIL_H

// The main menu's util manager (ov74_0223D454, set up by ov74_022352A0): the one definition, with its DS layout
// (0x34 = spriteManager, 0xEA8 bytes). Pointer-typed fields throughout: every user reads it by field, never at a raw offset.

#include "constants/heap.h"

#include "g2d_renderer.h"
#include "sprite_resources_header.h"

struct BgConfig;
struct SaveData;
struct Sprite;
struct SpriteList;
struct SpriteResource;
struct GF_2DGfxResMan;

// Progress of the main menu's save routine (ov74_022360B0 returns it once per frame).
enum MainMenuUtilSavingStatus {
    MAIN_MENU_UTIL_SAVING_IN_PROGRESS = 0,
    MAIN_MENU_UTIL_SAVING_FINISHING,
    MAIN_MENU_UTIL_SAVING_OK,
    MAIN_MENU_UTIL_SAVING_CORRUPT,
};

typedef struct PokemonSpriteTemplate {
    u16 narcID;
    u16 character;
    u16 palette;
    u16 spindaSpots;

    u8 dummy;

    u32 personality;
} PokemonSpriteTemplate;

typedef struct MainMenuSpriteManager {
    struct SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *resourceCollections[6];
    struct SpriteResource *resources[2][6];
    SpriteResourcesHeader resourceHeaders[2];
    int unused_1F0;
    int unused_1F4;
    int unused_1F8;
    int unused_1FC;
    int unused_200;
    int unused_204;
} MainMenuSpriteManager;

typedef struct MainMenuUtilManager MainMenuUtilManager;

typedef void (*MainMenuUtilVBlankFuncPtr)(MainMenuUtilManager *utilMan);

struct MainMenuUtilManager {
    struct BgConfig *bgConfig;
    BOOL initialized;
    enum HeapID heapID;
    int screenFadeQueuedState;
    BOOL fadeToWhite;
    int savingStatus;
    struct SaveData *saveData;
    void *unused_1C;
    int unused_20;
    BOOL unused_24[4];
    MainMenuSpriteManager spriteManager;
    struct Sprite *mysteryGiftSprite;
    int giftSpriteDelay;
    fx32 bottomScreenTopY;
    u8 monSpriteBuffer[3200];
    PokemonSpriteTemplate monSpriteTemplate;
    MainMenuUtilVBlankFuncPtr onVBlank;
};

extern MainMenuUtilManager ov74_0223D454;

MainMenuSpriteManager *ov74_02235708(void);

#endif // POKEHEARTGOLD_OVERLAY_74_MAIN_MENU_UTIL_H
