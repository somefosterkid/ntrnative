// From pokebw2 (GPL-3.0, https://github.com/fuddlesworth/pokebw2), src/ov079/c_gear_sw.c, ported by tools/port_file.py.
// The names are Black's; pokebw2's own guesses are in the comments.
#include "types.h"
#include "app/cg_help.h"
#include "constants/arc.h"
#include "constants/sound.h"
#include "field/c_gear.h"
#include "field/c_gear_sw.h"
#include "field/event_cgear_shutdown.h"
#include "field/event_gsync.h"
#include "field/event_irc.h"
#include "field/event_research_radar.h"
#include "field/field.h"
#include "field/field_menu.h"
#include "field/field_sound.h"
#include "field/intrude_work.h"
#include "gfl/arc.h"
#include "gfl/arc_util.h"
#include "gfl/bg_sys.h"
#include "gfl/clact.h"
#include "gfl/graphics.h"
#include "gfl/gx_layers.h"
#include "gfl/heap.h"
#include "gfl/net.h"
#include "gfl/overlay.h"
#include "gfl/sound.h"
#include "gfl/std.h"
#include "gfl/tcb.h"
#include "gfl/tcbl.h"
#include "gfl/touchpanel.h"
#include "gfl/ui.h"
#include "save/c_gear_savedata.h"
#include "nitro/gx.h"
#include "nitro/ioreg.h"
#include "nitro/mi.h"
#include "nitro/os.h"
#include "nitro/rtc.h"
#include "nnsys/g2d.h"
#include "save/cgear_picture_save.h"
#include "save/player_info.h"
#include "save/save_control.h"
#include "system/game_beacon.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/palanm.h"
#include "system/wipe.h"
#include "gfl/rtc_cache.h"
#include "gfl/button_man.h"
// Declarations from the references' headers that the functions below need (tools/ref_decls.py): they
// belong in the library's header


// The C-Gear's touch screen as Black has it (the ROM's "c_gear_sw.c"; "sw" is probably swan, Black 2 and White 2's code
// name): a grid of 9 by 4 cells that light up when touched, three lamps that blink by what the communication is doing,
// and the buttons around them. Only the lamps' and the beacons' code is Black 2's. The names are ours


typedef struct {
    u8 unk_0[0x16];
    u16 colors[5];
} CGearSwColorSrc;

static const u8 data_ov029_021f3204[] = {
#include "asset/c_gear_sw_data_ov029_021f3204.inc"
};

static const u8 data_ov029_021f3292[] = {
#include "asset/c_gear_sw_data_ov029_021f3292.inc"
};

// Each lamp's step of nine (a row a lamp; the first lamp's are all 0): its middle step at data_ov029_021f32b7
static const u8 data_ov029_021f32b3[5][9] = {
#include "asset/c_gear_sw_data_ov029_021f32b3.inc"
};


static const u8 data_ov029_021f3255[] = {
#include "asset/c_gear_sw_data_ov029_021f3255.inc"
};


static const s32 data_ov029_021f3344[] = {
    6, 4,
};



// The color of each lamp's palette entries, as the offset of the first
static const u8 data_ov029_021f31a8[LAMP_COUNT] = { 15, 7, 11 };

static const u8 data_ov029_021f31ab[PATTERN_COUNT] = { 0, 12, 3, 0, 0 };

static const u16 data_ov029_021f31b6[] = { 0x0441, 0x0423, 0x0441 };
static const s32 data_ov029_021f3324[] = { 3, 4 };
static const u8 data_ov029_021f323c[5][5] = {
#include "asset/c_gear_sw_data_ov029_021f323c.inc"
};
static const u8 data_ov029_021f31b0[PATTERN_COUNT] = { TRUE, TRUE, TRUE, TRUE, FALSE };

static const u16 data_ov029_021f31bc[PATTERN_COUNT] = { 23, 160, 80, 40, 16 };

static const ClActSurfaceSetup data_ov029_021f3214[] = {
    { 0, 0, 0x100, 0xc0, 1, 2 },
};

// The palette fade's strength at each frame of the fading actor's animation (c_gear.c's func_ov029_021f2e8c)
const u32 data_ov029_021f3224[6] = { 0, 9, 0, 9, 0, 9 };

// The buttons around the grid
static const TouchRect data_ov029_021f3272[] = {
#include "asset/c_gear_sw_data_ov029_021f3272.inc"
};


// Where the buttons' actors sit, and their animations
static const u8 data_ov029_021f3526[ACTOR_COUNT] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x11, 0x25, 0x0e, 0x26 };
static const u8 data_ov029_021f3531[ACTOR_COUNT] = { 0x3f, 0x20, 0x26, 0x2a, 0x2f, 0x35, 0xb2, 0x80, 0xc6, 0xae, 0xde };
static const u8 data_ov029_021f353c[ACTOR_COUNT] = { 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0xaa, 0xaa, 0xaa };
static const u32 data_ov029_021f3548[LAMP_COUNT] = { 8, 9, 10 };

// The lamps' palette file by gender, and the touch screen's graphics by skin and gender
static const u32 data_ov029_021f31e0[3] = { 13, 6, 13 };
static const u32 data_ov029_021f32e0[5][3] = {
#include "asset/c_gear_sw_data_ov029_021f32e0.inc"
};

// The wireless states the second lamp cycles through, and the palette each shows
static const u32 data_ov029_021f31ec[3] = { 0x20, 0x10, 0 };
static const u32 data_ov029_021f31f8[3] = { 0, 1, 4 };

// The palette of each type of cell
static const u16 data_ov029_021f31d4[] = { 0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0004 };

// The cells of the opening, in the frame each lights up on
typedef struct {
    u8 frame;
    u8 col;
    u8 row;
    u8 max;
} CGearSwOpeningCell;

static const CGearSwOpeningCell data_ov029_021f3410[] = {
#include "asset/c_gear_sw_data_ov029_021f3410.inc"
};

u16 func_ov029_021f1000(CGearSw *work);
void func_ov029_021f1458(CGearSw *work, int gender, int skin, BOOL init);
void func_ov029_021f1950(CGearSw *work);
void func_ov029_021f1a64(CGearSw *work, BOOL visible);
void func_ov029_021f1010(CGearSw *work);
void func_ov029_021f13fc(CGearSw *work);
void func_ov029_021f1574(CGearSw *work, int index);
void func_ov029_021f1a44(CGearSw *work, BOOL visible);
void func_ov029_021f18f0(u32 unk0, int unk1, CGearSw *work);
void func_ov029_021f1c0c(CGearSw *work);
void func_ov029_021f1740(CGearSw *work);
BOOL func_ov029_021f09dc(CGearSw *work, int lamp, int target, int pattern, int unk, int unk2);
BOOL func_ov029_021efec0(int col, int row);

void func_ov029_021f0390(CGearSw *work, int pattern, int lamp, BOOL init, BOOL alert);
void func_ov029_021f08bc(CGearSw *work, int lamp, int fraction, int max);
void func_ov029_021f0ac8(CGearSwCell *cell, int col, int row);
void func_ov029_021f0b7c(CGearSwCell *cell);
void func_ov029_021f0b88(CGearSwCell *cell, CGearSw *work);
BOOL func_ov029_021f0c68(CGearSwCell *cell);
void func_ov029_021f0ca0(CGearSwPattern *pattern, u16 max, u16 start, u8 loop);
void func_ov029_021f0cb8(CGearSwPattern *pattern);
BOOL func_ov029_021f0cdc(CGearSwPattern *pattern, int lamp);
void func_ov029_021f0cf0(CGearSwPattern *pattern, int lamp);
void func_ov029_021f0d00(CGearSwPattern *pattern, int lamp);
u16 func_ov029_021f0d08(CGearSwPattern *pattern, int lamp);
void func_ov029_021f0d10(CGearSwPattern *pattern, u8 active);
void func_ov029_021f07ac(CGearSw *work, int lamp, CGearSwPattern *pattern);
void func_ov029_021f07ec(CGearSw *work, int lamp, CGearSwPattern *pattern);
void func_ov029_021f0824(CGearSw *work, int lamp, CGearSwPattern *pattern);
void func_ov029_021f0858(CGearSw *work, int lamp, CGearSwPattern *pattern);
void func_ov029_021f088c(CGearSw *work, int lamp, CGearSwPattern *pattern);
void func_ov029_021f0c00(CGearSwCell *cell, long index);
void func_ov029_021f1290(CGearSw *work, int col, int row, int value);
BOOL func_ov029_021f0064(CGearSw *work, int x, int y);
void func_ov029_021eff58(CGearSw *work, int col, int row);
void func_ov029_021eff00(int col, int row, int *xs, int *ys);
void func_0204b3b4(ClActor *actor);
BOOL func_0204b98c(ClActor *actor);
void func_ov029_021f1758(CGearSw *work, u32 button);
void func_ov029_021f0d14(int unk0, int unk1, s32 *unk2, s32 *unk3);
CGearSwTouchEffect *func_ov029_021f0000(CGearSw *work);
void func_ov029_021f1910(CGearSw *work, BOOL flag);






BOOL func_ov029_021efec0(int col, int row) {
    if (col % 2 == 0) {
        if (row < 3) {
            return TRUE;
        }
    } else if (row < 4) {
        return TRUE;
    }
    return FALSE;
}

void func_ov029_021efee4(CGearSw *work, void (*state)(CGearSw *)) {
    work->state = state;
    work->counter = 0;
    work->seq = 0;
}

void func_ov029_021efef8(CGearSw *work, void (*state)(CGearSw *), int line) {
    func_ov029_021efee4(work, state);
}



void func_ov029_021eff00(int col, int row, int *xs, int *ys) {
    if (col % 2 == 0) {
        xs[0] = col;
        ys[0] = row - 1;
        xs[1] = col;
        ys[1] = row + 1;
        xs[2] = col + 1;
        ys[2] = row;
        xs[3] = col + 1;
        ys[3] = row - 1;
        xs[4] = col - 1;
        ys[4] = row;
        xs[5] = col - 1;
        ys[5] = row - 1;
    } else {
        xs[0] = col;
        ys[0] = row - 1;
        xs[1] = col;
        ys[1] = row + 1;
        xs[2] = col + 1;
        ys[2] = row;
        xs[3] = col + 1;
        ys[3] = row + 1;
        xs[4] = col - 1;
        ys[4] = row;
        xs[5] = col - 1;
        ys[5] = row + 1;
    }
}

void func_ov029_021eff58(CGearSw *work, int col, int row) {
    ClActorSetup setup;
    s32 x;
    s32 y;
    CGearSwTouchEffect *effect;

    func_ov029_021f0d14(col, row, &x, &y);
    setup.x = x * 8 + 16;
    setup.y = y * 8 + 16;
    setup.sequence = work->touchType + 10;
    setup.priority = 0;
    setup.bgPriority = 2;
    effect = func_ov029_021f0000(work);
    if (effect->actor != NULL) {
        func_0204b878(effect->actor, setup.sequence);
        func_0204b9a0(effect->actor);
        func_0204b49c(effect->actor, setup.x, 0, 0);
        func_0204b49c(effect->actor, setup.y, 0, 1);
    } else {
        effect->actor = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
        func_0204b92c(effect->actor, 1);
        func_0204b3dc(effect->actor, 1);
    }
    effect->x = col;
    effect->y = row;
}


CGearSwTouchEffect *func_ov029_021f0000(CGearSw *work) {
    s32 bestRank = -1;
    s32 bestTime = -1;
    int bestIndex = 0;
    int i;
    s32 rank;
    s32 time;

    for (i = 0; i < TOUCH_EFFECT_COUNT; i++) {
        if (work->touchEffects[i].actor == NULL) {
            return &work->touchEffects[i];
        }
        rank = func_0204b918(work->touchEffects[i].actor);
        time = func_0204b8f4(work->touchEffects[i].actor);
        if (rank > bestRank || (rank == bestRank && time > bestTime)) {
            bestRank = rank;
            bestTime = time;
            bestIndex = i;
        }
    }
    return &work->touchEffects[bestIndex];
}


BOOL func_ov029_021f0064(CGearSw *work, int x, int y) {
    int i;

    for (i = 0; i < TOUCH_EFFECT_COUNT; i++) {
        if (work->touchEffects[i].actor != NULL && x == work->touchEffects[i].x && y == work->touchEffects[i].y
            && func_0204b98c(work->touchEffects[i].actor)) {
            return FALSE;
        }
    }
    return TRUE;
}

BOOL func_ov029_021f009c(CGearSw *work) {
    int i;
    int j;
    int k;
    int xs[6];
    int ys[6];

    for (j = 0; j < CELL_ROWS; j++) {
        for (i = 0; i < CELL_COLUMNS; i++) {
            if (work->cellStates[i][j] == 4 && func_ov029_021f0064(work, i, j)) {
                work->cellStates[i][j] = 5;
            }
            if (work->cellStates[i][j] == 2) {
                func_ov029_021eff58(work, i, j);
                work->cellStates[i][j] = 3;
            }
        }
    }

    work->touchTimer++;
    if (work->touchTimer >= 16) {
        return TRUE;
    }

    for (j = 0; j < CELL_ROWS; j++) {
        for (i = 0; i < CELL_COLUMNS; i++) {
            if (work->cellStates[i][j] == 3) {
                work->cellStates[i][j] = 4;
                func_ov029_021eff00(i, j, xs, ys);
                for (k = 0; k < 6; k++) {
                    if (xs[k] >= 0 && xs[k] < CELL_COLUMNS && ys[k] >= 0 && ys[k] < CELL_ROWS) {
                        if (work->cellStates[xs[k]][ys[k]] == 1) {
                            work->cellStates[xs[k]][ys[k]] = 2;
                        } else if (work->cellStates[xs[k]][ys[k]] == 0) {
                            work->cellStates[xs[k]][ys[k]] = 3;
                        }
                    }
                }
            }
        }
    }
    return FALSE;
}

void func_ov029_021f0194(CGearSw *work) {
    int j;
    int i;
    int value;

    value = func_ov029_021f2b9c(work, work->touchCol, work->touchRow);
    work->touchType = value;
    work->touchTimer = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 9; j++) {
            if (func_ov029_021f2b9c(work, j, i) == value) {
                work->cellStates[j][i] = 1;
            }
        }
    }
    work->cellStates[work->touchCol][work->touchRow] = 2;
    func_ov029_021f009c(work);
}

typedef struct { int v[2]; } CGearSwPair;
int func_ov029_021f020c(CGearSw *work, int x, int y, int *col, int *row) {
    int result = 5;
    int starts[2] = { 6, 4 };
    int counts[2] = { 3, 4 };
    int c, d;

    x = x / 8 + 2;
    c = x / 4;
    if (c < 0) {
        return result;
    }
    y = y / 8 - starts[c % 2];
    if (y < 0) {
        return result;
    }
    d = y / 4;
    if (c < CELL_COLUMNS && d < counts[c % 2]) {
        result = func_ov029_021f2b9c(work, c, d);
    }
    *col = c;
    *row = d;
    return result;
}



void func_ov029_021f0298(CGearSw *work) {
    int i;
    int lamp;

    for (i = 0; i < PATTERN_COUNT; i++) {
        func_ov029_021f0ca0(&work->patterns[i], data_ov029_021f31bc[i], data_ov029_021f31ab[i],
                            data_ov029_021f31b0[i]);
        if (i == 0) {
            func_ov029_021f0d10(&work->patterns[i], FALSE);
        }
    }
    for (i = 0; i < PATTERN_COUNT; i++) {
        for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
            func_ov029_021f0390(work, i, lamp, FALSE, FALSE);
        }
    }
}

void func_ov029_021f0300(CGearSw *work) {
    int i;
    int lamp;

    for (i = 0; i < PATTERN_COUNT; i++) {
        func_ov029_021f0ca0(&work->patterns[i], data_ov029_021f31bc[i], data_ov029_021f31ab[i],
                            data_ov029_021f31b0[i]);
        if (i != 0) {
            func_ov029_021f0d10(&work->patterns[i], FALSE);
        }
    }
    for (i = 0; i < PATTERN_COUNT; i++) {
        for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
            func_ov029_021f0390(work, i, lamp, TRUE, work->alert);
        }
    }
}


void func_ov029_021f0370(CGearSw *work) {
    int lamp;

    for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
        func_ov029_021f08bc(work, lamp, 0, 31);
    }
}

void func_ov029_021f0390(CGearSw *work, int pattern, int lamp, BOOL init, BOOL alert) {
    GameCommSys *comm = GSYS_GetGameCommSystem(work->gsys);
    u32 busy;
    BOOL idle;
    u32 intrude;
    int light;
    int fast;
    int i;
    int twice;
    int slow;
    u32 bits;

    busy = 0;
    idle = TRUE;
    slow = 1;
    light = 4;
    fast = 3;
    twice = 2;
    if (init) {
        fast = 0;
        twice = 0;
        slow = 0;
        light = 0;
    }
    bits = func_0201058c(func_0201283c(GSYS_GetGameData(work->gsys)));
    intrude = func_ov010_02158630(comm);
    if (alert) {
        if (intrude == 3) {
            func_ov029_021f09dc(work, 2, 3, fast, pattern, lamp);
        } else if (bits & 8) {
            busy |= 4;
            func_ov029_021f09dc(work, 2, 2, fast, pattern, lamp);
        }
    }
    if (busy == 0) {
        idle = FALSE;
        if (bits & 2) {
            busy |= 2;
        }
        if (bits & 0x3c) {
            busy |= 4;
        }
        if (bits & 0x3c0) {
            busy |= 8;
        }
        if (bits & 0x40) {
            func_ov029_021f09dc(work, 1, 3, twice, pattern, lamp);
        } else if (bits & 0x100) {
            func_ov029_021f09dc(work, 1, 0, twice, pattern, lamp);
        } else if (bits & 0x80) {
            func_ov029_021f09dc(work, 1, 1, twice, pattern, lamp);
        } else if (bits & 0x200) {
            func_ov029_021f09dc(work, 1, 0, twice, pattern, lamp);
        }
        if (intrude != 0) {
            if (intrude != 3) {
                func_ov029_021f09dc(work, 2, 3, twice, pattern, lamp);
            }
        } else {
            for (i = 0; i < 3; i++) {
                work->wirelessCycle = (work->wirelessCycle + 1) % 3;
                if ((data_ov029_021f31ec[work->wirelessCycle] & bits) || (work->wirelessCycle == 2 && work->newBeacon)) {
                    if (func_ov029_021f09dc(work, 2, data_ov029_021f31f8[work->wirelessCycle], twice, pattern, lamp)) {
                        if (work->wirelessCycle == 2) {
                            work->newBeacon = FALSE;
                        }
                        break;
                    }
                }
            }
            if (i == 3) {
                func_ov029_021f09dc(work, 2, 8, slow, pattern, lamp);
            }
        }
    }
    if (!idle) {
        func_ov029_021f09dc(work, 0, 6, slow, pattern, lamp);
        if (!(busy & 8)) {
            func_ov029_021f09dc(work, 1, 7, slow, pattern, lamp);
        }
        if (!(busy & 4)) {
            func_ov029_021f09dc(work, 2, 8, slow, pattern, lamp);
        }
    } else {
        func_ov029_021f09dc(work, 0, 5, light, pattern, lamp);
        func_ov029_021f09dc(work, 1, 5, light, pattern, lamp);
    }
}


BOOL func_ov029_021f05b4(CGearSw *work) {
    if (work->call == TRUE && work->alert == TRUE) {
        return TRUE;
    }
    return FALSE;
}

void func_ov029_021f05d0(CGearSw *work) {
    int i;
    BOOL restart = FALSE;
    int lamp;

    for (i = 0; i < PATTERN_COUNT; i++) {
        func_ov029_021f0cb8(&work->patterns[i]);
        for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
            if (func_ov029_021f0cdc(&work->patterns[i], lamp) || work->lampsChanged) {
                func_ov029_021f0390(work, i, lamp, FALSE, work->alert);
                if (i == 0) {
                    restart = TRUE;
                }
                if (func_ov029_021f0cdc(&work->patterns[i], lamp)) {
                    func_ov029_021f0cf0(&work->patterns[i], lamp);
                }
            }
        }
    }
    if (restart) {
        for (i = 0; i < PATTERN_COUNT; i++) {
            if (i != 0) {
                func_ov029_021f0d10(&work->patterns[i], TRUE);
                for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
                    func_ov029_021f0390(work, i, lamp, FALSE, work->alert);
                }
            } else {
                func_ov029_021f0d10(&work->patterns[i], FALSE);
            }
        }
    }
    work->lampsChanged = FALSE;
}

void func_ov029_021f06b8(CGearSw *work, BOOL alert, BOOL call) {
    int i;
    int lamp;

    for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
        func_ov029_021f08bc(work, lamp, 0, 0x1f);
    }
    for (i = 0; i < PATTERN_COUNT; i++) {
        for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
            func_ov029_021f0d00(&work->patterns[i], lamp);
        }
    }
    work->alert = alert;
    work->lampsChanged = TRUE;
    work->alertChanged = TRUE;
    work->alertWait = 0;
    work->call = call;
}

void func_ov029_021f0728(CGearSw *work) {
    int lamp;

    for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
        switch (work->lampPatterns[lamp]) {
        case 3:
            func_ov029_021f07ac(work, lamp, &work->patterns[work->lampPatterns[lamp]]);
            break;
        case 1:
            func_ov029_021f07ec(work, lamp, &work->patterns[work->lampPatterns[lamp]]);
            break;
        case 2:
            func_ov029_021f0824(work, lamp, &work->patterns[work->lampPatterns[lamp]]);
            break;
        case 0:
            func_ov029_021f0858(work, lamp, &work->patterns[work->lampPatterns[lamp]]);
            break;
        case 4:
            func_ov029_021f088c(work, lamp, &work->patterns[work->lampPatterns[lamp]]);
            break;
        }
    }
}



void func_ov029_021f07ac(CGearSw *work, int lamp, CGearSwPattern *pattern) {
    u16 count = func_ov029_021f0d08(pattern, lamp);
    int fraction;

    if (count <= 36 && count >= 4) {
        count -= 4;
        fraction = (NELEMS(data_ov029_021f3255) - 1) * (count % 8) / 8;
        fraction = data_ov029_021f3255[fraction];
        func_ov029_021f08bc(work, lamp, fraction, 31);
    }
}


void func_ov029_021f07ec(CGearSw *work, int lamp, CGearSwPattern *pattern) {
    u16 count = func_ov029_021f0d08(pattern, lamp);
    int fraction;

    if (count <= 112 && count >= 12) {
        count -= 12;
        fraction = (NELEMS(data_ov029_021f3255) - 1) * count / 100;
        fraction = data_ov029_021f3255[fraction];
        func_ov029_021f08bc(work, lamp, fraction, 31);
    }
}



void func_ov029_021f0824(CGearSw *work, int lamp, CGearSwPattern *pattern) {
    u16 count = func_ov029_021f0d08(pattern, lamp);
    int fraction;

    if (count <= 78 && count >= 8) {
        count -= 8;
        fraction = (NELEMS(data_ov029_021f3292) - 1) * count / 70;
        fraction = data_ov029_021f3292[fraction];
        func_ov029_021f08bc(work, lamp, fraction, 31);
    }
}


void func_ov029_021f0858(CGearSw *work, int lamp, CGearSwPattern *pattern) {
    u16 count = func_ov029_021f0d08(pattern, lamp);
    int fraction;

    if (count <= 20) {
        fraction = (NELEMS(data_ov029_021f3255) - 1) * (count % 10) / 10;
        fraction = data_ov029_021f3255[fraction];
        func_ov029_021f08bc(work, lamp, fraction, 31);
    }
}




void func_ov029_021f088c(CGearSw *work, int lamp, CGearSwPattern *pattern) {
    u16 count = func_ov029_021f0d08(pattern, lamp);
    int fraction;

    if (count <= 15) {
        fraction = 12 * count / 15U;
        fraction = data_ov029_021f3204[fraction];
        func_ov029_021f08bc(work, lamp, fraction, 31);
    }
}



void func_ov029_021f08bc(CGearSw *work, int lamp, int fraction, int max) {
    int i;
    int channel;

    for (i = 0; i < LAMP_COLOR_COUNT; i++) {
        work->lampColors[lamp][i] = 0;
        for (channel = 0; channel < 3; channel++) {
            int shift = channel * 5;
            int from = (work->lampBaseColors[lamp][i] >> shift) & 0x1f;
            int to = (work->lampPalettes[work->lampTargets[lamp]][i] >> shift) & 0x1f;
            int diff = to - from;

            work->lampColors[lamp][i] |= ((from + diff * fraction / max) & 0x1f) << shift;
        }
        func_020215bc(work->fade, &work->lampColors[lamp][i], 1, data_ov029_021f31a8[lamp] + (i + 1) * 16, 2);
    }
}

void func_ov029_021f09b4(CGearSw *work, const CGearSwColorSrc *src, int index) {
    int i;

    for (i = 0; i < 5; i++) {
        work->lampPalettes[index][i] = src[index].colors[i];
    }
}

BOOL func_ov029_021f09dc(CGearSw *work, int lamp, int target, int pattern, int unk, int unk2) {
    if (pattern == unk && lamp == unk2) {
        work->lampTargets[lamp] = target;
        work->lampPatterns[lamp] = pattern;
        return TRUE;
    }
    return FALSE;
}



void func_ov029_021f0a0c(CGearSw *work) {
    int col;
    int row;

    for (col = 0; col < CELL_COLUMNS; col++) {
        for (row = 0; row < CELL_ROWS; row++) {
            func_ov029_021f0ac8(&work->cells[col][row], col, row);
        }
    }
}

void func_ov029_021f0a38(CGearSw *work) {
    int i;
    int j;

    for (i = 0; i < CELL_COLUMNS; i++) {
        for (j = 0; j < CELL_ROWS; j++) {
            func_ov029_021f0b88(&work->cells[i][j], work);
        }
    }
}


BOOL func_ov029_021f0a68(CGearSw *work) {
    BOOL active = FALSE;
    int col;
    int row;

    for (col = 0; col < CELL_COLUMNS; col++) {
        for (row = 0; row < CELL_ROWS; row++) {
            if (func_ov029_021f0c68(&work->cells[col][row])) {
                active = TRUE;
                break;
            }
        }
    }
    return active;
}

void func_ov029_021f0aa0(CGearSw *work) {
    int i;
    int j;

    for (i = 0; i < CELL_COLUMNS; i++) {
        for (j = 0; j < CELL_ROWS; j++) {
            func_ov029_021f0b7c(&work->cells[i][j]);
        }
    }
}


void func_ov029_021f0ac8(CGearSwCell *cell, int col, int row) {
    MI_CpuFill8(cell, 0, sizeof(CGearSwCell));
    cell->col = col;
    cell->row = row;
}

void func_ov029_021f0ae0(CGearSwCell *cell, CGearSw *work, int unk1) {
    cell->type = 0;
    func_ov029_021f1290(work, cell->col, cell->row, unk1);
    func_ov029_021f0c00(cell, 0);
}

u8 func_ov029_021f0c70(CGearSw *work, int lamp, int a, int b);

void func_ov029_021f0b00(CGearSwCell *cell, CGearSw *work, u8 type, int mode, int value, u16 max) {
    u8 stop;
    u32 flag = 0;

    cell->type = type;
    cell->count = 0;
    cell->active = TRUE;
    cell->max = max;
    if (mode == 0) {
        cell->stop = 0;
    } else if (mode == 1) {
        cell->stop = 4;
    } else if (mode == 2) {
        stop = func_ov029_021f0c70(work, type, cell->col, cell->row);
        cell->stop = stop;
        cell->count = max * 5;
        flag = 4;
    }
    func_ov029_021f1290(work, cell->col, cell->row, value);
    func_ov029_021f0c00(cell, flag);
}








void func_ov029_021f0b7c(CGearSwCell *cell) {
    cell->active = FALSE;
}

void func_ov029_021f0b88(CGearSwCell *cell, CGearSw *work) {
    u32 dividend;
    BOOL flag;

    if (cell->active) {
        cell->count++;
        dividend = (s32)cell->count / cell->max;
        flag = FALSE;
        if (dividend >= 5) {
            dividend = 4 - dividend % 5;
            if (dividend == cell->stop) {
                cell->active = FALSE;
            }
        }
        if (dividend < 5 && (s32)cell->count % cell->max == 0) {
            flag = TRUE;
        }
        if (flag) {
            func_ov029_021f0c00(cell, dividend);
        }
    }
}


void func_ov029_021f0c00(CGearSwCell *cell, long index) {
    int palette;
    s32 x;
    s32 y;
    int width;

    palette = data_ov029_021f323c[cell->type][index];
    func_ov029_021f0d14(cell->col, cell->row, &x, &y);
    width = 4;
    if (x < 0) {
        width = x + 4;
        x = 0;
    } else if (x + 4 > 0x20) {
        width = 0x20 - x;
    }
    GFL_BGSysSetScrPaletteNo(4, x, y, width, 4, palette);
    GFL_BGSysLoadScr(4);
}


BOOL func_ov029_021f0c68(CGearSwCell *cell) {
    return cell->active;
}

u8 func_ov029_021f0c70(CGearSw *work, int lamp, int a, int b) {
    if (func_ov029_021f2bb8(work, a, b)) {
        return data_ov029_021f32b3[lamp][4];
    }
    return data_ov029_021f32b3[lamp][a];
}


// Sets a pattern's counters (the third lamp's first), its length and whether it loops
static inline void CGearSwPattern_Set(CGearSwPattern *pattern, u16 max, u16 count2, u16 count0, u16 count1, u8 loop) {
    pattern->count[2] = count2;
    pattern->count[1] = count1;
    pattern->count[0] = count0;
    pattern->max = max;
    pattern->loop = loop;
    pattern->active = TRUE;
}

void func_ov029_021f0ca0(CGearSwPattern *pattern, u16 max, u16 start, u8 loop) {
    CGearSwPattern_Set(pattern, max, 0, start * 2, start, loop);
}

void func_ov029_021f0cb8(CGearSwPattern *pattern) {
    int lamp;

    if (pattern->active) {
        for (lamp = 0; lamp < LAMP_COUNT; lamp++) {
            if (pattern->count[lamp] < pattern->max) {
                pattern->count[lamp]++;
            }
        }
    }
}

BOOL func_ov029_021f0cdc(CGearSwPattern *pattern, int lamp) {
    if (pattern->count[lamp] >= pattern->max) {
        return TRUE;
    }
    return FALSE;
}

void func_ov029_021f0cf0(CGearSwPattern *pattern, int lamp) {
    if (pattern->loop) {
        pattern->count[lamp] = 0;
    }
}

void func_ov029_021f0d00(CGearSwPattern *pattern, int lamp) {
    pattern->count[lamp] = 0;
}

u16 func_ov029_021f0d08(CGearSwPattern *pattern, int lamp) {
    return pattern->count[lamp];
}

void func_ov029_021f0d10(CGearSwPattern *pattern, u8 active) {
    pattern->active = active;
}

void func_ov029_021f0d14(int unk0, int unk1, s32 *unk2, s32 *unk3) {
    s32 table[2] = { 6, 4 };

    *unk2 = unk0 * 4 - 2;
    *unk3 = table[unk0 % 2] + unk1 * 4;
}

BOOL func_ov029_021f0d48(CGearSw *work) {
    if (!GFL_WipeIsFinished()) {
        return FALSE;
    }
    if (GXx_GetMasterBrightness_(REG_DB_MASTER_BRIGHT_ADDR) != 0) {
        return FALSE;
    }
    if (func_ov021_02189a48(GSYS_GetField(work->gsys)) != 0) {
        return FALSE;
    }
    return TRUE;
}


int func_ov029_021f0d80(CGearSw *work, int type) {
    int i;
    int count;
    int j;

    if (type == 4) {
        return 1;
    }
    count = 0;
    for (j = 0; j < CELL_ROWS; j++) {
        for (i = 0; i < CELL_COLUMNS; i++) {
            if (func_ov029_021f2b9c(work, i, j) == type) {
                count++;
            }
        }
    }
    return count;
}

u16 *func_02021728(PaletteFade *fade, u16 buffer);
void func_ov029_021f0db4(CGearSw *work, BOOL on) {
    u16 *dst;
    u16 *src;
    int i;

    if (on == FALSE) {
        dst = func_02021728(work->fade, 3);
        src = func_02021730(work->fade, 3);
        for (i = 0; i < 14; i++) {
            if (0xfbee & (1 << i)) {
                func_02021f64(dst + i * 0x10, dst + i * 0x10, 0x10, 8, data_ov029_021f31b6[work->gender]);
                MI_CpuCopy8(dst + i * 0x10, src + i * 0x10, 0x20);
            }
        }
        dst = func_02021728(work->fade, 1);
        src = func_02021730(work->fade, 1);
        for (i = 0; i < 14; i++) {
            if (0x3e & (1 << i)) {
                func_02021f64(dst + i * 0x10, dst + i * 0x10, 0x10, 8, data_ov029_021f31b6[work->gender]);
                MI_CpuCopy8(dst + i * 0x10, src + i * 0x10, 0x20);
            }
        }
    }
}


u8 func_ov029_021f0e70(CGearSw *work) {
    return func_0202173c(work->fade, 8, 0xffff, -0x78, 0, 0x10, data_ov029_021f31b6[work->gender], work->tcbMgr);
}

void func_ov029_021f0eb0(CGearSw *work, BOOL on) {
    int end = 8;

    if (on == FALSE) {
        end = 0;
    }
    func_0202173c(work->fade, 8, 0xffff, -0x78, 0, end, data_ov029_021f31b6[work->gender], work->tcbMgr);
    func_0202173c(work->fade, 2, 0x3e, -0x78, 0, end, data_ov029_021f31b6[work->gender], work->tcbMgr);
}


u8 func_ov029_021f0f28(CGearSw *work, BOOL flag) {
    u32 end;

    end = flag ? 8 : 0;
    return func_0202173c(work->fade, 8, 0x3c0, -0x78, 0, (u8)end, data_ov029_021f31b6[work->gender], work->tcbMgr);
}


u8 func_ov029_021f0f70(CGearSw *work) {
    return func_0202173c(work->fade, 8, 0xffff, 1, 0, 0x10, data_ov029_021f31b6[work->gender], work->tcbMgr);
}

u8 func_ov029_021f0fb0(CGearSw *work) {
    return func_0202173c(work->fade, 8, 0xffff, 1, 0x10, 0, data_ov029_021f31b6[work->gender], work->tcbMgr);
}






void func_ov029_021f0ff0(CGearSw *work) {
    func_02021b18(work->fade);
}

u16 func_ov029_021f1000(CGearSw *work) {
    return func_02021ed8(work->fade);
}

void func_ov029_021f1010(CGearSw *work) {
    s32 counts[2] = { 3, 4 };
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < counts[i % 2]; j++) {
            func_ov029_021f1290(work, i, j, 0);
        }
    }
    GFL_BGSysLoadScr(4);
}

BOOL func_ov029_021f1060(CGearSw *work) {
    int counts[2] = { 3, 4 };
    int i;
    int j;

    switch (work->seq) {
    case 0:
        for (i = 0; i < CELL_COLUMNS; i++) {
            for (j = 0; j < counts[i % 2]; j++) {
                func_ov029_021f0b00(&work->cells[i][j], work, 4, 1, 4, 2);
            }
        }
        work->seq++;
        break;
    case 1:
        if (!func_ov029_021f0a68(work)) {
            work->seq++;
        }
        break;
    case 2:
        for (i = 0; i < NELEMS(data_ov029_021f3410); i++) {
            const CGearSwOpeningCell *entry = &data_ov029_021f3410[i];

            if (work->counter == entry->frame) {
                int row = entry->row;
                int col = entry->col;
                int type = func_ov029_021f2b9c(work, col, row);

                func_ov029_021f0b00(&work->cells[col][row], work, data_ov029_021f31d4[type], 2, type, entry->max);
            }
        }
        if (work->counter >= 4) {
            func_ov029_021f0fb0(work);
            func_ov029_021f1a44(work, TRUE);
            work->seq++;
        }
        work->counter++;
        break;
    case 3:
        if (!func_ov029_021f0a68(work) && !func_ov029_021f1000(work)) {
            return TRUE;
        }
        break;
    }
    func_ov029_021f0a38(work);
    return FALSE;
}


// The cells the startup lights, by frame: the frame, the cell's column and row, and its max
typedef struct {
    u8 frame;
    u8 col;
    u8 row;
    u8 max;
} CGearSwStartupCell;

static const CGearSwStartupCell data_ov029_021f348c[] = {
#include "asset/c_gear_sw_data_ov029_021f348c.inc"
};

void func_ov029_021f0b00(CGearSwCell *cell, CGearSw *work, u8 type, int mode, int value, u16 max);

BOOL func_ov029_021f11b4(CGearSw *work) {
    u32 i;

    if (work->counter <= 0x7e) {
        for (i = 0; i < 0x24; i++) {
            if (work->counter == data_ov029_021f348c[i].frame) {
                func_ov029_021f0b00(&work->cells[data_ov029_021f348c[i].col][data_ov029_021f348c[i].row], work, 4, 0, 4, data_ov029_021f348c[i].max);
            }
        }
        work->counter++;
    } else if (func_ov029_021f0a68(work) == 0) {
        return TRUE;
    }
    func_ov029_021f0a38(work);
    return FALSE;
}


void func_ov029_021f122c(CGearSw *work) {
    s32 counts[2] = { 3, 4 };
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < counts[i % 2]; j++) {
            func_ov029_021f0ae0(&work->cells[i][j], work, 4);
        }
    }
}

void func_ov029_021f1290(CGearSw *work, int col, int row, int value) {
    u32 offsets[6] = { 0x18, 0x0c, 0x10, 0x14, 0x18, 0x1c };
    u16 *scr;
    int x;
    int y;
    int j;
    int k;
    int xx;
    u16 pal;
    u8 shade;
    int kind;

    scr = GFL_BGSysIsScrHeapExists(4);
    func_ov029_021f0d14(col, row, &x, &y);
    pal = data_ov029_021f31d4[value];
    kind = func_ov029_021f0c70(work, pal, col, row);
    shade = data_ov029_021f323c[pal][kind];
    for (k = 0; k < 4; k++, y++) {
        for (j = 0, xx = x; j < 4; j++, xx++) {
            if (xx >= 0 && xx < 0x20) {
                scr[y * 0x20 + xx] = (offsets[value] + k * 0x20 + j) | (shade << 12);
            }
        }
    }
}

BOOL func_ov029_021f133c(CGearSw *work, int value, int *a, int *b) {
    int col;
    int row;
    int foundCol = 0;
    int foundRow = 0;
    BOOL found = FALSE;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 9; col++) {
            if (func_ov029_021f2b9c(work, col, row) == value) {
                if (!found) {
                    foundCol = col;
                    foundRow = row;
                    found = TRUE;
                }
                if (func_ov029_021f2bb8(work, col, row)) {
                    func_ov029_021f0d14(col, row, a, b);
                    return TRUE;
                }
                if (func_ov029_021f2bd4(work, col, row, value)) {
                    foundCol = col;
                    foundRow = row;
                    found = TRUE;
                }
            }
        }
    }
    if (found) {
        func_ov029_021f0d14(foundCol, foundRow, a, b);
        func_020091ac(work->settings->unk_00, foundCol, foundRow, value, 1, 0);
        func_ov029_021f1290(work, foundCol, foundRow, value);
        GFL_BGSysLoadScr(4);
        return TRUE;
    }
    return FALSE;
}




void func_ov029_021f13fc(CGearSw *work) {
    s32 counts[2] = { 3, 4 };
    s32 i;
    s32 j;
    int value;

    for (i = 0; i < 9; i++) {
        for (j = 0; j < counts[i % 2]; j++) {
            value = func_ov029_021f2b9c(work, i, j);
            func_ov029_021f1290(work, i, j, value);
        }
    }
    GFL_BGSysLoadScr(4);
}

void func_ov029_021f1458(CGearSw *work, int gender, int skin, BOOL init) {
    int i;
    void *buf;
    NNSG2dPaletteData *palette;
    u32 offset;
    int j;
    int k;
    u16 *colors;

    if (init) {
        buf = GFL_G2DIOReadNCLRArc(work->arc, data_ov029_021f31e0[gender], &palette, HEAPID_TAIL(0x70));
        DC_FlushRange(palette->pRawData, palette->szByte);
        GXS_LoadBGPltt(palette->pRawData, 0, palette->szByte);
        colors = palette->pRawData;
        for (i = 0; i < LAMP_COLOR_COUNT; i++) {
            for (j = 0; j < LAMP_COUNT; j++) {
                work->lampBaseColors[j][i] = colors[(i + 1) * 16 + data_ov029_021f31a8[j]];
            }
        }
        GFL_HeapFree(buf);
        buf = GFL_G2DIOReadNCLRArc(work->arc, 0x17, &palette, HEAPID_TAIL(0x70));
        for (k = 0; k < PALETTE_COUNT; k++) {
            func_ov029_021f09b4(work, palette->pRawData, k);
        }
        GFL_HeapFree(buf);
    }
    offset = GFL_BGSysAllocChar(6, 0x5000, FALSE);
    work->charBlock = (offset & 0xffff) | (0x5000 << 16);
    GFL_BGSysLoadArcNCGRStatic(work->arc, data_ov029_021f32e0[skin][gender], 6, offset, 0, FALSE, 0x70);
    GFL_G2DIOLoadNSCRSync(work->arc, 2, 6, 0, work->charBlock & 0xffff, 0, FALSE, 0x70);
}


static const u32 data_ov029_021f31c8[] = { 0x13, 0x0c, 0x13 };
static const u32 data_ov029_021f33cc[] = {
#include "asset/c_gear_sw_data_ov029_021f33cc.inc"
};
void func_ov029_021f1574(CGearSw *work, int index) {
    NNSG2dPaletteData *palette;
    void *ptr;
    u32 *colors;
    int i;

    work->palette = func_0204abf0(work->arc, data_ov029_021f31c8[index], 1, 0, 0x70);
    work->chars = func_0204a6c8(work->arc, 0x16, FALSE, 1, 0x70);
    work->cellAnims = func_0204af28(work->arc, 0x15, 0x14, work->heapId);
    ptr = GFL_G2DIOReadNCLRArc(work->arc, 0x18, &palette, work->heapId);
    colors = palette->pRawData;
    for (i = 0; i < 17; i++) {
        work->beaconColors[i] = colors[data_ov029_021f33cc[i]];
    }
    GFL_HeapFree(ptr);
    func_0203d88c(0xda, 0xe);
    func_0203d34c(FALSE, work->heapId);
}




void func_ov029_021f15fc(CGearSw *work, int unk) {
    if (unk == 0) {
        GFL_G2DIOLoadNSCRSync(work->arc, 1, 5, 0, work->charBlock & 0xffff, 0, FALSE, 0x70);
    }
}

void func_ov029_021f1624(CGearSw *work) {
    int bg;

    for (bg = 4; bg <= 7; bg++) {
        GFL_BGSysSetBGEnabled(bg, FALSE);
    }
    {
        BGSetup setup = {
            0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7000, GX_BG_CHARBASE_0x00000, 0x6000,
            GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
        };

        GFL_BGSysCreateBG(6, &setup, 0);
        GFL_BGSysSetBGEnabled(6, TRUE);
        GFL_BGSysSetBGPriority(6, 3);
        GFL_BGSysFillScrArea(6, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(6);
    }
    {
        BGSetup setup = {
            0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x7800, GX_BG_CHARBASE_0x00000, 0x6000,
            GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
        };

        GFL_BGSysCreateBG(5, &setup, 0);
        GFL_BGSysSetBGEnabled(5, TRUE);
        GFL_BGSysSetBGPriority(5, 1);
        GFL_BGSysFillScrArea(5, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(5);
    }
    {
        BGSetup setup = {
            0, 0, 0x800, 0, BGRES_256x256, GX_BG_COLORMODE_16, GX_BG_SCRBASE_0x6800, GX_BG_CHARBASE_0x00000, 0x6000,
            GX_BG_EXTPLTT_01, 0, GX_BG_AREAOVER_XLU, FALSE,
        };

        GFL_BGSysCreateBG(4, &setup, 0);
        GFL_BGSysSetBGEnabled(4, TRUE);
        GFL_BGSysSetBGPriority(4, 2);
        GFL_BGSysFillScrArea(4, 0, 0, 0, 32, 32, BGSYS_FILL_TILE_PALETTE);
        GFL_BGSysLoadScr(4);
    }
    func_ov029_021f1740(work);
    GXS_SetVisibleWnd(0);
}



void func_ov029_021f1740(CGearSw *work) {
    G2S_SetBlendAlpha(2, 0x15, 9, 0x10);
}

void func_ov029_021f1758(CGearSw *work, u32 button) {
    int col;
    int row;
    u32 x;
    u32 y;
    int type;

    if (!work->beacons && button != 6 && button != 0) {
        return;
    }
    if (work->touchAction != 0) {
        return;
    }
    func_020355d8(TRUE);
    switch (button) {
    case 0:
        func_02035cb8(&x, &y);
        type = func_ov029_021f020c(work, x, y, &col, &row);
        if (work->unk_457) {
            work->unk_2cc[0] = x;
            work->unk_2cc[1] = y;
            if (func_ov029_021f0d80(work, type) > 1 || type == 4) {
                if (func_ov029_021efec0(col, row)) {
                    func_ov029_021f2aec(work, work->settings, col, row, (type + 1) % 5);
                    GFL_BGSysLoadScr(4);
                    GFL_SndSEPlay(0x628);
                }
            }
        } else if (func_ov029_021f2da8(work) == FALSE) {
            work->touchCol = col;
            work->touchRow = row;
            switch (type) {
            case 1:
                work->touchAction = 1;
                break;
            case 3:
                work->touchAction = 6;
                break;
            case 2:
                work->touchAction = 5;
                break;
            }
        }
        break;
    case 1:
        GFL_SndSEPlay(0x628);
        work->unk_457 ^= 1;
        func_ov029_021f1910(work, work->unk_457);
        break;
    case 2:
        work->touchAction = 12;
        break;
    case 3:
        work->touchAction = 7;
        break;
    case 4:
        work->touchAction = 11;
        break;
    case 5:
        GFL_SndSEPlay(0x628);
        GFL_BGSysFreeCharMemory(6, (u16)work->charBlock, (u16)(work->charBlock >> 16));
        work->skin++;
        work->skin %= 5;
        func_020090e8(work->settings, work->skin);
        func_ov029_021f1458(work, work->gender, work->skin, FALSE);
        func_ov029_021f13fc(work);
        func_ov029_021f1a44(work, TRUE);
        break;
    case 6:
        work->touchAction = 13;
        break;
    }
}


void func_ov029_021f18f0(u32 unk0, int unk1, CGearSw *work) {
    if (func_ov029_021f2da8(work) == 0 && unk1 == 0) {
        func_ov029_021f1758(work, unk0);
    }
}

void func_ov029_021f1910(CGearSw *work, BOOL flag) {
    if (flag) {
        func_0204b908(work->actors[9], 0);
        func_0204b954(work->actors[9], 0x2000);
        func_0204b92c(work->actors[9], TRUE);
    } else {
        func_0204b92c(work->actors[9], FALSE);
        func_0204b908(work->actors[9], 1);
    }
}

void func_ov029_021f1950(CGearSw *work) {
    ClActorSetup setup;
    int i;

    GFL_BGSysSetBGEnabledB(GX_PLANEMASK_OBJ, TRUE);
    for (i = 0; i < ACTOR_COUNT; i++) {
        setup.x = data_ov029_021f3531[i];
        setup.y = data_ov029_021f353c[i];
        setup.sequence = data_ov029_021f3526[i];
        if (setup.sequence == 6 && OS_IsRunOnTwl()) {
            setup.sequence = 7;
        }
        setup.priority = 0;
        setup.bgPriority = 2;
        work->actors[i] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
        func_0204b3dc(work->actors[i], FALSE);
        func_0204ba20(work->actors[i], 0);
    }
    func_ov029_021f1910(work, 0);
    for (i = 0; i < LAMP_COUNT; i++) {
        setup.x = 8;
        setup.y = 8;
        setup.sequence = data_ov029_021f3548[i];
        setup.priority = 0;
        setup.bgPriority = 2;
        work->lampActors[i] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
        func_0204b3dc(work->lampActors[i], FALSE);
        func_0204ba20(work->lampActors[i], 0);
    }
    func_ov029_021f1f80(work);
    func_ov029_021f20ac(work);
}



void func_ov029_021f1a44(CGearSw *work, BOOL visible) {
    int i;

    for (i = 0; i < LAMP_COUNT; i++) {
        func_0204b3dc(work->lampActors[i], visible);
    }
}

void func_ov029_021f1a64(CGearSw *work, BOOL visible) {
    int i;

    for (i = 0; i < TOUCH_EFFECT_COUNT; i++) {
        if (work->touchEffects[i].actor != NULL) {
            func_0204b3dc(work->touchEffects[i].actor, visible);
        }
    }
    for (i = 0; i < LAMP_COUNT; i++) {
        if (work->lampActors[i] != NULL && work->beacons) {
            func_0204b3dc(work->lampActors[i], visible);
        }
    }
    for (i = 0; i < ACTOR_COUNT; i++) {
        if (work->actors[i] != NULL) {
            if (i == 8 || i == 9) {
                if (work->beacons) {
                    func_0204b3dc(work->actors[i], visible);
                }
            } else {
                func_0204b3dc(work->actors[i], visible);
            }
        }
    }
    if (work->unk_164[0] != NULL && work->beacons) {
        func_0204b3dc(work->unk_164[0], visible);
    }
    if (work->unk_164[1] != NULL && work->beacons) {
        func_0204b3dc(work->unk_164[1], visible);
    }
    if (work->unk_164[2] != NULL && work->beacons) {
        func_0204b3dc(work->unk_164[2], visible);
    }
    if (visible == TRUE) {
        func_ov029_021f1f80(work);
    }
}





void func_ov029_021f1b30(CGearSw *work, BOOL visible) {
    int i;

    for (i = 0; i < CELL_ACTOR_COUNT; i++) {
        func_0204b3dc(work->cellActors[i], visible);
    }
}

void func_ov029_021f1b50(CGearSw *work) {
    ClActorSetup setup;
    int i;

    for (i = 0; i < CELL_ACTOR_COUNT; i++) {
        setup.x = 0x80;
        setup.y = 0x60;
        setup.sequence = i + 0x12;
        setup.priority = 0;
        setup.bgPriority = 2;
        work->cellActors[i] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
        func_0204b3dc(work->cellActors[i], 0);
        func_0204b954(work->cellActors[i], 0x2000);
        func_0204b9a0(work->cellActors[i]);
        func_0204b92c(work->cellActors[i], 1);
    }
}


void func_ov029_021f1bbc(CGearSw *work) {
    int i;

    if (work->cellActors[0] != NULL) {
        for (i = 0; i < CELL_ACTOR_COUNT; i++) {
            func_0204b3b4(work->cellActors[i]);
            work->cellActors[i] = NULL;
        }
    }
}

BOOL func_ov029_021f1be4(CGearSw *work) {
    int i;

    for (i = 0; i < CELL_ACTOR_COUNT; i++) {
        if (func_0204b98c(work->cellActors[i]) == TRUE) {
            return FALSE;
        }
    }
    return TRUE;
}

void func_ov029_021f1c0c(CGearSw *work) {
    ClActorSetup setup;

    setup.x = 0x68;
    setup.y = 0xaa;
    setup.sequence = 0x28;
    setup.priority = 0;
    setup.bgPriority = 2;
    work->unk_164[0] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
    func_0204b3dc(work->unk_164[0], FALSE);
    func_0204ba20(work->unk_164[0], 0);

    setup.x = 0x68;
    setup.y = 0xaa;
    setup.sequence = 0x27;
    setup.priority = 0x14;
    setup.bgPriority = 2;
    work->unk_164[1] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
    func_0204b3dc(work->unk_164[1], FALSE);
    func_0204ba20(work->unk_164[1], 0);

    setup.x = 0x22;
    setup.y = 0xaa;
    setup.sequence = 0x10;
    setup.priority = 0;
    setup.bgPriority = 2;
    work->unk_164[2] = func_0204b294(work->unit, work->chars, work->palette, work->cellAnims, &setup, 0, work->heapId);
    func_0204b3dc(work->unk_164[2], FALSE);
    func_0204ba20(work->unk_164[2], 0);
}

void func_ov029_021f1cd8(CGearSw *work) {
    int beaconNo;
    int shown = 0;
    int index;
    int i;

    if (!work->beacons) {
        return;
    }
    if (func_ov029_021f05b4(work)) {
        return;
    }
    beaconNo = func_02026c8c();
    if (!work->newBeacon) {
        index = 0;
        if (func_02026ca0(&index) != 30) {
            work->newBeacon = TRUE;
        }
    }
    beaconNo--;
    for (i = 0; i < 30; i++) {
        if (shown >= 10) {
            break;
        }
        if (beaconNo < 0) {
            func_020215bc(work->fade, &work->beaconColors[16], PALFADE_BUFFER_SUB_OBJ,
                          0x60 + data_ov029_021f33cc[shown] * 2, 4);
            shown++;
        } else {
            GameBeacon *beacon = func_02026c78(beaconNo);

            if (beacon != NULL) {
                func_020215bc(work->fade, &work->beaconColors[func_ov010_02165cd8(beacon)], PALFADE_BUFFER_SUB_OBJ,
                              0x60 + data_ov029_021f33cc[shown] * 2, 4);
                shown++;
            }
            beaconNo--;
        }
    }
}



void func_ov029_021f1db0(CGearSw *work) {
    if (work->unk_164[0] != NULL) {
        func_0204b3b4(work->unk_164[0]);
        work->unk_164[0] = NULL;
    }
    if (work->unk_164[1] != NULL) {
        func_0204b3b4(work->unk_164[1]);
    }
    if (work->unk_164[2] != NULL) {
        func_0204b3b4(work->unk_164[2]);
        work->unk_164[2] = NULL;
    }
}

void func_ov029_021f1de8(CGearSw *work, BOOL startup) {
    BOOL visible = FALSE;

    work->gender = getTrainerGender(GetGameDataPlayerInfo(GSYS_GetGameData(work->gsys)));
    work->skin = func_020090f0(work->settings);
    work->unit = func_0204b100(0x44, 0, work->heapId);
    work->renderer = func_0204b034(data_ov029_021f3214, 1, work->heapId);
    func_0204b258(work->unit, work->renderer);
    func_ov029_021f1458(work, work->gender, work->skin, TRUE);
    func_ov029_021f1574(work, work->gender);
    func_ov029_021f1950(work);
    func_ov029_021f1c0c(work);
    work->buttonMan = func_02051b84(data_ov029_021f3272, (ButtonManCallback)func_ov029_021f18f0, work, work->heapId);
    if (startup) {
        func_ov029_021f1010(work);
        func_ov029_021f1a64(work, visible);
    } else {
        func_ov029_021f13fc(work);
        func_ov029_021f1a64(work, TRUE);
        func_ov029_021f1a44(work, TRUE);
    }
}


void func_ov029_021f1e98(CGearSw *work) {
    int i;

    func_ov029_021f1db0(work);
    func_ov029_021f1bbc(work);
    for (i = 0; i < 11; i++) {
        if (work->actors[i] != NULL) {
            func_0204b3b4(work->actors[i]);
            work->actors[i] = NULL;
        }
    }
    for (i = 0; i < 3; i++) {
        if (work->lampActors[i] != NULL) {
            func_0204b3b4(work->lampActors[i]);
            work->lampActors[i] = NULL;
        }
    }
    for (i = 0; i < TOUCH_EFFECT_COUNT; i++) {
        if (work->touchEffects[i].actor != NULL) {
            func_0204b3b4(work->touchEffects[i].actor);
            work->touchEffects[i].actor = NULL;
        }
    }
    func_0204afd8(work->cellAnims);
    func_0204a8d4(work->chars);
    func_0204ada4(work->palette);
    GFL_BGSysFreeCharMemory(6, (u16)work->charBlock, (u16)(work->charBlock >> 16));
}


void func_ov029_021f1e98(CGearSw *work);
void func_ov029_021f1f24(CGearSw *work) {
    if (work->buttonMan != NULL) {
        func_02051c6c(work->buttonMan);
    }
    func_ov029_021f1e98(work);
    func_0204b084(work->renderer);
    func_0204b1cc(work->unit);
    GFL_BGSysReleaseBG(4);
    GFL_BGSysReleaseBG(5);
    GFL_BGSysReleaseBG(6);
    GFL_BGSysSetBGEnabled(4, 0);
    GFL_BGSysSetBGEnabled(5, 0);
    GFL_BGSysSetBGEnabled(6, 0);
}






static u8 data_ov029_021f3520[8] = { 0, 0, 1, 1, 2, 3 };

u8 func_ov029_021f1f70(void) {
    return data_ov029_021f3520[func_0203568c()];
}
