// The lockstep tester's port half (docs/lockstep.md): BWPORT_LSTEP=OUT.jsonl[,FRAMES] makes the exe write one JSON line
// per game frame from the anchor on, the same record the melonDS side (<ref>/lockstep/lstep_melon) writes, from the
// same ROM, save, clock and key script.
//
//   BWPORT_LSTEP=OUT.jsonl[,FRAMES]   on; FRAMES records (default: the last key of the INPUTS + 300)
//   BWPORT_LSTEP_INPUTS=FILE|"TOKENS"  the INPUTS script (BWPORT_KEYS tokens KEY[*fHOLD]@fN, N in game frames after the
//                                      anchor; `#` comments in a file)
//   BWPORT_LSTEP_BOOT="..."            the boot keys, game frames from power-on (default: start@f600 start@f900
//                                      start@f1300 a@f1700)
//   BWPORT_LSTEP_PNG=DIR[,A-B]         also write DIR/fNNNNNN.png (256x384 RGB, top screen above) for frames A-B
//   BWPORT_LSTEP_SKIP=N                the screens (hash, grid, PNG) only on frames f % N == 0 (null on the others; the
//                                      vars are in every record). Sets BWPORT_FRAMESKIP=N too: the bridge then draws only
//                                      those, the V-blank waits' renders one frame in N and the capture frames (the game
//                                      reads those back). Default 1: every frame (unchanged)
//   BWPORT_LSTEP_SAVE=NAME             the header's "save" (default "save.bin")
//   BWPORT_LSTEP_CLOCK=YYYY-MM-DDTHH:MM:SS   the frozen RTC (default 2026-10-29T12:00:00, local time = UTC)
//   BWPORT_LSTEP_MAXBOOT=N             give up (exit 3) if no anchor by game frame N (default 20000)
//   BWPORT_LSTEP_SHIFT=0|1|2          which picture is record f's screens: 0 = the render inside game frame f's own
//                                      V-blank wait (3D of f, 2D before GFL_VBlankUpdate's transfers), 1 = the next
//                                      wait's render (3D of f+1), 2 (default) = an extra render right after frame f's
//                                      GFL_VBlankUpdate: the 3D swapped in f and the 2D transferred in it, as the DS
//                                      shows after the V-blank (on a walk 2 equals 0, 1 is a frame late)
//
// The hooks (port/meson.build: -Wl,--wrap=PORT_WaitIrq,GFL_VBlankUpdate,buildSeed,time): PORT_WaitIrq (main_threads.c: the game lock released around OS_WaitIrq) on the main loop's call (the return
// address inside NitroMain) is the end of a game frame; buildSeed gives MATH_InitRand32's constants; time() is frozen.
// The variables are read with the game's own types where it exports them, mirrors of the file-local ones, and the
// statics (data_020aa1a0 ...) by the exe's ELF symbol table. Linux only (the ELF reading).
#if defined(__linux__)
#include <pthread.h>
#include <nitro.h>
#include <elf.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "battle/btl_main.h"
#include "field/field_actor_internal.h"
#include "field/field_internal.h"
#include "field/field_player_core.h"
#include "field/field_script.h"
#include "field/field_script_event.h"
#include "field/field_script_supervisor.h"
#include "field/player_state.h"
#include "gfl/random.h"
#include "gfl/std.h"
#include "pml/poke_party.h"
#include "save/event_work.h"
#include "system/game_data.h"
#include "system/game_event.h"
#include "system/game_system.h"
#include "system/gfl_use.h"
#include "system/vm.h"

extern void NitroMain(void);
extern const unsigned char *bwport_last_rgba; // port/bridge/vcore_bridge.c: the last picture (256x384 RGBA)
extern unsigned bwport_last_rgba_count;       // pictures drawn so far
extern const unsigned char *bwport_render_now(void); // an extra render of the state now
void __real_GFL_VBlankUpdate(void);

void __real_PORT_WaitIrq(BOOL clear, OSIrqMask flags);
void __real_buildSeed(MATHRandContext32 *context);
time_t __real_time(time_t *t);

// ---------------------------------------------------------------------------------------------------------------
// state

static int sOn;
static FILE *sOut;
static int sFramesWanted = -1;
static const char *sPngDir;
static int sPngA = 0, sPngB = 0x7fffffff;
static const char *sSave = "save.bin";
static const char *sInputsName = "";
static int sShift = 2;
static int sSkip = 1; // BWPORT_LSTEP_SKIP: screens on frames f % sSkip == 0
static unsigned sEmitCount; // the bridge's picture count at the last SHIFT=0/1 screens
static int sMaxBoot = 20000;
static time_t sClock;
static char sClockStr[32] = "2026-10-29T12:00:00";
static int sAnchored;
static u32 sAnchorG, sAnchorVb;
static u16 sCurKeys; // the keys applied for the frame being run (what its HID read saw)

typedef struct {
    u16 mask;
    s32 at, len;
} Press;
static Press sBoot[256], sInp[4096];
static int sBootN, sInpN;
static const char *const kKeyNames[12] = {"a", "b", "select", "start", "right", "left", "up", "down", "r", "l", "x", "y"};

// ---------------------------------------------------------------------------------------------------------------
// the clock and the seed

time_t __wrap_time(time_t *t) {
    time_t v = sOn ? sClock : __real_time(NULL);
    if (t != NULL) {
        *t = v;
    }
    return v;
}

void __wrap_buildSeed(MATHRandContext32 *context) {
    if (!sOn) {
        __real_buildSeed(context);
        return;
    }
    // MATH_InitRand32's constants
    context->x = 0xFEDCBA9876543210ULL;
    context->mul = 0x5D588B656C078965ULL;
    context->add = 0x269EC3ULL;
}

// ---------------------------------------------------------------------------------------------------------------
// the ELF symbol table

typedef struct {
    u64 value, size;
    const char *name;
    u8 info;
} Sym;
static Sym *sSyms; // sorted by value
static size_t sSymN;
static u64 sBias;
static u64 sMainLo, sMainSize;

static int SymCmp(const void *a, const void *b) {
    const Sym *x = a, *y = b;
    if (x->value != y->value) {
        return x->value < y->value ? -1 : 1;
    }
    // a global function first
    int rx = ELF64_ST_BIND(x->info) == STB_GLOBAL ? 0 : 1, ry = ELF64_ST_BIND(y->info) == STB_GLOBAL ? 0 : 1;
    if (rx != ry) {
        return rx - ry;
    }
    rx = ELF64_ST_TYPE(x->info) == STT_FUNC ? 0 : 1;
    ry = ELF64_ST_TYPE(y->info) == STT_FUNC ? 0 : 1;
    return rx - ry;
}

static int LoadSyms(void) {
    int fd = open("/proc/self/exe", O_RDONLY);
    struct stat st;
    u8 *map;
    Elf64_Ehdr *eh;
    Elf64_Shdr *sh;
    size_t i, n = 0;

    if (fd < 0 || fstat(fd, &st) != 0) {
        return 0;
    }
    map = mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (map == MAP_FAILED) {
        return 0;
    }
    eh = (Elf64_Ehdr *)map;
    sh = (Elf64_Shdr *)(map + eh->e_shoff);
    for (i = 0; i < eh->e_shnum; i++) {
        if (sh[i].sh_type == SHT_SYMTAB) {
            Elf64_Sym *sy = (Elf64_Sym *)(map + sh[i].sh_offset);
            const char *str = (const char *)(map + sh[sh[i].sh_link].sh_offset);
            size_t cnt = sh[i].sh_size / sizeof(Elf64_Sym), k;

            sSyms = malloc(cnt * sizeof(Sym));
            for (k = 0; k < cnt; k++) {
                int t = ELF64_ST_TYPE(sy[k].st_info);
                if (sy[k].st_shndx == SHN_UNDEF || sy[k].st_shndx >= SHN_LORESERVE || sy[k].st_name == 0 ||
                    (t != STT_FUNC && t != STT_OBJECT && t != STT_NOTYPE)) {
                    continue;
                }
                sSyms[n].value = sy[k].st_value;
                sSyms[n].size = sy[k].st_size;
                sSyms[n].name = str + sy[k].st_name; // the mapping stays
                sSyms[n].info = sy[k].st_info;
                n++;
            }
            break;
        }
    }
    sSymN = n;
    if (n == 0) {
        return 0;
    }
    qsort(sSyms, n, sizeof(Sym), SymCmp);
    for (i = 0; i < n; i++) {
        if (strcmp(sSyms[i].name, "NitroMain") == 0) {
            sBias = (u64)(uintptr_t)NitroMain - sSyms[i].value;
            sMainLo = sSyms[i].value;
            sMainSize = sSyms[i].size;
        }
    }
    return sMainSize != 0;
}

static void *SymAddr(const char *name) {
    size_t i;
    for (i = 0; i < sSymN; i++) {
        if (strcmp(sSyms[i].name, name) == 0) {
            return (void *)(uintptr_t)(sSyms[i].value + sBias);
        }
    }
    return NULL;
}

static const char *SymName(const void *p) {
    u64 a = (u64)(uintptr_t)p - sBias;
    size_t lo = 0, hi = sSymN;

    while (lo < hi) {
        size_t m = (lo + hi) / 2;
        if (sSyms[m].value < a) {
            lo = m + 1;
        } else {
            hi = m;
        }
    }
    return lo < sSymN && sSyms[lo].value == a ? sSyms[lo].name : NULL;
}

// ---------------------------------------------------------------------------------------------------------------
// output buffer

typedef struct {
    char *p;
    size_t n, cap;
} Out;

static void Put(Out *o, const char *fmt, ...) {
    va_list ap;
    int k;

    for (;;) {
        va_start(ap, fmt);
        k = vsnprintf(o->p + o->n, o->cap - o->n, fmt, ap);
        va_end(ap);
        if ((size_t)k < o->cap - o->n) {
            o->n += (size_t)k;
            return;
        }
        o->cap = o->cap * 2 + (size_t)k + 64;
        o->p = realloc(o->p, o->cap);
    }
}

static u32 sCrcTab[256];
static u32 Crc32(u32 crc, const u8 *d, size_t n) {
    crc = ~crc;
    while (n--) {
        crc = sCrcTab[(crc ^ *d++) & 255] ^ (crc >> 8);
    }
    return ~crc;
}

// ---------------------------------------------------------------------------------------------------------------
// the variables (tools/lockstep/vars.txt, in its order)

// mirrors of the types local to a .c file (the same layouts as the game's)
typedef struct LsEvent {
    struct LsEvent *parent;
    GameEventCallback callback;
    u32 state;
    void *data;
} LsEvent;
typedef struct LsProc {
    const GameProcFunctions *functions;
    s32 overlayId;
    u32 state;
    u32 subState;
    void *param;
    void *work;
} LsProc;
typedef struct LsProcMgr {
    HeapID heapId;
    s32 unk4;
    LsProc *proc;
} LsProcMgr;
typedef struct {
    ScriptWork *work;
    FieldScriptSupervisor *supervisor;
} LsScriptCall;

static u32 *sFrameCtr;       // data_020aa1a0
static GFLUseWork **sUseWork; // data_020aa1a4
static u8 **sStdTables;      // data_021469f8: mt at +0, mti at +0x9c0
static const void *sBtlProcFns;
typedef struct { u16 heapId; u16 *moves; MATHRandContext32 rand; } LsBtlCalc; // btl_calc.c's BtlCalcWork
static LsBtlCalc *sBtlCalc; // data_ov093_021f6360

static LsEvent *Evt(void) { return g_GameSystem ? (LsEvent *)g_GameSystem->event : NULL; }
static LsProc *Proc(void) {
    LsProcMgr *m = g_GameSystem ? (LsProcMgr *)g_GameSystem->procManager : NULL;
    return m ? m->proc : NULL;
}
static Field *Fld(void) { return g_GameSystem ? g_GameSystem->field : NULL; }
static FieldPlayerCore *Core(void) {
    Field *f = Fld();
    return f && f->player ? f->player->core : NULL;
}
static FieldActor *Act(void) {
    FieldPlayerCore *c = Core();
    return c ? c->actor : NULL;
}
static PlayerState *PState(void) { return g_GameSystem && g_GameSystem->gameData ? GameData_GetPlayerState(g_GameSystem->gameData) : NULL; }
static LsScriptCall *ScrCall(void) {
    LsEvent *e = Evt();
    return e && e->callback == EventScriptCall_Callback ? (LsScriptCall *)e->data : NULL;
}
static VM *Vm0(void) {
    LsScriptCall *c = ScrCall();
    return c && c->supervisor ? c->supervisor->vms[0] : NULL;
}
static BtlMainModule *Btl(void) {
    LsProc *p = Proc();
    return p && sBtlProcFns != NULL && (const void *)p->functions == sBtlProcFns ? (BtlMainModule *)p->work : NULL;
}

static void Null(Out *o) { Put(o, "null"); }
static void Num(Out *o, s64 v) { Put(o, "%lld", (long long)v); }
static void UNum(Out *o, u64 v) { Put(o, "%llu", (unsigned long long)v); }
static void Hex64(Out *o, u64 v) { Put(o, "\"0x%016llx\"", (unsigned long long)v); }
static void Fn(Out *o, const void *p) {
    const char *n;
    if (p == NULL) {
        Null(o);
    } else if ((n = SymName(p)) != NULL) {
        Put(o, "\"%s\"", n);
    } else {
        Put(o, "\"?0x%llx\"", (unsigned long long)((u64)(uintptr_t)p - sBias));
    }
}
static void Hash(Out *o, const void *p, size_t n) { Put(o, "\"%08x\"", Crc32(0, p, n)); }

#define VAR(id, body) static void id(Out *o) { body }
#define NEEDA(a) FieldActor *a = Act(); if (!a) { Null(o); return; }
#define NEEDF(f) Field *f = Fld(); if (!f) { Null(o); return; }
#define NEEDE(e) LsEvent *e = Evt(); if (!e) { Null(o); return; }
#define NEEDP(p) LsProc *p = Proc(); if (!p) { Null(o); return; }
#define NEEDV(v) VM *v = Vm0(); if (!v) { Null(o); return; }
#define NEEDB(b) BtlMainModule *b = Btl(); if (!b) { Null(o); return; }

VAR(v_frame, UNum(o, *sFrameCtr);)
VAR(v_rng_lc, if (!*sUseWork) { Null(o); return; } Hex64(o, (*sUseWork)->rand.x);)
VAR(v_mt_index, if (!*sStdTables) { Null(o); return; } Num(o, *(s32 *)(*sStdTables + 0x9c0));)
VAR(v_mt, if (!*sStdTables) { Null(o); return; } Hash(o, *sStdTables, 0x9c0);)
VAR(v_event, Put(o, "%d", Evt() != NULL);)
VAR(v_event_cb, NEEDE(e) Fn(o, (const void *)e->callback);)
VAR(v_event_state, NEEDE(e) UNum(o, e->state);)
VAR(v_event_pcb, NEEDE(e) if (!e->parent) { Null(o); return; } Fn(o, (const void *)e->parent->callback);)
VAR(v_proc_fn, NEEDP(p) Fn(o, p->functions);)
VAR(v_proc_state, NEEDP(p) UNum(o, p->state);)
VAR(v_field, Put(o, "%d", Fld() != NULL);)
VAR(v_f_routine, NEEDF(f) UNum(o, f->routineID);)
VAR(v_f_phase, NEEDF(f) UNum(o, f->routineAlternator);)
VAR(v_f_loaded, NEEDF(f) UNum(o, f->routineState);)
VAR(v_f_zone, NEEDF(f) UNum(o, f->zoneId);)
VAR(v_p_x, NEEDA(a) Num(o, a->gPos.x);)
VAR(v_p_y, NEEDA(a) Num(o, a->gPos.y);)
VAR(v_p_z, NEEDA(a) Num(o, a->gPos.z);)
VAR(v_p_wx, NEEDA(a) Num(o, a->wPos.x);)
VAR(v_p_wy, NEEDA(a) Num(o, a->wPos.y);)
VAR(v_p_wz, NEEDA(a) Num(o, a->wPos.z);)
VAR(v_p_face, NEEDA(a) UNum(o, a->faceDir);)
VAR(v_p_motion, NEEDA(a) UNum(o, a->motionDir);)
VAR(v_p_zone, NEEDA(a) UNum(o, a->zoneId);)
VAR(v_p_acmd, NEEDA(a) UNum(o, a->acmdState);)
VAR(v_p_move, FieldPlayerCore *c = Core(); if (!c) { Null(o); return; } UNum(o, c->moveStatus);)
VAR(v_s_zone, PlayerState *s = PState(); if (!s) { Null(o); return; } UNum(o, s->zoneId);)
VAR(v_s_x, PlayerState *s = PState(); if (!s) { Null(o); return; } Num(o, s->position.x);)
VAR(v_s_z, PlayerState *s = PState(); if (!s) { Null(o); return; } Num(o, s->position.z);)
VAR(v_sc_id, LsScriptCall *c = ScrCall(); if (!c || !c->work) { Null(o); return; } UNum(o, c->work->scriptId);)
VAR(v_sc_vms, LsScriptCall *c = ScrCall(); if (!c || !c->supervisor) { Null(o); return; } UNum(o, c->supervisor->vmCount);)
VAR(v_sc_state, NEEDV(v) UNum(o, v->state);)
VAR(v_sc_pc, NEEDV(v) UNum(o, (u32)(v->pc - (const u8 *)v->unk_2c));)
VAR(v_evwork, if (!g_GameSystem || !g_GameSystem->gameData) { Null(o); return; }
    EventWork *w = GameData_GetEventWork(g_GameSystem->gameData); if (!w) { Null(o); return; } Hash(o, w, 0x3ea);)
VAR(v_pcount, if (!g_GameSystem || !g_GameSystem->gameData) { Null(o); return; }
    PokeParty *p = GameData_GetParty(g_GameSystem->gameData); if (!p) { Null(o); return; } Num(o, p->count);)
VAR(v_party, if (!g_GameSystem || !g_GameSystem->gameData) { Null(o); return; }
    PokeParty *p = GameData_GetParty(g_GameSystem->gameData); if (!p) { Null(o); return; } Hash(o, p, 0x534);)
VAR(v_b_rng, NEEDB(b) Hex64(o, b->rand.x);)
VAR(v_b_calc, NEEDB(b) (void)b; if (!sBtlCalc) { Null(o); return; } Hex64(o, sBtlCalc->rand.x);)
VAR(v_b_main, NEEDB(b) Fn(o, (const void *)b->mainFunc);)
VAR(v_b_result, NEEDB(b) UNum(o, b->result);)

static const struct {
    const char *name;
    void (*fn)(Out *);
} kVars[] = {
    {"game.frame", v_frame}, {"rng.lc", v_rng_lc}, {"rng.mt.index", v_mt_index}, {"rng.mt", v_mt},
    {"event", v_event}, {"event.cb", v_event_cb}, {"event.state", v_event_state}, {"event.parent.cb", v_event_pcb},
    {"proc.fn", v_proc_fn}, {"proc.state", v_proc_state},
    {"field", v_field}, {"field.routine", v_f_routine}, {"field.phase", v_f_phase}, {"field.loaded", v_f_loaded},
    {"field.zone", v_f_zone},
    {"player.x", v_p_x}, {"player.y", v_p_y}, {"player.z", v_p_z}, {"player.wx", v_p_wx}, {"player.wy", v_p_wy},
    {"player.wz", v_p_wz}, {"player.face", v_p_face}, {"player.motion", v_p_motion}, {"player.zone", v_p_zone},
    {"player.acmd", v_p_acmd}, {"player.move", v_p_move},
    {"state.zone", v_s_zone}, {"state.x", v_s_x}, {"state.z", v_s_z},
    {"script.id", v_sc_id}, {"script.vms", v_sc_vms}, {"script.vm0.state", v_sc_state}, {"script.vm0.pc", v_sc_pc},
    {"eventwork", v_evwork}, {"party.count", v_pcount}, {"party", v_party},
    {"battle.rng", v_b_rng}, {"battle.calc.rng", v_b_calc}, {"battle.main", v_b_main}, {"battle.result", v_b_result},
};

// ---------------------------------------------------------------------------------------------------------------
// the keys

static void ParseKeys(const char *text, Press *tab, int *count, int max) {
    char *buf = strdup(text), *tok, *save, *c;

    // comments to the end of the line
    for (c = buf; *c; c++) {
        if (*c == '#') {
            while (*c && *c != '\n') {
                *c++ = ' ';
            }
            if (!*c) {
                break;
            }
        }
    }
    for (tok = strtok_r(buf, " ,\t\r\n", &save); tok != NULL; tok = strtok_r(NULL, " ,\t\r\n", &save)) {
        char *at = strchr(tok, '@'), *star = strchr(tok, '*');
        size_t len;
        int i, hold = 4;
        Press *k;

        if (at == NULL || at[1] != 'f' || *count >= max) {
            fprintf(stderr, "bwport: lstep: %s is not KEY[*fHOLD]@fN\n", tok);
            continue;
        }
        if (star != NULL && star > at) {
            star = NULL;
        }
        len = (size_t)((star != NULL ? star : at) - tok);
        k = &tab[*count];
        k->mask = 0;
        for (i = 0; i < 12; i++) {
            if (strlen(kKeyNames[i]) == len && strncmp(tok, kKeyNames[i], len) == 0) {
                k->mask = (u16)(1 << i);
            }
        }
        if (k->mask == 0) {
            fprintf(stderr, "bwport: lstep: no DS key %.*s\n", (int)len, tok);
            continue;
        }
        if (star != NULL) {
            hold = atoi(star + 1 + (star[1] == 'f'));
        }
        k->len = hold < 2 ? 2 : hold;
        k->at = atoi(at + 2);
        (*count)++;
    }
    free(buf);
}

static u16 KeysAt(const Press *tab, int n, s32 frame) {
    u16 m = 0;
    int i;
    for (i = 0; i < n; i++) {
        if (frame >= tab[i].at && frame < tab[i].at + tab[i].len) {
            m |= tab[i].mask;
        }
    }
    return m;
}

// X and Y reach the ARM9 through the ARM7 (HW_BUTTON_XY_BUF); melonDS delivers them 1 game frame after the other
// keys (the field starts the menu on an even half-cycle: X from f200 opens it at f202, from f180 at f181), so xyDown
// is the key set of 1 frame earlier.
static void ApplyKeys(u16 down, u16 xyDown) {
    vu16 *xy = (vu16 *)HW_BUTTON_XY_BUF;
    s_reg_PAD_KEYINPUT = (u16)(0x03ff & ~down);
    *xy = (u16)((*xy & ~0x0c00) | (~xyDown & 0x0c00));
    sCurKeys = down;
}

static void KeysStr(Out *o, u16 m) {
    int i, first = 1;
    for (i = 0; i < 12; i++) {
        if (m & (1 << i)) {
            Put(o, "%s%s", first ? "" : "+", kKeyNames[i]);
            first = 0;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------
// the screens

static u32 ScreenCrc(const u8 *rgba) { // 256x192 RGBA8 -> 15-bit LE u16 crc
    static u8 buf[256 * 192 * 2];
    int i;
    for (i = 0; i < 256 * 192; i++) {
        u16 c = (u16)((rgba[i * 4] >> 3) | (rgba[i * 4 + 1] >> 3) << 5 | (rgba[i * 4 + 2] >> 3) << 10);
        buf[i * 2] = (u8)c;
        buf[i * 2 + 1] = (u8)(c >> 8);
    }
    return Crc32(0, buf, sizeof(buf));
}

static void ScreenGrid(Out *o, const u8 *rgba) {
    int cy, cx, x, y;
    for (cy = 0; cy < 6; cy++) {
        for (cx = 0; cx < 8; cx++) {
            u32 sr = 0, sg = 0, sb = 0;
            for (y = 0; y < 32; y++) {
                for (x = 0; x < 32; x++) {
                    const u8 *p = rgba + ((cy * 32 + y) * 256 + cx * 32 + x) * 4;
                    sr += p[0] >> 3;
                    sg += p[1] >> 3;
                    sb += p[2] >> 3;
                }
            }
            Put(o, "%02x%02x%02x", sr / 1024, sg / 1024, sb / 1024);
        }
    }
}

static void PutBE32(u8 *p, u32 v) { p[0] = (u8)(v >> 24); p[1] = (u8)(v >> 16); p[2] = (u8)(v >> 8); p[3] = (u8)v; }

static void PngChunk(FILE *f, const char *type, const u8 *d, u32 n) {
    u8 h[8];
    u32 crc;
    PutBE32(h, n);
    memcpy(h + 4, type, 4);
    fwrite(h, 1, 8, f);
    if (n) {
        fwrite(d, 1, n, f);
    }
    crc = Crc32(Crc32(0, (const u8 *)type, 4), d, n);
    PutBE32(h, crc);
    fwrite(h, 1, 4, f);
}

static void WritePng(const char *path, const u8 *rgba) { // 256x384 RGB, stored deflate
    enum { W = 256, H = 384, ROW = 1 + W * 3 };
    static u8 raw[H * ROW];
    u8 *z, ihdr[13];
    size_t rawn = (size_t)H * ROW, zn = 0, off = 0;
    u32 a = 1, b = 0;
    int x, y;
    FILE *f = fopen(path, "wb");

    if (!f) {
        return;
    }
    for (y = 0; y < H; y++) {
        raw[y * ROW] = 0;
        for (x = 0; x < W; x++) {
            memcpy(raw + y * ROW + 1 + x * 3, rgba + (y * W + x) * 4, 3);
        }
    }
    z = malloc(rawn + rawn / 65535 * 5 + 64);
    z[zn++] = 0x78;
    z[zn++] = 0x01;
    while (off < rawn) {
        size_t n = rawn - off > 65535 ? 65535 : rawn - off;
        z[zn++] = off + n == rawn;
        z[zn++] = (u8)n;
        z[zn++] = (u8)(n >> 8);
        z[zn++] = (u8)~n;
        z[zn++] = (u8)(~n >> 8);
        memcpy(z + zn, raw + off, n);
        zn += n;
        off += n;
    }
    for (off = 0; off < rawn; off++) {
        a = (a + raw[off]) % 65521;
        b = (b + a) % 65521;
    }
    PutBE32(z + zn, (b << 16) | a);
    zn += 4;
    fwrite("\x89PNG\r\n\x1a\n", 1, 8, f);
    PutBE32(ihdr, W);
    PutBE32(ihdr + 4, H);
    ihdr[8] = 8; ihdr[9] = 2; ihdr[10] = ihdr[11] = ihdr[12] = 0;
    PngChunk(f, "IHDR", ihdr, 13);
    PngChunk(f, "IDAT", z, (u32)zn);
    PngChunk(f, "IEND", NULL, 0);
    fclose(f);
    free(z);
}

// ---------------------------------------------------------------------------------------------------------------
// the frame end

static Out sPend; // the record of the last frame, up to the screens
static int sPendF2 = -1; // SHIFT=2: the record waits for GFL_VBlankUpdate's extra render
static int sPendF = -1;
static unsigned sPendCount;
static int sFrameRecords;

extern void acapture_close(void); // the sound capture and the acore WAV are closed here (_exit skips atexit)
extern void ab_wav_close(void);
static void Finish(int code) {
    acapture_close();
    ab_wav_close();
    fflush(sOut);
    fclose(sOut);
    _exit(code);
}

static void Emit(int f, const u8 *rgba) { // the pending record plus the screens of picture `rgba`
    Out o = sPend;
    if (rgba != NULL) {
        Put(&o, ", \"top\": \"%08x\", \"bot\": \"%08x\", \"gtop\": \"", ScreenCrc(rgba), ScreenCrc(rgba + 256 * 192 * 4));
        ScreenGrid(&o, rgba);
        Put(&o, "\", \"gbot\": \"");
        ScreenGrid(&o, rgba + 256 * 192 * 4);
        Put(&o, "\"");
    } else {
        Put(&o, ", \"top\": null, \"bot\": null, \"gtop\": null, \"gbot\": null");
    }
    Put(&o, "}\n");
    fwrite(o.p, 1, o.n, sOut);
    if (rgba != NULL && sPngDir != NULL && f >= sPngA && f <= sPngB) {
        char path[512];
        snprintf(path, sizeof(path), "%s/f%06d.png", sPngDir, f);
        WritePng(path, rgba);
    }
    sFrameRecords++;
    sPend.p = o.p; // grown buffer kept
    sPend.cap = o.cap;
}

static int AnchorHolds(void) {
    Field *f = Fld();
    return f != NULL && g_GameSystem->event == NULL && f->routineID == 3 && f->routineState == 1 &&
           f->routineAlternator == 0;
}

static void FrameEnd(void) {
    static int init;
    u32 g;
    int f;
    size_t i;
    Out *o = &sPend;

    if (!init) {
        init = 1;
        sFrameCtr = SymAddr("data_020aa1a0");
        sUseWork = SymAddr("data_020aa1a4");
        sStdTables = SymAddr("data_021469f8");
        sBtlProcFns = SymAddr("data_ov093_021efdac");
        sBtlCalc = SymAddr("data_ov093_021f6360");
        if (!sFrameCtr || !sUseWork || !sStdTables) {
            fprintf(stderr, "bwport: lstep: statics not in the symbol table\n");
            Finish(4);
        }
    }
    g = *sFrameCtr;

    // lstep_melon records at the first V-blank wait of each game frame, wherever it is: an event callback that
    // waits (OS_WaitVBlankIntr inside GameUpdate, e.g. the building-enter transition) is sampled mid-iteration
    // there (townmap f=69). The port does the same: one record per counter value, at its first wait.
    {
        static u32 sLastG = 0xFFFFFFFFu;

        if (g == sLastG) {
            return;
        }
        sLastG = g;
    }

    // the screens of the previous record: this wait's render (BWPORT_LSTEP_SHIFT=1)
    if (sShift == 1 && sPendF >= 0) {
        Emit(sPendF, bwport_last_rgba_count != sPendCount && (sSkip <= 1 || sPendF % sSkip == 0) ? bwport_last_rgba : NULL);
        sPendF = -1;
    }

    if (!sAnchored) {
        s32 lastBoot = 0;
        for (i = 0; i < (size_t)sBootN; i++) {
            if (sBoot[i].at + sBoot[i].len > lastBoot) {
                lastBoot = sBoot[i].at + sBoot[i].len;
            }
        }
        if ((s32)g >= lastBoot && AnchorHolds()) {
            Out h = {0};
            sAnchored = 1;
            sAnchorG = g;
            sAnchorVb = OS_GetVBlankCount();
            // the generators, reseeded
            (*sUseWork)->rand.x = 0x0123456789ABCDEFULL;
            GFL_RandomUpdateMT(0x01234567);
            Put(&h, "{\"lockstep\": 1, \"side\": \"port\", \"rom\": \"baserom_b_us.nds\", \"save\": \"%s\", "
                    "\"inputs\": \"%s\", \"clock\": \"%s\", \"anchor_g\": %u, \"anchor_vblank\": %u, "
                    "\"vars\": \"tools/lockstep/vars.txt\"}\n",
                sSave, sInputsName, sClockStr, g, sAnchorVb);
            fwrite(h.p, 1, h.n, sOut);
            free(h.p);
            if (sFramesWanted < 0) {
                s32 last = 0;
                for (i = 0; i < (size_t)sInpN; i++) {
                    if (sInp[i].at + sInp[i].len > last) {
                        last = sInp[i].at + sInp[i].len;
                    }
                }
                sFramesWanted = last + 300;
            }
        } else if ((int)g > sMaxBoot) {
            fprintf(stderr, "bwport: lstep: no anchor by game frame %u\n", g);
            Finish(3);
        }
    }

    if (sAnchored) {
        f = (int)(g - sAnchorG);
        if (f >= sFramesWanted) {
            if (!sShift && sPendF >= 0) {
                Emit(sPendF, NULL);
            }
            Finish(0);
        }
        o->n = 0;
        if (o->p == NULL) {
            o->cap = 4096;
            o->p = malloc(o->cap);
        }
        Put(o, "{\"f\": %d, \"g\": %u, \"vb\": %u, \"lag\": 0, \"keys\": \"", f, g, OS_GetVBlankCount());
        KeysStr(o, sCurKeys);
        Put(o, "\"");
        sPendF = f;
        sPendCount = bwport_last_rgba_count;
        // the vars are in the record before the screens: written now
        {
            Out v = {0};
            v.cap = 4096;
            v.p = malloc(v.cap);
            Put(o, ", \"vars\": {");
            for (i = 0; i < sizeof(kVars) / sizeof(kVars[0]); i++) {
                v.n = 0;
                kVars[i].fn(&v);
                Put(o, "%s\"%s\": %.*s", i ? ", " : "", kVars[i].name, (int)v.n, v.p);
            }
            Put(o, "}");
            free(v.p);
        }
        // the keys of the next frame
        ApplyKeys(KeysAt(sInp, sInpN, f + 1), KeysAt(sInp, sInpN, f));
        if (sShift == 0) {
            Emit(f, sSkip <= 1 || (f % sSkip == 0 && bwport_last_rgba_count != sEmitCount) ? bwport_last_rgba : NULL);
            sEmitCount = bwport_last_rgba_count;
            sPendF = -1;
        } else if (sShift == 2) {
            sPendF2 = f;
            sPendF = -1;
        }
        return;
    }
    ApplyKeys(KeysAt(sBoot, sBootN, (s32)g + 1), KeysAt(sBoot, sBootN, (s32)g));
}

static pthread_t sMainThr;
static int sMainThrSet;

void __wrap_GFL_VBlankUpdate(void) {
    __real_GFL_VBlankUpdate();
    if (sOn && sPendF2 >= 0) {
        Emit(sPendF2, sSkip <= 1 || sPendF2 % sSkip == 0 ? bwport_render_now() : NULL);
        sPendF2 = -1;
    }
}

void __real_OS_WaitVBlankIntr(void);
// OS_WaitVBlankIntr (an event callback's wait inside GameUpdate): a sample point on the main thread, see FrameEnd
void __wrap_OS_WaitVBlankIntr(void) {
    __real_OS_WaitVBlankIntr();
    if (sOn && sMainThrSet && pthread_equal(sMainThr, pthread_self())) {
        FrameEnd();
    }
}

void __wrap_PORT_WaitIrq(BOOL clear, OSIrqMask flags) {
    int inMain = 0;

    if (sOn && (flags & OS_IE_V_BLANK) && sSymN != 0) {
        u64 ra = (u64)(uintptr_t)__builtin_return_address(0) - sBias;
        inMain = ra >= sMainLo && ra < sMainLo + sMainSize;
        if (inMain && !sMainThrSet) {
            sMainThr = pthread_self();
            sMainThrSet = 1;
        }
        // a V-blank wait deeper in the frame, on the main thread, is a sample point too (FrameEnd keeps the first)
        if (!inMain && sMainThrSet && pthread_equal(sMainThr, pthread_self())) {
            inMain = 1;
        }
    }
    __real_PORT_WaitIrq(clear, flags);
    if (inMain) {
        FrameEnd();
    }
}

// ---------------------------------------------------------------------------------------------------------------
// setup

static char *ReadFileOrText(const char *s) {
    FILE *f = fopen(s, "rb");
    char *buf;
    long n;

    if (!f) {
        return strdup(s);
    }
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = malloc((size_t)n + 1);
    n = (long)fread(buf, 1, (size_t)n, f);
    buf[n] = 0;
    fclose(f);
    return buf;
}

__attribute__((constructor)) static void LstepInit(void) {
    const char *env = getenv("BWPORT_LSTEP"), *e;
    char *spec, *comma, *txt;
    struct tm tm = {0};
    int i;

    if (env == NULL || *env == 0) {
        return;
    }
    spec = strdup(env);
    comma = strchr(spec, ',');
    if (comma) {
        *comma = 0;
        sFramesWanted = atoi(comma + 1);
    }
    sOut = fopen(spec, "wb");
    if (!sOut) {
        fprintf(stderr, "bwport: lstep: cannot write %s\n", spec);
        exit(2);
    }
    for (i = 0; i < 256; i++) {
        u32 c = (u32)i, k;
        for (k = 0; k < 8; k++) {
            c = c & 1 ? 0xEDB88320u ^ (c >> 1) : c >> 1;
        }
        sCrcTab[i] = c;
    }
    if ((e = getenv("BWPORT_LSTEP_PNG")) != NULL && *e) {
        char *p = strdup(e), *c = strchr(p, ',');
        if (c) {
            *c = 0;
            sscanf(c + 1, "%d-%d", &sPngA, &sPngB);
        }
        sPngDir = p;
        mkdir(p, 0777);
    }
    if ((e = getenv("BWPORT_LSTEP_SAVE")) != NULL && *e) {
        sSave = e;
    }
    if ((e = getenv("BWPORT_LSTEP_SKIP")) != NULL && atoi(e) > 1) {
        sSkip = atoi(e);
        setenv("BWPORT_FRAMESKIP", e, 1); // the bridge draws only what the dump records, and the capture frames
    }
    if ((e = getenv("BWPORT_LSTEP_SHIFT")) != NULL) {
        sShift = atoi(e);
    }
    if ((e = getenv("BWPORT_LSTEP_MAXBOOT")) != NULL) {
        sMaxBoot = atoi(e);
    }
    if ((e = getenv("BWPORT_LSTEP_CLOCK")) != NULL && *e) {
        snprintf(sClockStr, sizeof(sClockStr), "%s", e);
    }
    {
        int Y, M, D, h, m, s;
        if (sscanf(sClockStr, "%d-%d-%dT%d:%d:%d", &Y, &M, &D, &h, &m, &s) == 6) {
            tm.tm_year = Y - 1900; tm.tm_mon = M - 1; tm.tm_mday = D; tm.tm_hour = h; tm.tm_min = m; tm.tm_sec = s;
            sClock = timegm(&tm);
        }
    }
    setenv("TZ", "UTC", 1); // local time is the frozen time, no zone
    tzset();
    setenv("BWPORT_RENDERER", "vcore", 1); // the picture the screens are read from
    if ((e = getenv("BWPORT_LSTEP_INPUTS")) != NULL && *e) {
        sInputsName = e;
        txt = ReadFileOrText(e);
        ParseKeys(txt, sInp, &sInpN, 4096);
        free(txt);
    }
    e = getenv("BWPORT_LSTEP_BOOT");
    ParseKeys(e != NULL ? e : "start@f600 start@f900 start@f1300 a@f1700", sBoot, &sBootN, 256);
    if (!LoadSyms()) {
        fprintf(stderr, "bwport: lstep: no ELF symbol table\n");
        exit(2);
    }
    sOn = 1;
}
#endif // __linux__
