#ifndef POKEHEARTGOLD_SCREEN_FADE_INTERNAL_H
#define POKEHEARTGOLD_SCREEN_FADE_INTERNAL_H

#include "global.h"

#include "screen_fade.h"

// The screen fade's work record (one ScreenFade per screen, in ScreenFadeManager) and what it points to, shared by the FadeFunc_NN
// callbacks (CallFadeFunc passes each one its ScreenFade). Pointer members, so offsets differ between DS and 64-bit hosts: use the fields.

typedef struct HardwareWindowVisibility HardwareWindowVisibility;

typedef struct HardwareWindowInsidePlane HardwareWindowInsidePlane;

typedef struct HardwareWindowOutsidePlane HardwareWindowOutsidePlane;

typedef struct HardwareWindowDimensions HardwareWindowDimensions;

typedef struct HardwareWindowSettings HardwareWindowSettings;

typedef struct ScreenFadeHBlanks ScreenFadeHBlanks;

typedef struct ScreenFade ScreenFade;

enum DSScreen {
    DS_SCREEN_MAIN = 0,
    DS_SCREEN_SUB,
    DS_SCREEN_MAX,
};

enum HardwareWindow {
    HW_WINDOW_WND0 = 0,
    HW_WINDOW_WND1,
    HW_WINDOW_MAX,
};

struct HardwareWindowVisibility {
    GXWndMask windowMask;
    enum DSScreen screen;
};

struct HardwareWindowInsidePlane {
    int wnd;
    BOOL applyColorEffect;
    enum HardwareWindow windowID;
    enum DSScreen screen;
};

struct HardwareWindowOutsidePlane {
    int wnd;
    BOOL applyColorEffect;
    enum DSScreen screen;
};

struct HardwareWindowDimensions {
    s16 left;
    s16 top;
    s16 right;
    s16 bottom;
    enum HardwareWindow windowID;
    enum DSScreen screen;
};

struct HardwareWindowSettings {
    HardwareWindowVisibility visible[DS_SCREEN_MAX];
    HardwareWindowInsidePlane inside[DS_SCREEN_MAX][HW_WINDOW_MAX];

    u8 padding_50[24];

    HardwareWindowOutsidePlane outside[DS_SCREEN_MAX];
    HardwareWindowDimensions dimensions[DS_SCREEN_MAX][HW_WINDOW_MAX];
};

typedef void (*Callback)(void *);

enum FadeDirection {
    FADE_IN = 0,
    FADE_OUT,
};

enum FadeMethod {
    FADE_BY_WINDOW = 0,
    FADE_BY_BRIGHTNESS,
};

enum FadeState {
    FADE_IDLE,
    FADE_ACTIVE,
    FADE_CLEANUP,
    FADE_DONE,
};

struct ScreenFadeHBlanks {
    void *data[DS_SCREEN_MAX];
    Callback callback[DS_SCREEN_MAX];
    BOOL running[DS_SCREEN_MAX];
};

struct ScreenFade {
    enum FadeType type;
    int steps;
    int framesPerStep;
    enum FadeState state;
    enum DSScreen screen;
    void *data;
    HardwareWindowSettings *hwSettings;
    ScreenFadeHBlanks *hblanks;
    enum HeapID heapID;
    u16 color;

    enum FadeDirection direction;
    enum FadeMethod method;
};

#endif // POKEHEARTGOLD_SCREEN_FADE_INTERNAL_H
