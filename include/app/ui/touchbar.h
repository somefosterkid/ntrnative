// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/app/ui/touchbar.h: the lines in use so far.
#ifndef BWPORT_APP_UI_TOUCHBAR_H
#define BWPORT_APP_UI_TOUCHBAR_H

#include "types.h"
#include "gfl/clact.h"
#include "gfl/heap.h"

// touchbar.c: The bar of icons at the bottom of the lower screen, such as the return button
typedef struct TouchBar TouchBar;

typedef struct TouchBarIcon TouchBarIcon;

// An icon of the bar: one of the bar's own, or from TOUCHBAR_ICON_CUSTOM on, one drawn from the caller's
// resources and animations. Pressing the key does what touching it does
#define TOUCHBAR_ICON_CUSTOM 7

// Black 2's; twinned code confirms the offsets of icon, pos, charRes, plttRes, cellRes, anims, key and se in Black
typedef struct {
    s32 icon;
    ClActorPos pos;
    u16 charRes;
    u16 plttRes;
    u16 cellRes;
    // The animations of the icon at rest, inactive, pressed and, for an icon that toggles, toggled
    u16 anims[4];
    u16 unk16;
    u32 key;
    u32 se;
} TouchBarItem;

typedef struct {
    TouchBarItem *items;
    u32 count;
    ClActUnit *unit;
    u32 bg;
    u32 bgPalette;
    u32 objPalette;
    u32 vramType;
    BOOL unk1C;
} TouchBarSetup;

// An icon of the bar: its sprite, its state and what to do when it is touched, which depends on its kind
struct TouchBarIcon {
    ClActor *actor;
    BOOL active;
    u32 anim;
    void (*onTouch)(TouchBarIcon *icon);
    void (*setActive)(TouchBarIcon *icon, BOOL active);
    u32 kind;
    TouchBarItem item;
};

// The resources the bar loads: its icons' palette, characters, and cells and animations, and its BG
typedef struct {
    u32 res[3];
    u32 bg;
    u32 bgChars;
} TouchBarGfx;

struct TouchBar {
    s32 selected;
    u32 count;
    u32 state;
    TouchBarGfx gfx;
    TouchBarIcon icons[];
};

// pokebw2's TouchBar_Free
void func_ov067_021b639c(TouchBar *bar);
// pokebw2's TouchBar_Main
void func_ov067_021b63d0(TouchBar *bar);
// pokebw2's TouchBar_GetDecided
// The icon whose touch animation has ended, or -1 when none
int func_ov067_021b6448(TouchBar *bar);
// pokebw2's TouchBar_GetTouched
// The icon being touched, or -1 when none
u32 func_ov067_021b6470(TouchBar *bar);
// pokebw2's TouchBar_SetActive
void func_ov067_021b64a8(TouchBar *bar, BOOL active);
// pokebw2's TouchBar_SetIconActive
void func_ov067_021b6548(TouchBar *bar, u32 icon, BOOL a2);
// pokebw2's TouchBar_Reset
void func_ov067_021b65b4(TouchBar *bar, u32 a1);

// No twin in pokebw2's C: these prototypes are Black 2's, unchecked against Black's code
// pokebw2's TouchBar_Create
TouchBar *func_ov067_021b62e0(TouchBarSetup *setup, HeapID heapId);
// pokebw2's TouchBar_IsTouching
// Whether an icon is being touched
BOOL func_ov067_021b6498(TouchBar *bar);
// pokebw2's TouchBar_SetVisible
void func_ov067_021b64d0(TouchBar *bar, BOOL visible);
// pokebw2's TouchBar_SetBgPriority
void func_ov067_021b64f8(TouchBar *bar, u8 bgPriority);
// pokebw2's TouchBar_SetPriority
void func_ov067_021b6520(TouchBar *bar, u8 priority);
// pokebw2's TouchBar_SetIconVisible
void func_ov067_021b6558(TouchBar *bar, u32 icon, BOOL a2);
// pokebw2's TouchBar_SetIconSe
void func_ov067_021b6568(TouchBar *bar, u32 icon, u32 se);
// pokebw2's TouchBar_SetIconKey
void func_ov067_021b6578(TouchBar *bar, u32 icon, u32 key);
// pokebw2's TouchBar_SetIconBgPriority
void func_ov067_021b6588(TouchBar *bar, u32 icon, u8 bgPriority);
// pokebw2's TouchBar_SetIconPressed
void func_ov067_021b6598(TouchBar *bar, u32 icon, BOOL pressed);
// pokebw2's TouchBar_IsIconPressed
BOOL func_ov067_021b65a8(TouchBar *bar, u32 icon);

#endif // BWPORT_APP_UI_TOUCHBAR_H
