// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), include/field/c_gear_sw.h: the lines in use so far.
#ifndef BWPORT_FIELD_C_GEAR_SW_H
#define BWPORT_FIELD_C_GEAR_SW_H

#include "types.h"
#include "struct_decls.h"
#include "gfl/clact.h"
#include "gfl/button_man.h"
#include "system/palanm.h"

typedef struct CGearSw CGearSw;

void func_ov029_021f0194(CGearSw *work);

// pokebw2's: Black's func_ov029_021f27d8 stores it with its work and calls it the same way
typedef void (*CGearSwCallback)(void *work);

#define LAMP_COUNT 3
#define PATTERN_COUNT 5
#define PALETTE_COUNT 9
#define LAMP_COLOR_COUNT 5
#define CELL_COLUMNS 9
#define CELL_ROWS 4
#define TOUCH_EFFECT_COUNT 20
#define ACTOR_COUNT 11
#define CELL_ACTOR_COUNT 18
#define BEACON_COLOR_COUNT 17

// A cell of the grid. active and count are the bits of one halfword
typedef struct {
    u16 active : 1;
    u16 count : 15;
    u16 max;
    u8 type;
    u8 stop;
    u8 col;
    u8 row;
} CGearSwCell;

// A blinking pattern: a counter for each lamp, up to max, which starts over if the pattern loops
typedef struct {
    u16 count[LAMP_COUNT];
    u16 max;
    u8 loop;
    u8 active;
} CGearSwPattern;

typedef struct {
    ClActor *actor;
    int x;
    int y;
} CGearSwTouchEffect;

// A fade the C-Gear runs on the palette: 0x14 bytes at unk_21C
typedef struct {
    u32 active;
    u32 paletteMask;
    ClActor *actor;
    PaletteFade *fade;
    TCBManager *tcbMgr;
} CGearSwFade;

struct CGearSw {
    void (*state)(CGearSw *work);
    u32 unk_4;
    u32 unk_8;
    u16 heapId;
    ButtonMan *buttonMan;
    u32 charBlock;
    FieldSubscreen *unk_18;
    GameSystem *gsys;
    CGearSettings *settings;
    ArcTool *arc;
    u32 palette;
    u32 chars;
    u32 cellAnims;
    ClActUnit *unit;
    ClActRenderer *renderer;
    CGearSwTouchEffect touchEffects[TOUCH_EFFECT_COUNT];
    ClActor *actors[ACTOR_COUNT];
    ClActor *lampActors[LAMP_COUNT];
    ClActor *unk_164[3];
    ClActor *cellActors[CELL_ACTOR_COUNT];
    // Two colors of the palette file each
    u32 beaconColors[BEACON_COLOR_COUNT];
    // Called with callbackWork once the C-Gear shows (func_ov029_021f27d8)
    CGearSwCallback callback;
    void *callbackWork;
    TCBManager *tcbMgr;
    // The V-blank task, and the memory of tcbMgr
    TCB *unk_208;
    void *unk_20c;
    PaletteFade *fade;
    u32 touchAction;
    u32 unk_218;
    CGearSwFade unk_21C;
    // The lamps' colors: unlit, as shown, and the pattern each fades to
    u16 lampBaseColors[LAMP_COUNT][LAMP_COLOR_COUNT];
    u16 lampColors[LAMP_COUNT][LAMP_COLOR_COUNT];
    u16 lampTargets[LAMP_COUNT];
    u16 lampPalettes[PALETTE_COUNT][LAMP_COLOR_COUNT];
    u16 unk_2cc[2];
    CGearSwCell cells[CELL_COLUMNS][CELL_ROWS];
    u8 cellStates[CELL_COLUMNS][CELL_ROWS];
    u8 touchType;
    u8 touchTimer;
    u8 unk_416;
    u8 unk_417[0x1];
    CGearSwPattern patterns[PATTERN_COUNT];
    // The pattern each lamp blinks by
    u8 lampPatterns[LAMP_COUNT];
    u8 wirelessCycle;
    u8 unk_44e[2];
    u8 newBeacon;
    u8 unk_451;
    u8 touchCol;
    u8 touchRow;
    u8 unk_454;
    u8 seq;
    u8 counter;
    u8 unk_457;
    u8 skin;
    u8 gender;
    // Whether the game shows other players' beacons
    u8 beacons;
    u8 unk_45B;
    u8 unk_45C;
    u8 unk_45D;
    u8 unk_45E;
    u8 unk_45F;
    u8 lampsChanged;
    // Whether someone asks for the player, and whether it is a call
    u8 alert;
    u8 unk_462;
    u8 unk_463;
    u8 call;
    u8 alertChanged;
    u8 alertWait;
}; // size: 0x468

void func_ov029_021eff58(CGearSw *work, int col, int row);
BOOL func_ov029_021f05b4(CGearSw *work);
u8 func_ov029_021f0f28(CGearSw *work, BOOL flag);
void func_ov029_021f0370(CGearSw *work);
void func_ov029_021f1a44(CGearSw *work, BOOL visible);
void func_ov029_021f1a64(CGearSw *work, BOOL visible);
void func_ov029_021f1bbc(CGearSw *work);
void func_ov029_021f13fc(CGearSw *work);
void func_ov029_021f0298(CGearSw *work);
BOOL func_ov029_021f009c(CGearSw *work);
u8 func_ov029_021f1f70(void);
void func_ov029_021f1b50(CGearSw *work);
void func_ov029_021f1b30(CGearSw *work, BOOL visible);
BOOL func_ov029_021f1be4(CGearSw *work);
u8 func_ov029_021f0f70(CGearSw *work);
BOOL func_ov029_021f11b4(CGearSw *work);
void func_ov029_021f122c(CGearSw *work);
void func_ov029_021f0aa0(CGearSw *work);
void func_ov029_021f1624(CGearSw *work);
void func_ov029_021f1de8(CGearSw *work, BOOL startup);
void func_ov029_021f15fc(CGearSw *work, int unk);
void func_ov029_021f0a0c(CGearSw *work);
void func_ov029_021f0db4(CGearSw *work, BOOL on);
// Sets the state (and clears seq and counter); line is the caller's source line, unused
void func_ov029_021efef8(CGearSw *work, void (*state)(CGearSw *work), int line);
void func_ov029_021f0300(CGearSw *work);
void func_ov029_021f05d0(CGearSw *work);
void func_ov029_021f06b8(CGearSw *work, BOOL alert, BOOL call);
void func_ov029_021f0728(CGearSw *work);
BOOL func_ov029_021f0d48(CGearSw *work);
u8 func_ov029_021f0e70(CGearSw *work);
void func_ov029_021f0eb0(CGearSw *work, BOOL on);
void func_ov029_021f0ff0(CGearSw *work);
void func_ov029_021f1010(CGearSw *work);
BOOL func_ov029_021f1060(CGearSw *work);
void func_ov029_021f1290(CGearSw *work, int col, int row, int value);
// The screen position (a, b) of the cell set for panel value; with none set, sets one that has it. FALSE if none has
BOOL func_ov029_021f133c(CGearSw *work, int value, int *a, int *b);
void func_ov029_021f1cd8(CGearSw *work);
void func_ov029_021f1f24(CGearSw *work);

#endif // BWPORT_FIELD_C_GEAR_SW_H
