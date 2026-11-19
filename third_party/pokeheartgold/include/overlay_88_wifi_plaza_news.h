#ifndef POKEHEARTGOLD_OVERLAY_88_WIFI_PLAZA_NEWS_H
#define POKEHEARTGOLD_OVERLAY_88_WIFI_PLAZA_NEWS_H

#include "global.h"

#include "bg_window.h"
#include "filesystem.h"
#include "g2d_renderer.h"
#include "overlay_88_unk_struct_ov66_02231428.h"
#include "overlay_88_wifi_plaza_news_countdown.h"
#include "pm_string.h"
#include "sprite.h"

struct GF_2DGfxResMan;
struct UnkStruct_ov66_0222DFF8_t;
struct UnkStruct_ov66_0223177C_t;

enum {
    WIFI_PLAZA_NEWS_CAPTION_STATIC = 0,
    WIFI_PLAZA_NEWS_CAPTION_COUNTDOWN,
    WIFI_PLAZA_NEWS_CAPTION_PLAYER_COUNT,
    WIFI_PLAZA_NEWS_CAPTION_LATEST_NAME,
    WIFI_PLAZA_NEWS_CAPTION_COUNT,
};

typedef struct WiFiPlazaNewsDigits {
    void *buffer;
    NNSG2dScreenData *screenData;
} WiFiPlazaNewsDigits;

typedef struct WiFiPlazaNewsAnimState {
    s16 animationFrame;
    u16 animationState;
} WiFiPlazaNewsAnimState;

typedef struct WiFiPlazaNewsNamePalette {
    u8 paletteIndices[4];
} WiFiPlazaNewsNamePalette;

typedef struct WiFiPlazaNewsNameEntry {
    u8 active;
    u8 paletteSlot;
    u16 holdTimer;
    u16 holdDuration;
    u16 scrollRange;
    String *name;
    Window textWindow;
    WiFiPlazaNewsNamePalette namePalette;
} WiFiPlazaNewsNameEntry;

typedef struct WiFiPlazaNewsNameList {
    WiFiPlazaNewsNameEntry entries[3];
    Window windows[3];
    String *pendingName;
    void *paletteBuffer;
    NNSG2dPaletteData *paletteData;
} WiFiPlazaNewsNameList;

typedef struct WiFiPlazaNewsFlash {
    u16 active;
    u16 frameCounter;
    u16 blendColor;
    u16 paletteIndex;
    u32 soundEffect;
} WiFiPlazaNewsFlash;

typedef struct WiFiPlazaNewsCaptions {
    Window windows[WIFI_PLAZA_NEWS_CAPTION_COUNT];
    WiFiPlazaNewsFlash newsFlashes[WIFI_PLAZA_NEWS_CAPTION_COUNT];
} WiFiPlazaNewsCaptions;

typedef struct WiFiPlazaNewsGraphics {
    BgConfig *bgConfig;
    SpriteList *spriteList;
    G2dRenderer renderer;
    struct GF_2DGfxResMan *spriteResources[4];
    NARC *narc;
} WiFiPlazaNewsGraphics;

// The args the Wi-Fi Plaza news board is launched with (OverlayManager args of ov88_02258800).
typedef struct WiFiPlazaApplicationArgs {
    struct UnkStruct_ov66_0223177C_t *playerList;
    const UnkStruct_ov66_02231428 *closeTime;
    struct UnkStruct_ov66_0222DFF8_t *plazaSystem;
} WiFiPlazaApplicationArgs;

// The Wi-Fi Plaza news board's work (OverlayManager data of ov88_02258800), 0x270 bytes.
typedef struct WiFiPlazaNewsData {
    struct UnkStruct_ov66_0223177C_t *playerList; // 0x000
    const UnkStruct_ov66_02231428 *closeTime;     // 0x004
    WiFiPlazaNewsGraphics graphics;               // 0x008
    WiFiPlazaNewsDigits digits;                   // 0x14C
    WiFiPlazaNewsCountdown countdown;             // 0x154
    WiFiPlazaNewsAnimState animState;             // 0x160
    WiFiPlazaNewsNameList nameList;               // 0x164
    WiFiPlazaNewsCaptions captions;               // 0x200
} WiFiPlazaNewsData;

// sizeof(WiFiPlazaNewsData) == 0x270

#endif // POKEHEARTGOLD_OVERLAY_88_WIFI_PLAZA_NEWS_H
